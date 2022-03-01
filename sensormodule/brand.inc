<?php
//==============================================================
// SEARCH
//==============================================================
/**
Name: sensormodule_brandSearchBar 
Goal: Provide a form for searching by brand1
Date: 8/9/2020
*/
function sensormodule_brandSearchBar(&$form_state) {
  $form['search'] = array(
    '#type' => 'fieldset',
    '#title' => t('Brand Search'),
    '#tree' => TRUE,
    '#attributes' => array('class' => 'sensor-search-fieldset'),
  );

  $form['search']['searchTerm'] = array(
    '#type' => 'textfield',
    '#title' => t('Search'),
    '#size' => 50,
    '#maxlength' => 50,
    '#description' => t('Enter the desired brand name.'),
  );

  $form['search']['searchBtn'] = array(
    '#type' => 'submit',
    '#name' => 'Search',
    '#value' => t('Search'),
    '#attributes' => array('class' => 'sensor-search-btn sensormodule-btn')
  );

  $form['search']['showAllBtn'] = array(
    '#type' => 'submit',
    '#name' => 'ShowAll',
    '#value' => t('Show All'),
    '#attributes' => array('class' => 'sensor-search-btn sensormodule-btn')
  );

  return $form;
}

/**
Name: sensormodule_brandSearchBar_validate
Goal: Provide validation that input is not blank
Date: 8/9/2020
*/
function sensormodule_brandSearchBar_validate($form,&$form_state) {
  if($form_state['clicked_button']['#name'] == 'Search') {
    $searchTerm = $form_state['values']['search']['searchTerm'];
    if($searchTerm == "")
    {
      form_set_error('', t('Please enter a valid search term.')); 
    }
    else if(!checkAlphanumeric($searchTerm))
    {
      form_set_error('', t('Please enter only safe characters'));
    }  
  }
  else if($form_state['clicked_button']['#name'] == 'ShowAll'){
    drupal_goto("sensormodule/search/brand");
  }
}

/**
Name: sensormodule_brandSearchBar_submit
Goal: a hook to parse form information on submission
Date: 8/9/2020
*/
function sensormodule_brandSearchBar_submit($form,&$form_state) {
  $searchValue = $form_state['values']['search']['searchTerm'];  
  drupal_goto("sensormodule/search/brand", "search=".$searchValue);   
}


//**************************************************************
//  NAME: sensormodule_brandSearchResultsTable
//  USE: Performs DB query and produces a HTML table populated with results
//  DATE: 22/10/2020
//**************************************************************
function sensormodule_brandSearchResultsTable($form_state, $searchTerm='') {
  $limit = 25;
  db_set_active('gas_sensor_db');

  $header = array(
    array('data' => t('Name'), 'field' => 'Name'),
  );

  $tablesort = tablesort_sql($header);
  if($searchTerm != "") {
    $query = "SELECT BrandID, Name FROM Brand WHERE (BrandID LIKE '%".$searchTerm."%' OR Name LIKE '%".$searchTerm."%')";
    $result = pager_query($query.$tablesort, $limit, 0, NULL, $searchTerm);
  } else {
    $query = "SELECT BrandID, Name FROM Brand";
    $result = pager_query($query.$tablesort, $limit);
  }
  
  $form['tableButton'] = array(
    //Changing the theme here wasn't working for me either (with individual theme hooks as well as shared but with different themes)
    '#theme' => 'brandSearchResultsTable_theme',
    '#tree' => TRUE,
  );
  // Search result count
  $count = 0;
  while($item =db_fetch_object($result))
  {
    $key= $item->BrandID;
    $form['tableButton'][$key]['Name'] = array( '#value' => $item->Name);
    $form['tableButton'][$key]['RelatedSensor'] = array (
      '#type' => 'submit', 
      '#value' => t('All Sensors'), 
      '#name' => $item->BrandID,
      '#attributes' => array('class' => 'sensormodule-btn sensormodule-table-btn'),
    );
    $count++;
  }
  db_set_active('default');
  $form['tableButton']['pager'] = array('#value' => theme('pager', NULL, $limit, 0));

  //Display message if none found
  if($count == 0) {
    drupal_set_message("No brands like: '".$searchTerm."' were found.");
    drupal_goto("sensormodule/search/brand");
  }
  return $form;
}

