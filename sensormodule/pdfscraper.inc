<?php
//**************************************************************
//  NAME: sensormodule_uploadForm
//  USE: Produces HTML for the Datasheet PDF upload page
//  DATE: 17/10/2020
//**************************************************************
function sensormodule_uploadForm(&$form_state) {
  $form = array();
  $form['#attributes'] = array(
    'enctype' => 'multipart/form-data'
  );
  $form['file'] = array(
    '#type' => 'file',
    '#title' => t('Upload new PDF Datasheet'),
    '#size' => 40,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Upload',
    '#attributes' => array('class' => 'sensor-search-btn sensormodule-btn')
  );
  return $form;
}

//**************************************************************
//  NAME: validate_datasheet_upload
//  USE: Validates whether the uploaded pdf has a valid filename
//  DATE: 17/10/2020
//**************************************************************
function validate_datasheet_upload($file) {
  
  $errors = array();
  // Filename must be less that 100 characters long
  if(strlen($file->filename) > 100) {
    $errors[] = t('Filename exceeds 100 characer limit. Please rename the file and try again.');
  }
  // Filename must not contain any potentially dangerous characters
  if(checkAlphanumeric($file->filename)) {
    $errors[] = t('Filename contains illegal characters.');
  }
  // Filetype must be a pdf
  if(strcmp($file->filemime, 'application/pdf') != 0) {
    $errors[] = t('Incorrect filetype. File must be a PDF.');
  }
  return $errors;
}

//**************************************************************
//  NAME: sensormodule_uploadForm_submit
//  USE: Submit function for datasheet upload form
//  DATE: 17/10/2020
//**************************************************************
function sensormodule_uploadForm_submit($form, &$form_state) {

  // Get sensormodule data directory path -> sites/default/files/sensormodule/
  $dir = file_directory_path().'/sensormodule/';
  // Add validator
  $validators['validate_datasheet_upload'] = array();

  // CHANGE TO FILE EXISTS RENAME BEFORE HANDOVER
  $file = file_save_upload('file', $validators, $dir, FILE_EXISTS_RENAME);

  // If file was successfully uploaded continue
  if($file != NULL) {
    // Get file name
    $filename = $file->filename;

    // File upload path for clean up later
    $destination = $file->filepath;
    
    // PDF scraper python command -> cd to correct directory then call the scraper supplying the uploaded pdf.
    $command = "(cd sites/all/modules/sensormodule/ && python3 -m pdfscraper.pdf_scraper '../../../default/files/sensormodule/".$filename."')";
    
    // Execute the command and receive json results
    $json = shell_exec($command);
    
    // Decode the JSON into an associative array
    $decodedJson = json_decode($json, true);

    // -CHECK FOR PDF SCRAPER ERRORS-
    // Fatal unknown error during scraping
    if(array_key_exists("fatal_error", $decodedJson)) {
      if($decodedJson['fatal_error']) {
        cleanupPdf($destination);
        drupal_set_message("A fatal error has occurred during the pdf scraping process, contact your System Administrator.", "error");
        drupal_goto("sensormodule/edit/sensor/upload");
      }
    }
    // Scraper could not find the supplied file path
    else if(array_key_exists("file_found", $decodedJson)) {
      if(!$decodedJson['file_found']) {
        cleanupPdf($destination);
        drupal_set_message("The PDF scraper was unable to find the specified file, please check the supplied file path and try again", "error");
        drupal_goto("sensormodule/edit/sensor/upload");
      }
    }
    // Scraper could not find a parser for the supplied datasheet type 
    else if(array_key_exists("format_parser_detected", $decodedJson)) {
      if(!$decodedJson['format_parser_detected']) {
        cleanupPdf($destination);
        drupal_set_message("The PDF scraper was unable find a parser for the PDF provided, this most likely means the supplied PDF is not a gas sensor datasheet.", "error");
        drupal_goto("sensormodule/edit/sensor/upload");
      }
    }
    
    // Else scraper should have functioned as expected so continue with insert
    $newID = insertScrapedSensorData($decodedJson);
    cleanupPdf($destination);
    drupal_set_message("Successfully added sensor! Please confirm the scraped details and save.");
    drupal_goto('sensormodule/edit/sensor/modify/', "sensorID=".$newID);
  }
  // Else file did not successfully upload
  else {
    drupal_set_message("There was an error uploading your file or you supplied a NULL file. Please try again.", "error");
    drupal_goto("sensormodule/edit/sensor/upload");
  }
}