//**************************************************************
//  NAME: theme_brandSearchResultsTable_theme
//  USE: Theme function for the table form above
//  DATE: 22/10/2020
//**************************************************************
function theme_brandSearchResultsTable_theme($form) {
  $header = array(
    #data
    array('data' => t('Brand'), 'field' => 'Name', 'class' => 'sensormodule-sensor-table-header'),
    array('data' => t('List Of Brand\'s Sensors'), 'class' => 'sensormodule-sensor-table-header'),
    theme('table_select_header_cell'),
  );
  $rows = array();
  foreach (element_children($form)as $index) {
    $row = array();
    $row[] = drupal_render($form[$index]['Name']);
    $row[] = drupal_render($form[$index]['RelatedSensor']);
    $rows[] = array('data' => $row, 'class' => 'sensormodule-sensor-table-row'); 
  }
  
  $output = theme('table', $header, $rows);
  if($form['pager']['#value']) {
    $output .= drupal_render($form['pager']);
  }

  $output .= drupal_render($form);
  $output = str_replace("<th class=\"select-all\"></th>", '', $output);
  return $output;
}

//**************************************************************
//  NAME: sensormodule_brandSearchResultsTable_submit
//  USE: Submit function to redirect to all sensors for selected brand
//  DATE: 22/10/2020
//**************************************************************
function sensormodule_brandSearchResultsTable_submit($form, &$form_state) {
  $brandID = $form_state['clicked_button']['#name'];
  $brandName = getBrandName($brandID);
  if($brandName == "") {
    drupal_set_message("Error occurred, could not find BrandID ($brandID) in the database.", "error");
    drupal_got("sensormodule/search/brand");
  }
  else {
    drupal_goto("sensormodule/search/sensor/", "searchTerm=$brandName&filter=brand-strict");
  }
}



//==============================================================
// ADD
//==============================================================
//***************************************************************
//  NAME: sensormodule_addBrandForm
//  USE: Loads page for user to add a new Brand
//  DATE: 21/10/2020
//***************************************************************
function sensormodule_addBrandForm(&$form_state) {

  $form['addBrand'] = array(
    '#title' => t('Add Brand'),
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#attributes' => array('class' => 'form-fieldset'),
  );

  $form['addBrand']['BrandName'] = array(
    '#type' => 'textfield',
    '#title' => t('New Brand Name'),
    '#maxlength' => 64,
  );

  $form['addBrand']['Add'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
    '#name' => 'Add',
    '#attributes' => array('class' => 'sensor-search-btn sensormodule-btn')
    );
 
   $form['addBrand']['Cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#name' => 'Cancel',
    '#attributes' => array('class' => 'sensor-search-btn sensormodule-btn')
  );
   return $form;
}
 
//***************************************************************
//  NAME: sensormodule_addBrandForm_validate
//  USE: Performs necessary validation on add brand form
//  DATE: 21/10/2020
//***************************************************************
function sensormodule_addBrandForm_validate($form, &$form_state) {
  $clicked_button = $form_state['clicked_button']['#name'];
  $brandName = $form_state['values']['addBrand']['BrandName'];

  if($clicked_button == "Cancel") {
    drupal_set_message("No changes made.");
    drupal_goto('sensormodule/edit/brand');
  }
  else if($clicked_button == "Add") {
    if(checkNull($brandName)) {
      form_error($form['addBrand']['BrandName'], "Name cannot be NULL.");
    }
    else if(!checkAlphanumeric($brandName)) {
      form_error($form['addBrand']['BrandName'], "Please enter only safe characters.");
    }
    else if(getBrandID($brandName) != -1) {
      form_error($form['addBrand']['BrandName'], "'$brandName' already exists in the database.");
    }
  }
}
 
//***************************************************************
//  NAME: sensormodule_addBrandForm_submit
//  USE: Performs necessary validation then adds Brand into Brand table in database
//  DATE: 21/10/2020
//***************************************************************
function sensormodule_addBrandForm_submit($form, &$form_state)
{
  $brandName = $form_state['values']['addBrand']['BrandName'];

  // CHANGE FOR FULLNAME
  db_set_active('gas_sensor_db');
  db_query("INSERT INTO Brand (Name) VALUES ('".$brandName."')");
  db_set_active("default");

  drupal_set_message("Successfully added '$brandName' to the database.");
  drupal_goto('sensormodule/edit/brand');
}