//**************************************************************
//  NAME: cleanupPdf
//  USE: Deletes the uploaded pdf and removes Drupals stored info for the file
//  DATE: 29/10/2020
//**************************************************************
function cleanupPdf($destination) {
  //Try to delete the file
  $success = file_delete($destination);

  //If the file was successfully deleted, update the database
  if ($success){
    db_query('DELETE FROM {files} WHERE filepath = "%s"', $destination);
  }

  // Double check to make sure file is deleted from drupals file table
  $query = db_query('SELECT * FROM {files} WHERE filepath = "%s"', $destination);
  $obj = db_fetch_object($query);
  if($obj != NULL) {
    $properties = var_export($obj, TRUE);
    drupal_set_message("There was a problem cleaning up the uploaded pdf at: '$destination'.<br>File Info:<br>$properties", "error");
  }
}

//**************************************************************
//  NAME: insertScrapedSensorData
//  USE: Inserts the supplied scraped sensor data into the database
//  DATE: 19/10/2020
//**************************************************************
function insertScrapedSensorData($data) {
  // Set brand id from brand name
  $brandID = getBrandID($data["BrandID"]);
  if($brandID == -1 || $brandID == 0) {
    drupal_set_message("The brand retrieved from the datasheet '".$data["BrandID"]."' does not exist in the database, add this new brand then try again.", "error");
    drupal_goto("sensormodule/edit/sensor/upload");
  }
  $data["BrandID"] = $brandID;

  // Set todays date as last review date
  $data['LastReview'] = getCurrentDate();
  // Generally always going to be 4-series
  $data['Series'] = '4-series';

  // Create insert query
  $query = createSensorInsertQuery($data);

  // Insert new sensor in database
  db_set_active("gas_sensor_db");
  db_query($query);

  // Get last inserted SensorID
  $newIDQuery = db_query("SELECT LAST_INSERT_ID()");
  $sensorID = db_result($newIDQuery);
  db_set_active("default");

  
  // Get gas count
  $gasCount = intval($data['GasCount']);

  //INSERT ERROR HAS OCCURED
  if($sensorID == 0) {
    drupal_set_message("MySQL insert error has occured, operation aborted.", "error");
    drupal_goto('sensormodule/edit/sensor/upload');
  }
  //INSERT WAS SUCCESSFUL CHECK IF GASSES NEED TO BE ADDED
  else if($gasCount > 0) {
    // Add each gasses detection data to database
    for($i=0; $i<$gasCount; $i++) {
      insertScrapedDetectData($sensorID, $data['Gasses'][$i]);
    }
  }

  return $sensorID;
}


//**************************************************************
//  NAME: insertScrapedDetectData
//  USE: Inserts the supplied scraped detect data into the database
//  DATE: 21/10/2020
//**************************************************************
function insertScrapedDetectData($sensorID, $data) {
  // Get gas ID
  $gasID = getGasID($data['Name']);
  if($gasID == -1) {
    drupal_set_message("The gas retrieved from the datasheet '".$data['Name']."' does not exist in the database, add this new gas then try again.", "error");
    drupal_goto("sensormodule/edit/sensor/modify/", "sensorID=$sensorID");
  }
  $data['SensorID'] = $sensorID;
  $data['GasID'] = $gasID;
  // Create insert query
  $detectQuery = createSensorDetectInsertQuery($data);

  // Insert into database
  db_set_active("gas_sensor_db");
  db_query($detectQuery);
  db_set_active("default");
}

?>