//==============================================================
// EDIT
//==============================================================
//***************************************************************
//  NAME: sensormodule_modifyBrandForm
//  USE: Button to allow a user to return to the Brand table page
//  DATE: 21/10/2020
//***************************************************************
function sensormodule_modifyBrandForm (&$form_state, $brandID){
  $name = getBrandName($brandID);
  $form['modifyBrandEdit'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#attributes' => array('class' => 'form-fieldset'),
  );
 
  $form['modifyBrandEdit']['brandName'] = array(
    '#type' => 'textfield',
    '#title' => t('Brand Name'),
    '#default_value' => $name,
    '#size' => 50,
    '#maxlength' => 50,
    '#description' => t('Enter new brand name.'),
  );

  $form['modifyBrandEdit']['Save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#name' => 'Save',
    '#attributes' => array('class' => 'sensor-search-btn sensormodule-btn')
  );

  $form['modifyBrandEdit']['Cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#name' => 'Cancel',
    '#attributes' => array('class' => 'sensor-search-btn sensormodule-btn')
  );

  $form['modifyBrandEdit']['Delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#name' => 'Delete',
    '#attributes' => array('class' => 'sensormodule-delete-btn sensormodule-btn')

  );


  $form['modifyBrandEdit']['brandID'] = array(
    '#type' => 'hidden',
    '#value' => $brandID,
  );
  return $form;
}

//***************************************************************
//  NAME: sensormodule_modifyBrandForm_validate
//  USE: Validate the modify brand form so that brandname is not null
//  DATE:  21/10/2020
//***************************************************************/
function sensormodule_modifyBrandForm_validate($form, &$form_state) {
  $clicked_button = $form_state['clicked_button']['#name'];
  $brandID = $form_state['values']['modifyBrandEdit']['brandID'];
  $brandName = $form_state['values']['modifyBrandEdit']['brandName'];
  
  if($clicked_button == "Delete") {
    drupal_goto('sensormodule/edit/brand/remove', "brandID=".$brandID);
  }
  else if($clicked_button == "Cancel") {
    drupal_set_message("No changes made.");
    drupal_goto('sensormodule/edit/brand');
  }
  else if($clicked_button == "Save") {
    if(checkNull($brandName)) {
      form_error($form['modifyBrandEdit']['brandName'], "Name cannot be NULL.");
    }
    else if(!checkAlphanumeric($brandName)) {
      form_error($form['modifyBrandEdit']['brandName'], "Please enter only safe characters.");
    }
    else if(getBrandID($brandName) != -1) {
      if(getBrandName($brandID) != $brandName) {
        form_error($form['modifyBrandEdit']['brandName'], "'$brandName' already exists in the database.");
      }
    }
  }
}

//***************************************************************
//  NAME: sensormodule_modifyGasPageForm_submit
//  USE: Saves the new name of the gas
//  DATE: 21/10/2020
//***************************************************************
function sensormodule_modifyBrandForm_submit($form, &$form_state) {
  $newBrandName = $form_state['values']['modifyBrandEdit']['brandName'];
  $brandID = $form_state['values']['modifyBrandEdit']['brandID'];
  
  db_set_active('gas_sensor_db');
  db_query("UPDATE Brand SET Name='%s' WHERE BrandID='%s'", $newBrandName, $brandID);
  db_set_active('default');
  drupal_set_message("Saved changes.");
  drupal_goto("sensormodule/edit/brand");
}

//***************************************************************
//  NAME: sensormodule_editBrandSearchBar
//  USE: Form for edit brand search bar
//  DATE: 16/09/2020
//***************************************************************
function sensormodule_editBrandSearchBar(&$form_state) {
  $form['searchBrandEdit'] = array(
    '#type' => 'fieldset',
    '#title' => t('Edit Brand'),
    '#tree' => TRUE,
    '#attributes' => array('class' => 'sensor-search-fieldset'),
  );

  $form['searchBrandEdit']['searchTerm'] = array(
    '#type' => 'textfield',
    '#title' => t('Brand Name'),
    '#size' => 60,
    '#maxlength' => 64,
    '#description' => t('Enter the desired brand name or ID.'),
    '#attributes' => array('class' => 'sensor-search-bar'),
  );

  $form['searchBrandEdit']['editSearchBtn'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
    '#name' => 'Search',
    '#attributes' => array('class' => 'sensor-search-btn sensormodule-btn')
  );
  
  $form['searchBrandEdit']['editShowAllBtn'] = array(
    '#type' => 'submit',
    '#value' => t('Show All'),
    '#name' => 'ShowAll',
    '#attributes' => array('class' => 'sensor-search-btn sensormodule-btn')
  );
  return $form;

}

//**************************************************************
//  NAME: sensormodule_editBrandSearchBar_submit
//  USE: Submit function for searchSensorForm, redirects to result page
//  DATE: 16/09/2020
//**************************************************************
function sensormodule_editBrandSearchBar_submit($form, &$form_state) {
  $searchTerm = $form_state['values']['searchBrandEdit']['searchTerm'];
  drupal_goto("sensormodule/edit/brand/", "search=".$searchTerm);
}

//**************************************************************
//  NAME: searchEditSensorForm_validate
//  USE: Validation function for searchSensorForm
//  DATE: 16/09/2020
//**************************************************************
function sensormodule_editBrandSearchBar_validate($form, &$form_state) {
  $clicked_button = $form_state['clicked_button']['#name'];
  $searchTerm = $form_state['values']['searchBrandEdit']['searchTerm'];
  
  if($clicked_button == "ShowAll") {
    drupal_goto("sensormodule/edit/brand");
  }
  if($clicked_button == "Search") {
    if($searchTerm == "")
    {
      form_error($form['searchBrandEdit']['searchTerm'], t('Please enter a valid search term.'));
    }
    else if(!checkAlphanumeric($searchTerm)) {
      form_error($form['searchBrandEdit']['searchTerm'], 'Please enter only safe characters.');
    }
  }

}

//***************************************************************
//  NAME: sensormodule_editBrandSearchResultsTable
//  USE: Table of Brands that are in the Brand database
//  DATE: 21/10/2020
//***************************************************************
function sensormodule_editBrandSearchResultsTable($form_state, $searchTerm) {

  $limit = 25;
  db_set_active('gas_sensor_db');
  
  $header = array(
    array('data' => t('BrandID'), 'field' => 'BrandID'),
    array('data' => t('Name'), 'field' => 'Name'),
    array('data' => t('Edit'), 'field' => 'Edit'),
    array('data' => t('Delete'), 'field' => 'Delete'),
  );

  $tablesort = tablesort_sql($header);
  if($searchTerm!="") {
    $query = "SELECT BrandID, Name FROM Brand 
              WHERE (Name LIKE '%".$searchTerm."%' OR
                     BrandID='".$searchTerm."')";
    $result = pager_query($query.$tablesort, $limit, 0, NULL, $searchTerm);
  }
  else  {    
    $query = "SELECT BrandID, Name FROM Brand";
    $result = pager_query($query.$tablesort, $limit);
  }

  $form['tableBrandButton'] = array(
   '#theme' => 'editBrandSearchResultsTable_theme',
   '#tree' => TRUE,
  );

  // Search result count
  $count = 0;
  while($item =db_fetch_object($result))  {
    $submitKey=0;
    $key= $item->BrandID;  

    $form['tableBrandButton'][$key]['BrandID'] = array( '#value' => $item->BrandID);
    $form['tableBrandButton'][$key]['Name'] = array( '#value' => $item->Name);
    $form['tableBrandButton'][$key]['Edit'] = array (
      '#type' => 'submit',
      '#value' => t('Edit'),
      '#name' => $item->BrandID,
      '#submit' => array('sensormodule_editBrandSearchResultsTable_edit'),
      '#attributes' => array('class' => 'sensormodule-table-btn sensormodule-btn'),
    );
    $form['tableBrandButton'][$key]['Delete'] = array (
        '#type' => 'submit',
        '#value' => t('Delete'),
        '#name' => $item->BrandID,
        '#submit' => array('sensormodule_editBrandSearchResultsTable_delete'),
        '#attributes' => array('class' => 'sensormodule-delete-btn sensormodule-table-btn'),
    );
    $count++;
  }
  db_set_active('default');
  $form['tableBrandButton']['pager'] = array('#value' => theme('pager', NULL, $limit, 0));

  // Display message if none found
  if($count == 0) {
    drupal_set_message("No brands like: '".$searchTerm."' were found.");
    drupal_goto("sensormodule/edit/brand");
  }
  
  return $form;
}

//***************************************************************
//  NAME: theme_editBrandSearchResultsTable_theme
//  USE: Themes the Edit Brand table
//  DATE: 16/10/2020
//***************************************************************
function theme_editBrandSearchResultsTable_theme($form) {
  $header = array(
  array('data' => t('BrandID'), 'field' => 'BrandID', 'class' => 'sensormodule-sensor-table-header'),
  array('data' => t('Name'), 'field' => 'Name', 'class' => 'sensormodule-sensor-table-header'),
  array('Edit' => t('Edit'), 'class' => 'sensormodule-sensor-table-header'),
  array('Delete' => t('Delete'), 'class' => 'sensormodule-sensor-table-header'),
  theme('table_select_header_cell'),
  );
  $rows = array();
  foreach (element_children($form)as $index) {
    $row = array();
    $row[] = drupal_render($form[$index]['BrandID']);
    $row[] = drupal_render($form[$index]['Name']);
    $row[] = drupal_render($form[$index]['Edit']);
    $row[] = drupal_render($form[$index]['Delete']);
    $rows[] = array('data' => $row, 'class' => 'sensormodule-sensor-table-row'); 
  }
  $output = theme('table', $header, $rows);
  if($form['pager']['#value']) {
    $output .= drupal_render($form['pager']);
  }

  $output .= drupal_render($form);
  $output = str_replace("<th class=\"select-all\"></th>", '', $output);
  return $output;
  
}


//***************************************************************
//  NAME: sensormodule_editBrandSearchResultsTable_delete
//  USE: Button to send user to Brand delete confirmation page
//  DATE: 21/10/2020
//***************************************************************
function sensormodule_editBrandSearchResultsTable_delete($form, &$form_state) {
  $brandID = $form_state['clicked_button']['#name'];
  drupal_goto("sensormodule/edit/brand/remove/", "brandID=".$brandID);
}

//***************************************************************
//  NAME: sensormodule_editBrandSearchResultsTable_edit
//  USE: Button to send user to a Brand edit page
//  DATE: 21/10/2020
//***************************************************************
function sensormodule_editBrandSearchResultsTable_edit($form, &$form_state) {
  $brandID = $form_state['clicked_button']['#name'];
  drupal_goto("sensormodule/edit/brand/modify/", "brandID=".$brandID);
}

//==============================================================
// REMOVE
//==============================================================
//***************************************************************
//  NAME: sensormodule_removeBrandConfirmation
//  USE: The confirmation page for a Brands deletion
//  DATE: 21/10/2020
//***************************************************************
function sensormodule_removeBrandConfirmation(&$form_state, $brandID="") {
  $form['deleteBrand'] = array(
   '#type' => 'fieldset',
   '#title' => t('Are you sure?'),
   '#tree' => TRUE,
   '#attributes' => array('class' => 'delete-form-fieldset'),
   );

   $form['deleteBrand']['delete'] = array(
   '#type' => 'submit',
   '#value' => t('Delete'),
   '#name' => 'Delete',
   '#attributes' => array(
    'onclick' => 'if(!confirm("Are you really sure?")) {return false;}',
    'class' => 'sensormodule-delete-btn sensormodule-btn'),
   );

   $form['deleteBrand']['cancel'] = array(
   '#type' => 'submit',
   '#value' => t('Cancel'),
   '#name' => 'Cancel',
   '#attributes' => array('class' => 'sensor-search-btn sensormodule-btn')
   );

   $form['deleteBrand']['brandID'] = array(
     '#type' => 'hidden',
     '#value' => $brandID,
   );
   return $form;
}

//***************************************************************
//  NAME: sensormodule_removeBrandConfirmation_validate
//  USE: Validates whether the brand deletes form should be submitted
//  DATE: 21/10/2020
//***************************************************************
function sensormodule_removeBrandConfirmation_validate($form, &$form_state) {
  $clicked_button = $form_state['clicked_button']['#name'];
  $brandID = $form_state['values']['deleteBrand']['brandID'];
  if($clicked_button == "Cancel") {
    drupal_set_message("No changes made.");
    drupal_goto("sensormodule/edit/brand");
  }
  else if(checkNull($brandID)) {
    drupal_set_message("No Brand ID supplied, no changes made.");
    drupal_goto("sensormodule/edit/brand");
  }
}

//***************************************************************
//  NAME: sensormodule_removeBrandConfirmation_submit
//  USE: Submit function to delete the given Brand from db
//  DATE: 21/10/2020
//***************************************************************
function sensormodule_removeBrandConfirmation_submit($form, &$form_state) {
  $brandID = $form_state['values']['deleteBrand']['brandID'];
  
  db_set_active('gas_sensor_db');
  db_query("DELETE FROM Brand WHERE BrandID='%s';",$brandID);
  db_set_active('default');

  drupal_set_message("Sucessfully removed brand from the database.");
  drupal_goto("sensormodule/edit/brand"); 
}

?>