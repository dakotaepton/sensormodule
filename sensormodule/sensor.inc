<?php
//==============================================================
// SEARCH
//==============================================================
//**************************************************************
//  NAME: sensormodule_sensorSearchBar
//  USE: Produces HTML for the Search Sensor Form
//  DATE: 07/09/2020
//**************************************************************
function sensormodule_sensorSearchBar(&$form_state) {
  $form['search'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sensor Search'),
    '#tree' => TRUE,
    '#attributes' => array('class' => 'sensor-search-fieldset'),
  );

  $form['search']['searchField'] = array(
    '#type' => 'textfield',
    '#title' => t('Search'),
    '#size' => 60,
    '#maxlength' => 64,
    '#description' => t('Enter the desired search term.'),
    '#attributes' => array('class' => 'sensor-search-bar'),
  );

  $form['search']['filter'] = array(
    '#type' => 'select',
    '#title' => 'Filter',
    '#default_value' => 'partnum',
    '#options' => array(
      'partnum' => t('Part Number'),
      'alias' => t('Alias'),
      'gas' => t('Gas'),
      'brand' => t('Brand'),
      'range' => t('Reading Range'),
      'bias' => t('Bias'),
    ),
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

//**************************************************************
//  NAME: sensormodule_sensorSearchBar_submit
//  USE: Submit function for searchSensorForm, redirects to result page
//  DATE: 07/09/2020
//**************************************************************
function sensormodule_sensorSearchBar_submit($form, &$form_state) {
  $searchTerm = $form_state['values']['search']['searchField'];
  $filter = $form_state['values']['search']['filter'];
  drupal_goto("sensormodule/search/", "searchTerm=".$searchTerm."&filter=".$filter);
}

//**************************************************************
//  NAME: sensormodule_sensorSearchBar_validate
//  USE: Validation function for searchSensorForm
//  DATE: 07/09/2020
//**************************************************************
function sensormodule_sensorSearchBar_validate($form, &$form_state) {
  
  // Get clicked button, search term and search filter
  $filter = $form_state['values']['search']['filter'];
  $clicked_button = $form_state['clicked_button']['#name'];
  $searchTerm = $form_state['values']['search']['searchField'];
  if( $clicked_button == 'Search') {
    if($searchTerm == "")  {
      form_error($form['search']['searchField'], 'Please enter a valid search term.'); 
    }
    else if($filter == 'bias' || $filter == 'range') {
      if(!checkDouble($searchTerm) && !checkInt($searchTerm)) {
        form_error($form['search']['searchField'], 'Please enter a valid number for search.'); 
      }
    }
    else if(!checkAlphanumeric($searchTerm)){
      form_error($form['search']['searchField'], 'Please enter only safe characters.'); 
    }
  }
  else if($clicked_button == 'ShowAll'){
    drupal_goto("sensormodule/search");
  }
}
  
//**************************************************************
//  NAME: sensorSearchResultsTable
//  USE: Form layout for the searchSensorForm, which is standard sensor search (now includes interactible table)
//  DATE: 30/10/2020
//  COMMENT: This is for the Search table despite the buttons being listed as Edit, something is wrong with
//  the themeing and despite me trying to change both the theme hook and the theme submodule, neither work 
//  unless the button is type Edit.
//**************************************************************
function sensormodule_sensorSearchResultsTable($form_state, $searchTerm, $filter) {
  
  // Escape percentage sign so select query works
  $searchTerm = str_replace('%', "\%", $searchTerm);

  switch($filter) {
    case 'partnum':
      $query = "SELECT DISTINCT SensorID, BrandID, Alias, PartNumber,
                CONCAT(MinTemperature, ' to ', MaxTemperature) AS TempRange,
                CONCAT(MinPressure, ' to ', MaxPressure) AS PressureRange,
                CONCAT(MinHumidity, ' to ', MaxHumidity) AS HumidityRange, Bias,
                Name AS Gases
                FROM Sensor NATURAL JOIN Detects NATURAL JOIN Gas
                WHERE PartNumber LIKE '%".$searchTerm."%'";
      break;
    case 'alias':
      $query = "SELECT DISTINCT SensorID, BrandID, Alias, PartNumber,
                CONCAT(MinTemperature, ' to ', MaxTemperature) AS TempRange,
                CONCAT(MinPressure, ' to ', MaxPressure) AS PressureRange,
                CONCAT(MinHumidity, ' to ', MaxHumidity) AS HumidityRange, Bias,
                Name as Gases
                FROM Sensor NATURAL JOIN Detects NATURAL JOIN Gas
                WHERE Alias LIKE '%".$searchTerm."%'";
      break;
    case 'gas':
      $query = "SELECT DISTINCT SensorID, BrandID, Alias, PartNumber,
                CONCAT(MinTemperature, ' to ', MaxTemperature) AS TempRange,
                CONCAT(MinPressure, ' to ', MaxPressure) AS PressureRange,
                CONCAT(MinHumidity, ' to ', MaxHumidity) AS HumidityRange, Bias,
		            Name as Gases
                FROM Sensor sensor NATURAL JOIN Detects detects NATURAL JOIN Gas gas
                WHERE gas.Name='".$searchTerm."'";
      break;
    case 'brand':
      $query = "SELECT DISTINCT SensorID, BrandID, Alias, PartNumber,
                CONCAT(MinTemperature, ' to ', MaxTemperature) AS TempRange,
                CONCAT(MinPressure, ' to ', MaxPressure) AS PressureRange,
                CONCAT(MinHumidity, ' to ', MaxHumidity) AS HumidityRange, Bias,
                gas.Name AS Gases
                FROM Brand brand NATURAL JOIN Sensor sensor NATURAL JOIN Detects detects INNER JOIN Gas gas ON gas.GasID = detects.GasID
                WHERE brand.Name LIKE '%".$searchTerm."%'";
      break;
    case 'range':
      $query = "SELECT DISTINCT SensorID, BrandID, Alias, PartNumber,
                CONCAT(MinTemperature, ' to ', MaxTemperature) AS TempRange,
                CONCAT(MinPressure, ' to ', MaxPressure) AS PressureRange,
                CONCAT(MinHumidity, ' to ', MaxHumidity) AS HumidityRange, Bias,
                Name as Gases
                FROM Sensor sensor NATURAL JOIN Detects detects NATURAL JOIN Gas gas
                WHERE ".$searchTerm." >= MinReading AND ".$searchTerm." <= MaxReading";
      break;
    case 'bias':
      $query = "SELECT DISTINCT SensorID, BrandID, Alias, PartNumber,
                CONCAT(MinTemperature, ' to ', MaxTemperature) AS TempRange,
                CONCAT(MinPressure, ' to ', MaxPressure) AS PressureRange,
                CONCAT(MinHumidity, ' to ', MaxHumidity) AS HumidityRange, Bias,
                Name as Gases
                FROM Sensor sensor NATURAL JOIN Detects detects NATURAL JOIN Gas
                WHERE Bias='".$searchTerm."'";
      break;
    case 'brand-strict':
      $query = "SELECT DISTINCT SensorID, BrandID, Alias, PartNumber,
                CONCAT(MinTemperature, ' to ', MaxTemperature) AS TempRange,
                CONCAT(MinPressure, ' to ', MaxPressure) AS PressureRange,
                CONCAT(MinHumidity, ' to ', MaxHumidity) AS HumidityRange, Bias,
                gas.Name as Gas
                FROM Brand brand NATURAL JOIN Sensor sensor NATURAL JOIN Detects detects INNER JOIN Gas gas ON gas.GasID = detects.GasID
                WHERE brand.Name='".$searchTerm."'";
      break;
  }
  $limit = 25;
  db_set_active('gas_sensor_db');
  $header = array(
    array('data' => t('Alias'), 'field' => 'Alias'),
    array('data' => t('PartNumber'), 'field' => 'PartNumber'),
    array('data' => t('Brand'),'field' => 'BrandID', 'sort' => 'asc'),
    array('data' => t('TempRange'), 'field' => 'TemperatureRange'),
    array('data' => t('PressureRange'), 'field' => 'PressureRange'),
    array('data' => t('HumidityRange'), 'field' => 'HumidityRange'),
    array('data' => t('Bias'), 'field' => 'Bias'),
    array('data' => t('Gases'), 'field' => 'Name')
  );

  $tablesort = tablesort_sql($header);
  if($searchTerm!="") {
    $result = pager_query($query.$tablesort, $limit, 0, NULL, $searchTerm);
  } else {
    $query = "SELECT Sensor.SensorID, Sensor.BrandID, Sensor.Alias, Sensor.PartNumber,
        CONCAT(Sensor.MinTemperature,' to ',Sensor.MaxTemperature) AS TempRange, 
        CONCAT(Sensor.MinPressure,' to ',Sensor.MaxPressure) AS PressureRange, 
        CONCAT(Sensor.MinHumidity,' to ',Sensor.MaxHumidity) AS HumidityRange,Bias,
        Gas.Name AS Gases
        FROM Sensor NATURAL JOIN Detects  NATURAL JOIN Gas";
    $result = pager_query($query.$tablesort, $limit);
  }
 
  $form['tableButton'] = array(
    //Changing the theme here wasn't working for me either (with individual theme hooks as well as shared but with different themes)
    '#theme' => 'sensorSearchResultsTable_theme',
    '#tree' => TRUE,);

  // Search result count
  $count = 0;
  while($item =db_fetch_object($result))
  {
    $key= $item->SensorID;
    $brand_query = db_query("SELECT Name FROM Brand WHERE BrandID='%s'",$item->BrandID);
    $brand = db_fetch_array($brand_query);
    $brandName = $brand['Name'];

    $gas_query = db_query("SELECT GROUP_CONCAT(Gas.Name) AS Name FROM Gas INNER JOIN Detects ON Gas.GasID=Detects.GasID WHERE Detects.SensorID='%s'",$key);
    $gas = db_fetch_array($gas_query);
    $gasName = $gas['Name'];
    

    $form['tableButton'][$key]['Alias'] = array('#value' =>$item->Alias != "" ? $item->Alias : "N.D.");
    $form['tableButton'][$key]['PartNumber'] = array('#value' => $item->PartNumber != "" ? $item->PartNumber : "N.D.");
    $form['tableButton'][$key]['Brand'] = array('#value' => $brandName != "" ? $brandName : "N.D.");
    $form['tableButton'][$key]['TemperatureRange'] = array('#value' => $item->TempRange != "" ? $item->TempRange : "N.D.");
    $form['tableButton'][$key]['PressureRange'] = array('#value' => $item->PressureRange != "" ? $item->PressureRange : "N.D.");
    $form['tableButton'][$key]['HumidityRange'] = array('#value' => $item->HumidityRange != "" ? $item->HumidityRange : "N.D.");
    $form['tableButton'][$key]['Bias'] = array('#value' => $item->Bias != "" ? $item->Bias : "N.D.");
    $form['tableButton'][$key]['Gases'] = array('#value' => $gasName != "" ? $gasName : "N.D.");
    $form['tableButton'][$key]['Details'] = array (
      '#type' => 'submit', 
      '#value' => t('Details'), 
      '#name' => $item->SensorID,
      '#attributes' => array('class' => 'sensormodule-sensor-table-btn sensormodule-btn'),
    );
    $count++;
  }
  db_set_active('default');
  $form['tableButton']['pager']['#value'] = theme('pager', NULL, $limit, 0);
  
  // Turn percentage sign back to normal
  $searchTerm = str_replace('\%', "%", $searchTerm);

  // Display message if none found
  if($count == 0) {
    drupal_set_message("No sensors like: '".$searchTerm."' were found.", "error");
    drupal_goto("sensormodule/search/sensor");
  }
  return $form;
}


//**************************************************************
//  NAME: theme_searchSensorTableForm_theme
//  USE: Theme function for the table form above
//  DATE: 30/10/2020
//**************************************************************
function theme_sensorSearchResultsTable_theme($form) {
  $header = array(
    array('data' => t('Alias'),'field' => 'Alias', 'class' => 'sensormodule-sensor-table-header'),
    array('data' => t('Part Number'), 'field' => 'PartNumber' , 'class' => 'sensormodule-sensor-table-header'),
    array('data' => t('Brand'), 'field'=> 'Brand', 'class' => 'sensormodule-sensor-table-header'),
    array('data' => t('Temperature Range'), 'field' => 'TemperatureRange', 'class' => 'sensormodule-sensor-table-header'),
    array('data' => t('Pressure Range'), 'field' => 'PressureRange', 'class' => 'sensormodule-sensor-table-header'),
    array('data' => t('Humidity Range'), 'field' => 'HumidityRange', 'class' => 'sensormodule-sensor-table-header'),
    array('data' => t('Bias'), 'field' => 'Bias', 'class' => 'sensormodule-sensor-table-header'),
    array('data' => t('Gases'), 'field' => 'Gases', 'class' => 'sensormodule-sensor-table-header'),
    array('Details' => t('Details'), 'class' => 'sensormodule-sensor-table-header'),
    theme('table_select_header_cell'),
  );
  $rows = array();
  foreach (element_children($form)as $index) {
    $row = array();
    $row[] = drupal_render($form[$index]['Alias']);
    $row[] = drupal_render($form[$index]['PartNumber']);
    $row[] = drupal_render($form[$index]['Brand']);
    $row[] = drupal_render($form[$index]['TemperatureRange']);
    $row[] = drupal_render($form[$index]['PressureRange']);
    $row[] = drupal_render($form[$index]['HumidityRange']);
    $row[] = drupal_render($form[$index]['Bias']);
    $row[] = drupal_render($form[$index]['Gases']);
    $row[] = drupal_render($form[$index]['Details']);
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
  
//*************************************************************
//  NAME: sensormodule_editSensorBtn_submit
//  USE: Redirects to load the edit page with the list of sensor details
//  DATE: 27/09/2020
//*************************************************************
function sensormodule_sensorSearchResultsTable_submit($form, &$form_state)
{
  $sensorID = $form_state['clicked_button']['#name'];
  drupal_goto("sensormodule/search/sensor/details/", "sensorID=".$sensorID);
}

//**************************************************************
//  NAME: sensormodule_sensorDetectsListTable
//  USE: Produces HTML for the Detects  Table for sensor
//  DATE: 16/09/2020
//**************************************************************
function sensormodule_sensorDetectsListTable(&$form_state, $searchTerm) {

  $selectedSensor = $searchTerm;
  $limit = 25;
  db_set_active('gas_sensor_db');
  $header = array(
    array('data' => t('Gas ID'), 'field' => 'GasID'),
    array('data' => t('Alias'), 'field' => 'Alias'),
    array('data' => t('Name'), 'field' =>'Name'),
    array('data' => t('Edit'), 'field' => 'Edit'),
    array('data' => t('Delete'), 'field' => 'Delete'),
  );

  $tablesort = tablesort_sql($header);
  if($selectedSensor!="") {
    $query = "SELECT SensorID,GasID FROM Detects WHERE SensorID=".$selectedSensor;
    
  }
  $result = pager_query($query.$tablesort, $limit);
  db_set_active('default');
  
  
  db_set_active('gas_sensor_db');			
  $form['detectTable']= array(
    '#theme' => 'sensorDetectsListTable_theme',
    '#tree' => TRUE,);

    while( $item =db_fetch_object($result)){	
    $gas_query = db_query("SELECT Name FROM Gas WHERE GasID = '%s';",$item->GasID);
    $gas = db_fetch_array($gas_query);
    $alias_query = db_query("SELECT Alias FROM Sensor WHERE SensorID='%s';", $item->SensorID);
    $alias = db_fetch_array($alias_query)['Alias'];
    $gasName = $gas['Name'];
    $key= $item->GasID;  
    
    $form['detectTable'][$key]['Sensor Alias'] = array( '#value' =>$alias);
    $form['detectTable'][$key]['GasID'] = array( '#value' => $item->GasID);
    $form['detectTable'][$key]['Name'] = array( '#value' => $gasName);
    $form['detectTable'][$key]['Edit'] = array (
      '#type' => 'submit', 
      '#value' => t('Edit'), 
      '#name' => $item->GasID, 
      '#submit' => array('sensormodule_sensorDetectsListTable_edit'),
      '#attributes' => array('class' => 'sensormodule-table-btn sensormodule-btn')
    );
    $form['detectTable'][$key]['Delete'] = array (
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#name' => $item->GasID,
      '#submit' => array('sensormodule_sensorDetectsListTable_delete'),
      '#attributes' => array('class' => 'sensormodule-delete-btn sensormodule-table-btn'),
    );
  }
  db_set_active('default');
  
  $form['detectTable']['pager'] = array('#value' => theme('pager', NULL, $limit, 0));
  $form['AddDetect'] = array(
    '#type' => 'button',
    '#value' => t('Add Detects'),
    '#executes_submit_callback' => TRUE,
    '#submit' => array('searchDetectsTableForm_addDetects'),
    '#attributes' => array('class' => 'sensormodule-add-btn sensormodule-btn')
  );
  $form['sensorID'] = array(
    '#type' => 'hidden',
    '#value' => $selectedSensor,
  );

  return $form;
}

//**************************************************************
//  NAME: theme_searchDetectsTableForm_theme
//  USE: Formats the table with buttons for the sensors list of detects table
//  DATE: 16/09/2020
//**************************************************************
function theme_sensorDetectsListTable_theme($form) {
  $header = array(
  array('data' => t('Sensor Alias'), 'field' => 'Sensor Alias'),
  array('data' => t('GasID'), 'field' => 'GasID'),
  array('data' => t('Name'),'field' => 'Name'),
  array('data' => t('Edit'), 'field' => 'Edit'),
  array('data' => t('Delete'), 'field' => 'Delete'),
  theme('table_select_header_cell'),
  );
  $rows = array();
    foreach (element_children($form)as $index) {
    $row = array();
    $row[] = drupal_render($form[$index]['Sensor Alias']);
    $row[] = drupal_render($form[$index]['GasID']);
    $row[] = drupal_render($form[$index]['Name']);
    $row[] = drupal_render($form[$index]['Edit']);
    $row[] = drupal_render($form[$index]['Delete']);
    $rows[] = $row; 
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
//  NAME: searchDetectsTableForm_addDetects
//  USE: Redirects to add detects page with sensorID
//  DATE: 16/09/2020
//**************************************************************
function searchDetectsTableForm_addDetects($form, &$form_state) {
  $sensorID = $form_state['values']['sensorID'];
  drupal_goto('sensormodule/edit/detects/add/', 'sensorID='.$sensorID);
}

//***************************************************************
// NAME: sensormodule_editDetectsPageForm_edit
// USE: Button to send the user to the edit Detects page
// DATE: 12/10/2020
//***************************************************************
function sensormodule_sensorDetectsListTable_edit($form, &$form_state) {

  $gasID = $form_state['clicked_button']['#name'];
  $sensorID = $form_state['values']['sensorID'];

  $params = "sensorID=".$sensorID."&gasID=".$gasID;
  drupal_goto("sensormodule/edit/detects/modify/", $params);
}

//***************************************************************
// NAME: sensormodule_editDetectsPageForm_delete
// USE: Button to send user to delete confirmation page
// DATE: 15/10/2020
//***************************************************************
function sensormodule_sensorDetectsListTable_delete($form, &$form_state) {

  $gasID = $form_state['clicked_button']['#name'];
  $sensorID = $form_state['values']['sensorID'];

  $params = "sensorID=".$sensorID."&gasID=".$gasID;
  drupal_goto("sensormodule/edit/detects/remove/", $params);
}


//==============================================================
// ADD
//==============================================================
//**************************************************************
//  NAME: sensormodule_addSensorForm
//  USE: Produces HTML for the Add Sensor Form
//  DATE: 27/09/2020
//**************************************************************
function sensormodule_addSensorForm(&$form_state) {

  // Get list of available brands for dropdown select
  db_set_active("gas_sensor_db");
  // Get all brand ID's and Names
  $brandQuery = db_query("SELECT BrandID, Name FROM Brand;");
  $brands = array();
  // Create associative array of BrandID->Name for dropdown
  while($brand = db_fetch_object($brandQuery))
  {
    $brands[$brand->BrandID] = t($brand->Name);
  }
  db_set_active("default");

  $activeOptions = array('1' => t('True'), '0' => t('False'), 'NULL' => t('N.D.'));


  $form['addSensor'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add New Sensor'),
    '#tree' => TRUE,
    '#attributes' => array('class' => 'form-fieldset'),
  );

  $form['addSensor']['GasCount'] = array(
    '#type' => 'hidden',
    '#value' => '0',
  );
  
  $form['addSensor']['Alias'] = array(
    '#type' => 'textfield',
    '#title' => t('Alias'),
    '#maxlength' => 64,
  );

  $form['addSensor']['PartNumber'] = array(
    '#type' => 'textfield',
    '#title' => t('Part Number'),
    '#maxlength' => 64,
    '#required' => TRUE,
  );

  $form['addSensor']['BrandID'] = array(
    '#type' => 'select',
    '#title' => t('Brand'),
    '#default_value' => t('SELECT A BRAND'),
    '#options' => $brands,
  );

  $form['addSensor']['Technology'] = array(
    '#type' => 'textfield',
    '#title' => t('Technology'),
    '#default_value' => 'EC',
    '#maxlength' => 64,
  );

  $form['addSensor']['ActivePart'] = array(
    '#type' => 'select',
    '#title' => t('Active Part'),
    '#options' => $activeOptions,
    '#maxlength' => 64,
  );

  $form['addSensor']['LastReview'] = array(
    '#type' => 'date',
    '#title' => t('Last Review'),
  );

  $form['addSensor']['DatasheetReview'] = array(
    '#type' => 'textfield',
    '#title' => t('DatasheetReview'),
    '#maxlength' => 64,
  );

  $form['addSensor']['Series'] = array(
    '#type' => 'textfield',
    '#title' => t('Series'),
    '#default_value' => '4-series',
    '#maxlength' => 64,
  );

  $form['addSensor']['Diameter'] = array(
    '#type' => 'textfield',
    '#title' => t('Diameter'),
    '#maxlength' => 64,
  );

  $form['addSensor']['Height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#maxlength' => 64,
  );

  $form['addSensor']['PinCount'] = array(
    '#type' => 'textfield',
    '#title' => t('Pin Count'),
    '#maxlength' => 64,
  );

  $form['addSensor']['PinLength'] = array(
    '#type' => 'textfield',
    '#title' => t('Pin Length'),
    '#maxlength' => 64,
  );

  $form['addSensor']['MaxWeight'] = array(
    '#type' => 'textfield',
    '#title' => t('Max Weight'),
    '#maxlength' => 64,
  );

  $form['addSensor']['MinTemperature'] = array(
    '#type' => 'textfield',
    '#title' => t('MinTemperature'),
    '#maxlength' => 64,
  );

  $form['addSensor']['MaxTemperature'] = array(
    '#type' => 'textfield',
    '#title' => t('Max Temp'),
    '#maxlength' => 64,
  );

  $form['addSensor']['MinPressure'] = array(
    '#type' => 'textfield',
    '#title' => t('Min Pressure'),
    '#maxlength' => 64,
  );

  $form['addSensor']['MaxPressure'] = array(
    '#type' => 'textfield',
    '#title' => t('Max Pressure'),
    '#maxlength' => 64,
  );


  $form['addSensor']['MinHumidity'] = array(
    '#type' => 'textfield',
    '#title' => t('Min Humidity'),
    '#maxlength' => 64,
  );

  $form['addSensor']['MaxHumidity'] = array(
    '#type' => 'textfield',
    '#title' => t('Max Humidity'),
    '#maxlength' => 64,
  );


  $form['addSensor']['Bias'] = array(
    '#type' => 'textfield',
    '#title' => t('Bias'),
    '#maxlength' => 64,
  );

  $form['addSensor']['MinLoad'] = array(
    '#type' => 'textfield',
    '#title' => t('Min Load'),
    '#maxlength' => 64,
  );

  $form['addSensor']['MaxLoad'] = array(
    '#type' => 'textfield',
    '#title' => t('Max Load'),
    '#maxlength' => 64,
  );

  $form['addSensor']['MaxSignalDrift'] = array(
    '#type' => 'textfield',
    '#title' => t('Max Signal Drift'),
    '#maxlength' => 64,
  );

  $form['addSensor']['SignalDriftInterval'] = array(
    '#type' => 'textfield',
    '#title' => t('Signal Drift Interval'),
    '#maxlength' => 64,
  );

  $form['addSensor']['ExpectedLife'] = array(
    '#type' => 'textfield',
    '#title' => t('Expected Life'),
    '#maxlength' => 64,
  );

  $form['addSensor']['Warranty'] = array(
    '#type' => 'textfield',
    '#title' => t('Warranty'),
    '#maxlength' => 64,
  );

  $form['addSensor']['finishBtn'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
    '#name' => 'Add',
    '#attributes' => array('class' => 'sensor-search-btn sensormodule-btn')
  );

  $form['addSensor']['cancelBtn'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#name' => 'Cancel',
    '#attributes' => array('class' => 'sensor-search-btn sensormodule-btn', 
    'onclick' => 'if(!confirm("Are you sure you want to cancel?")) {return false;}'),
  );

  return $form;
}

//**************************************************************
//  NAME: sensormodule_addSensorForm_validate
//  USE: Validates all add sensor form inputs before submission
//  DATE: 27/09/2020
//**************************************************************
function sensormodule_addSensorForm_validate($form, &$form_state) {
  $lastReviewDate = formatFormDate($form_state['values']['addSensor']['LastReview']);
  $form_state['values']['addSensor']['LastReview'] = $lastReviewDate;
  $clicked_button = $form_state['clicked_button']['#name'];
  if($clicked_button == 'Add') {
    sensorDetailValidation($form['addSensor'], $form_state['values']['addSensor']);
  }
  else if($clicked_button == 'Cancel'){
    drupal_set_message("Cancelled Sensor insertion.");
    drupal_goto('sensormodule/edit/sensor');
  }
}

//**************************************************************
//  NAME: sensormodule_addSensorForm_submit
//  USE: Converts all inputted data to correct types and creates
//       new sensor in db.
//  DATE: 27/09/2020
//**************************************************************
function sensormodule_addSensorForm_submit($form, &$form_state) {

  // As Part Number is a required field, if no alias is given make the Part Number the alias
  if(checkNull($form_state['values']['addSensor']['Alias'])) {
    $form_state['values']['addSensor']['Alias'] = $form_state['values']['addSensor']['PartNumber'];
  }

  // Create insert query from data
  $insert_query = createSensorInsertQuery($form_state['values']['addSensor']);

  db_set_active("gas_sensor_db");
  // Insert in DB
  db_query($insert_query);
  // Get LAST_INSERT_ID();
  $newIDQuery = db_query("SELECT LAST_INSERT_ID()");
  $id = db_result($newIDQuery);
  db_set_active("default");
  
  //INCASE ALIAS IS NULL GIVE PART NUMBER
  if($alias == "") {
    $alias = $partNum;
  }
  
  if($id != 0) {
    drupal_set_message('Sensor succesfully added! Add gas detection info in the table below.');
    drupal_goto('sensormodule/edit/sensor/modify/', "sensorID=".$id);
  }
  else {
    drupal_set_message('Something went wrong and the new sensor was not inserted into the database.', 'error');
    drupal_goto('sensormodule/edit/sensor');
  }
}

//***************************************************************
// NAME: sensormodule_addDetectForm
// USE: Contains all the Detect fields for a user to edit
// DATE: 15/10/2020
//***************************************************************
function sensormodule_addDetectForm(&$form_state, $sensorID="") {
  
  //GET LIST OF AVAILABLE GASSES FOR DROP DOWN SELECT
  db_set_active("gas_sensor_db");

  $aliasQuery = db_query("SELECT Alias FROM Sensor WHERE SensorID=%s", $sensorID);
  $alias = db_fetch_object($aliasQuery); 

  //GET ALL GASSES
  $gasQuery = db_query("SELECT GasID, Name FROM Gas;");
  //GET ALREADY USED GASSES FOR THIS SENSOR
  $usedGassesQuery = db_query("SELECT GasID FROM Detects WHERE SensorID=%d", intval($sensorID));
  $usedGasses = array();
  while($used = db_fetch_object($usedGassesQuery)) {
    $usedGasses[] = $used->GasID;
  }
  $gasses = array();
  while($gas =db_fetch_object($gasQuery))
  {
    //IF GAS NOT ALREADY USED
    if(!in_array($gas->GasID, $usedGasses)) {
      $gasses[$gas->GasID] = t($gas->Name);
    }
  }
  db_set_active("default");

  $measurementOptions['ppm'] = t('ppm');
  $measurementOptions['%Vol'] =t('%Vol');
  $measurementOptions['mg/m^3']=t('mg/m^3');

  $sensitivityOptions['nA/ppm'] = t('nA/ppm');
  $sensitivityOptions['nA/mg/m^3'] = t('nA/mg/m^3');
  $sensitivityOptions['mA/ppm'] = t('mA/ppm');
  $sensitivityOptions['µA/ppm'] = t('µA/ppm');
  $sensitivityOptions['µA'] = t('µA');


  $form['addDetect'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#title' => t('Add Gas Detection Info'),
    '#attributes' => array('class' => 'form-fieldset'),
  );
  $form['addDetect']['AliasField'] = array(
    '#type' => 'textfield',
    '#title' => t('Sensor Alias'),
    '#value' => $alias->Alias,
    '#maxlength' => 64,
    '#disabled' => TRUE,
  );
    $form['addDetect']['SensorID'] = array(
    '#type' => 'textfield',
    '#title' => t('Sensor ID'),
    '#value' => $sensorID,
    '#maxlength' => 64,
    '#disabled' => TRUE,
  );

  $form['addDetect']['GasName'] = array(
    '#type' => 'select',
    '#title' => t('Empirical Gas Name'),
    '#default_value' => t("SELECT A GAS"),
    '#options' => $gasses,
    '#required' => TRUE,
    );

  $form['addDetect']['MeasurementUnits'] = array(
    '#type' => 'select',
    '#title' => t('Measurement Units'),
    '#options' => $measurementOptions,
  );

  $form['addDetect']['MinReading'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum Reading'),
    '#maxlength' => 64,
  );

  $form['addDetect']['MaxReading'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum Reading'),
    '#maxlength' => 64,
  );

  $form['addDetect']['Resolution'] = array(
    '#type' => 'textfield',
    '#title' => t('Resolution'),
    '#maxlength' => 64,
  );

  $form['addDetect']['OvergasLimit'] = array(
    '#type' => 'textfield',
    '#title' => t('Overgas Limit'),
    '#maxlength' => 9,
  );

  $form['addDetect']['MinSensitivity'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum Sensitivity'),
    '#maxlength' => 4,
  );

  $form['addDetect']['MaxSensitivity'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum Sensitivity'),
    '#maxlength' => 4,
  );

  $form['addDetect']['SensitivityRatio'] = array(
    '#type' => 'select',
    '#title' => t('Sensitivity Ratio'),
    '#options' => $sensitivityOptions,
  );

  $form['addDetect']['SensitivityTestConcentration'] = array(
    '#type' => 'textfield',
    '#title' => t('Sensitivity Test Concentration'),
    '#maxlength' => 10,
  );

  $form['addDetect']['TTestType'] = array(
    '#type' => 'textfield',
    '#title' => t('TTest Type'),
    '#maxlength' => 4,
  );

  $form['addDetect']['TTestResponse'] = array(
    '#type' => 'textfield',
    '#title' => t('TTest Response'),
    '#maxlength' => 4,
  );

  $form['addDetect']['ZeroCurrent'] = array(
    '#type' => 'textfield',
    '#title' => t('Zero Current Value'),
    '#maxlength' => 25,
  );

  $form['addDetect']['ResponseStartLevel'] = array(
    '#type' => 'textfield',
    '#title' => t('Response Start Level'),
    '#maxlength' => 8,
  );

  $form['addDetect']['ResponseEndLevel'] = array(
    '#type' => 'textfield',
    '#title' => t('Response End Level'),
    '#maxlength' => 8,
  );

  $form['addDetect']['GasID'] = array(
    '#type' => 'hidden',
    '#value' => NULL,
  );

  $form['addDetect']['addBtn'] = array(
    '#type' => 'submit',
    '#name' => 'Add',
    '#value' => t('Add'),
    '#name' => 'Add',
    '#attributes' => array('class' => 'sensor-search-btn sensormodule-btn')
  );

  $form['addDetect']['cancelBtn'] = array(
    '#type' => 'submit',
    '#name' => 'Cancel',
    '#value' => t('Cancel'),
    '#attributes' => array('class' => 'sensor-search-btn sensormodule-btn')
  );
  return $form;
}

//***************************************************************
// NAME: sensormodule_addDetectForm_validate
// USE: Calls a validation function to check all entries
// DATE: 15/10/2020
//***************************************************************
function sensormodule_addDetectForm_validate($form, &$form_state) {
  $clicked_button = $form_state['clicked_button']['#name'];
  $sensorID = $form_state['values']['addDetect']['SensorID'];

  if( $clicked_button == "Cancel" ) {
    drupal_set_message("No changes made.");
    drupal_goto('sensormodule/edit/sensor/modify/', "sensorID=".$sensorID);
  }
  else if( $clicked_button == "Add" ) {
    $form_state['values']['addDetect']['GasID'] = $form_state['values']['addDetect']['GasName'];
    sensorDetectDetailValidation($form['addDetect'], $form_state['values']['addDetect']);
  }
}

//***************************************************************
// NAME: sensormodule_addDetectForm_submit
// USE: Adds the proposed detect entry to the Detects table
// DATE: 15/10/2020
//***************************************************************
function sensormodule_addDetectForm_submit($form, &$form_state) {

  $sensorID = intval($form_state['values']['addDetect']['SensorID']);
  $alias = $form_state['values']['addDetect']['Alias'];  
  $gasID = $form_state['values']['addDetect']['GasID'];
  $measurementUnits = $form_state['values']['addDetect']['MeasurementUnits'];
  $minReading = $form_state['values']['addDetect']['MinReading'];
  $maxReading = $form_state['values']['addDetect']['MaxReading'];
  $resolution = $form_state['values']['addDetect']['Resolution'];
  $overgasLimit = $form_state['values']['addDetect']['OvergasLimit'];
  $minSensitivity = $form_state['values']['addDetect']['MinSensitivity'];
  $maxSensitivity = $form_state['values']['addDetect']['MaxSensitivity'];
  $sensitivityRatio = $form_state['values']['addDetect']['SensitivityRatio'];
  $sensitivityTestConcent = $form_state['values']['addDetect']['SensitivityTestConcentration'];
  $ttestType = $form_state['values']['addDetect']['TTestType'];
  $ttestResponse = $form_state['values']['addDetect']['TTestResponse'];
  $responseStartLevel = $form_state['values']['addDetect']['ResponseStartLevel'];
  $responseEndLevel = $form_state['values']['addDetect']['ResponseEndLevel'];
  $zeroCurrent = $form_state['values']['addDetect']['ZeroCurrent'];
 
  $insert_query = createSensorDetectInsertQuery($form_state['values']['addDetect']);

  db_set_active("gas_sensor_db");

  db_query($insert_query, $sensorID, $gasID);
  
  db_query("UPDATE Sensor SET GasCount = GasCount+1 WHERE SensorID='%s';", $sensorID);
  db_set_active("default");

    
  drupal_set_message("Successfully added Gas!", 'status');
  drupal_goto('sensormodule/edit/sensor/modify/', "sensorID=".$sensorID);
}

//==============================================================
// EDIT
//==============================================================
//**************************************************************
//  NAME: sensormodule_editSensorSearchBar
//  USE: Produces HTML for the Search Sensor Form
//  DATE: 16/09/2020
//**************************************************************
function sensormodule_editSensorSearchBar(&$form_state) {

  $form['searchEdit'] = array(
    '#type' => 'fieldset',
    '#title' => t('Edit Sensor'),
    '#tree' => TRUE,
    '#attributes' => array('class' => 'sensor-search-fieldset'),
  );

  $form['searchEdit']['searchTerm'] = array(
    '#type' => 'textfield',
    '#title' => t('Search'),
    '#size' => 60,
    '#maxlength' => 64,
    '#description' => t('Enter the desired sensor alias, part number or ID.'),
    '#attributes' => array('class' => 'sensor-search-bar'),
  );

  $form['searchEdit']['editSearchBtn'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
    '#name' => 'Search',
    '#attributes' => array('class' => 'sensor-search-btn sensormodule-btn')
  );
  
  $form['searchEdit']['editShowAllBtn'] = array(
    '#type' => 'submit',
    '#value' => t('Show All'),
    '#name' => 'ShowAll',
    '#attributes' => array('class' => 'sensor-search-btn sensormodule-btn')
  );
  return $form;
}

//**************************************************************
//  NAME: searchEditSensorForm_submit
//  USE: Submit function for searchSensorForm, redirects to result page
//  DATE: 16/09/2020
//**************************************************************
function sensormodule_editSensorSearchBar_submit($form, &$form_state) {
  $searchTerm = $form_state['values']['searchEdit']['searchTerm'];
  drupal_goto("sensormodule/edit/sensor/", "search=$searchTerm");
}

//**************************************************************
//  NAME: searchEditSensorForm_validate
//  USE: Validation function for searchSensorForm
//  DATE: 16/09/2020
//**************************************************************
function sensormodule_editSensorSearchBar_validate($form, &$form_state) {
  $searchTerm = $form_state['values']['searchEdit']['searchTerm'];
  $clicked_button = $form_state['clicked_button']['#name'];
  if($clicked_button == 'Search') {
    if( $searchTerm == "")
    {
      form_error($form['searchEdit']['searchTerm'], 'Please enter a valid search term.');
    }
    else if( !checkAlphanumeric($searchTerm) ) {
      form_error($form['searchEdit']['searchTerm'], 'Please enter only safe characters.');
    }
  }
  else if($clicked_button == 'ShowAll') {
    drupal_goto("sensormodule/edit/sensor/", "search=$searchTerm");
  }
}

//**************************************************************
//  NAME: sensormodule_editSensorSearchResultsTable
//  USE: Produces HTML for the Edit Sensors Table
//  DATE: 16/09/2020
//**************************************************************
function sensormodule_editSensorSearchResultsTable($form_state, $searchTerm) {

  $limit = 25;
  db_set_active('gas_sensor_db');
  
  $header = array(
    array('data' => t('SensorID'), 'field' => 'SensorID'),
    array('data' => t('Brand'), 'field' => 'BrandID', 'sort' => 'asc'),
    array('data' => t('Alias'),'field' => 'Alias'),
    array('data' => t('Part Number'), 'field' => 'PartNumber'),
    array('data' => t('Edit'), 'field' => 'Edit'),
    array('data' => t('Delete'), 'field' => 'Delete')
  );

  $tablesort = tablesort_sql($header);
  if($searchTerm!="") {
    $query = "SELECT SensorID, BrandID, Alias, PartNumber FROM Sensor 
              WHERE (SensorID='".$searchTerm."' OR 
                     Alias LIKE '%".$searchTerm."%' OR
                     PartNumber LIKE '%".$searchTerm."%')";
    $result = pager_query($query.$tablesort, $limit, 0, NULL, $searchTerm);
  }
  else {
    $query = "SELECT SensorID, BrandID, Alias, PartNumber FROM Sensor";
    $result = pager_query($query.$tablesort, $limit);
  }

  $form['tableButton'] = array(
   '#theme' => 'editSensorSearchResultsTable_theme',
   '#tree' => TRUE,
  );
  // Search result count
  $count = 0;
  while($item =db_fetch_object($result))
  {
    $brand_query = db_query("SELECT Name FROM Brand WHERE BrandID='%s'", $item->BrandID);
    $brand = db_fetch_array($brand_query);
    $brandName = $brand['Name'];
    $key= $item->SensorID;  
    
    $form['tableButton'][$key]['SensorID'] = array( '#value' =>$item->SensorID);
    $form['tableButton'][$key]['Brand'] = array( '#value' => $brandName);
    $form['tableButton'][$key]['Alias'] = array('#value' => $item->Alias);
    $form['tableButton'][$key]['PartNumber'] = array('#value'=>$item->PartNumber);
    $form['tableButton'][$key]['Edit'] = array (
      '#type' => 'submit', 
      '#value' => t('Edit'), 
      '#name' => $item->SensorID,
      '#attributes' => array('class' => 'sensormodule-table-btn sensormodule-btn'),
      '#submit' => array('sensormodule_editSensorSearchResultsTable_edit')
    );
    $form['tableButton'][$key]['Delete'] = array (
        '#type' => 'submit',
        '#value' => t('Delete'),
        '#name' => $item->SensorID,
        '#attributes' => array('class' => 'sensormodule-delete-btn sensormodule-table-btn'),
        '#submit' => array('sensormodule_editSensorSearchResultsTable_delete'),
    );
    $count++;
  }

  db_set_active('default');
  $form['tableButton']['pager'] = array('#value' => theme('pager', NULL, $limit, 0));

  // Display message if none found
  if($count == 0) {
    drupal_set_message("No sensors like: '".$searchTerm."' were found.", "error");
    drupal_goto("sensormodule/edit/sensor");
  }
  return $form;
}

//***************************************************************
// NAME: theme_editSensorSearchResultsTable_theme
// USE: Themes the table for displaying the sensors
// DATE: 28/09/2020
//***************************************************************
function theme_editSensorSearchResultsTable_theme($form) {
  $header = array(
  array('data' => t('SensorID'), 'field' => 'SensorID', 'class' => 'sensormodule-sensor-table-header'),
  array('data' => t('Brand'), 'field' => 'Brand', 'sort' => 'asc', 'class' => 'sensormodule-sensor-table-header'),
  array('data' => t('Alias'),'field' => 'Alias', 'class' => 'sensormodule-sensor-table-header'),
  array('data' => t('Part Number'), 'field' => 'PartNumber', 'class' => 'sensormodule-sensor-table-header'),
  array('Edit' => t('Edit'), 'class' => 'sensormodule-sensor-table-header'),
	array('Delete' => t('Delete'), 'class' => 'sensormodule-sensor-table-header'),
  theme('table_select_header_cell'),
  );
  $rows = array();
  foreach (element_children($form)as $index) {
    $row = array();
    $row[] = drupal_render($form[$index]['SensorID']);
    $row[] = drupal_render($form[$index]['Brand']);
    $row[] = drupal_render($form[$index]['Alias']);
    $row[] = drupal_render($form[$index]['PartNumber']);
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
// NAME: sensormodule_editSensorSearchResultsTable_delete
// USE:  Custom submit function for Delete button redirects to remove page
// DATE: 28/9/2020
//***************************************************************
function sensormodule_editSensorSearchResultsTable_delete($form, &$form_state)
{
  $id = $form_state['clicked_button']['#name'];
  drupal_goto ('sensormodule/edit/sensor/remove/', "sensorID=".$id);
}

//***************************************************************
// NAME: sensormodule_editSensorSearchResultsTable_edit
// USE:  Custom submit function for Edit button redirects to modify page
// DATE: 28/9/2020
//***************************************************************
function sensormodule_editSensorSearchResultsTable_edit($form, &$form_state)
{
  $id = $form_state['clicked_button']['#name'];
  drupal_goto ('sensormodule/edit/sensor/modify/', "sensorID=".$id);
}


//**************************************************************
//  NAME: sensormodule_modifySensorForm
//  USE: Populates form with sensor data to display full sensor info for editing
//  DATE: 27/09/2020
//**************************************************************
function sensormodule_modifySensorForm(&$form_state, $SensorID) {
  
  $sensorObj = getSensorInfo($SensorID);

  // Get list of available brands for dropdown select
  db_set_active("gas_sensor_db");
  // Get all brand ID's and Names
  $brandQuery = db_query("SELECT BrandID, Name FROM Brand;");
  $brands = array();
  // Create associative array of BrandID->Name for dropdown
  while($brand = db_fetch_object($brandQuery))
  {
    $brands[$brand->BrandID] = t($brand->Name);
  }

  $array = "";
  // Check if date string needs 0 padding to fit YYYY-MM-DD
  if($sensorObj->LastReview != NULL) {
    $date = explode("-", $sensorObj->LastReview);
    if(strlen($date[1]) == 2)  {
      $date[1] = ltrim($date[1], "0");
    }
    if(strlen($date[2]) == 2) {
      $date[2] = ltrim($date[2], "0");
    }
    $array  = array('year' => $date[0], 'month' => $date[1], 'day' => $date[2]);
  }
  
  db_set_active("default");

  $activeOptions = array('1' => t('True'), '0' => t('False'), 'NULL' => t('N.D.'));
  

  $form['editSensor'] = array(
    '#type' => 'fieldset',
    '#attributes' => array('class' => 'form-fieldset'),
    '#tree' => TRUE,
  );
 
  $form['editSensor']['SensorID'] = array(
  '#type' => 'textfield',
  '#title' => t('SensorID'),
  '#value' => $SensorID,
  '#disabled' => TRUE,
  );

  $form['editSensor']['GasCount'] = array(
    '#type' => 'hidden',
    '#value' => $sensorObj->GasCount,
  );
 
  $form['editSensor']['Alias'] = array(
    '#type' => 'textfield',
    '#title' => t('Alias'),
    '#maxlength' => 64,
    '#default_value' => checkNull($sensorObj->Alias) ? "N.D." : $sensorObj->Alias,
  );
 
  $form['editSensor']['PartNumber'] = array(
    '#type' => 'textfield',
    '#title' => t('Part Number'),
    '#maxlength' => 64,
    '#default_value' => checkNull($sensorObj->PartNumber) ? "N.D." : $sensorObj->PartNumber,
    '#required' => TRUE,
  );

  $form['editSensor']['BrandID'] = array(
    '#type' => 'select',
    '#title' => t('Brand'),
    '#maxlength' => 64,
    '#default_value' => variable_get('BrandID', $sensorObj->BrandID),
    '#options' => $brands,
  );
 
  $form['editSensor']['Technology'] = array(
    '#type' => 'textfield',
    '#title' => t('Technology'),
    '#maxlength' => 64,
    '#default_value' => checkNull($sensorObj->Technology) ? "N.D." : $sensorObj->Technology,
  );
 
  $form['editSensor']['ActivePart'] = array(
    '#type' => 'select',
    '#title' => t('Active Part'),
    '#maxlength' => 64,
    '#default_value' => checkNull($sensorObj->ActivePart) ? variable_get('ActivePart', 'NULL') : variable_get('ActivePart', $sensorObj->ActivePart),
    '#options' => $activeOptions,
  );
 
  $form['editSensor']['LastReview'] = array(
    '#type' => 'date',
    '#title' => t('Last Review'),
    '#default_value' => $array,
  );
 
  $form['editSensor']['DatasheetReview'] = array(
    '#type' => 'textfield',
    '#title' => t('DatasheetReview'),
    '#maxlength' => 64,
    '#default_value' => checkNull($sensorObj->DatasheetReview) ? "N.D." : $sensorObj->DatasheetReview,
  );
 
  $form['editSensor']['Series'] = array(
    '#type' => 'textfield',
    '#title' => t('Series'),
    '#maxlength' => 64,
    '#default_value' => checkNull($sensorObj->Series) ? "N.D." : $sensorObj->Series,
  );
 
  $form['editSensor']['Diameter'] = array(
    '#type' => 'textfield',
    '#title' => t('Diameter'),
    '#maxlength' => 64,
    '#default_value' => checkNull($sensorObj->Diameter) ? "N.D." : $sensorObj->Diameter,
  );
 
  $form['editSensor']['Height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#maxlength' => 64,
    '#default_value' => checkNull($sensorObj->Height) ? "N.D." : $sensorObj->Height,
  );
 
  $form['editSensor']['PinCount'] = array(
    '#type' => 'textfield',
    '#title' => t('Pin Count'),
    '#maxlength' => 64,
    '#default_value' => checkNull($sensorObj->PinCount) ? "N.D." : $sensorObj->PinCount,
  );
 
  $form['editSensor']['PinLength'] = array(
    '#type' => 'textfield',
    '#title' => t('Pin Length'),
    '#maxlength' => 64,
    '#default_value' => checkNull($sensorObj->PinLength) ? "N.D." : $sensorObj->PinLength,
  );
 
  $form['editSensor']['MaxWeight'] = array(
    '#type' => 'textfield',
    '#title' => t('Max Weight'),
    '#maxlength' => 64,
    '#default_value' => checkNull($sensorObj->MaxWeight) ? "N.D." : $sensorObj->MaxWeight,
  );
 
  $form['editSensor']['MinTemperature'] = array(
    '#type' => 'textfield',
    '#title' => t('MinTemperature'),
    '#maxlength' => 64,
    '#default_value' => checkNull($sensorObj->MinTemperature) ? "N.D." : $sensorObj->MinTemperature,
  );
 
 
  $form['editSensor']['MaxTemperature'] = array(
    '#type' => 'textfield',
    '#title' => t('Max Temp'),
    '#maxlength' => 64,
    '#default_value' => checkNull($sensorObj->MaxTemperature) ? "N.D." : $sensorObj->MaxTemperature,
  );
 
  $form['editSensor']['MinPressure'] = array(
    '#type' => 'textfield',
    '#title' => t('Min Pressure'),
    '#maxlength' => 64,
    '#default_value' => checkNull($sensorObj->MinPressure) ? "N.D." : $sensorObj->MinPressure,
  );
 
  $form['editSensor']['MaxPressure'] = array(
    '#type' => 'textfield',
    '#title' => t('Max Pressure'),
    '#maxlength' => 64,
    '#default_value' => checkNull($sensorObj->MaxPressure) ? "N.D." : $sensorObj->MaxPressure,
  );
 
  $form['editSensor']['MinHumidity'] = array(
    '#type' => 'textfield',
    '#title' => t('Min Humidity'),
    '#maxlength' => 64,
    '#default_value' => checkNull($sensorObj->MinHumidity) ? "N.D." : $sensorObj->MinHumidity,
  );
 
  $form['editSensor']['MaxHumidity'] = array(
    '#type' => 'textfield',
    '#title' => t('Max Humidity'),
    '#maxlength' => 64,
    '#default_value' => checkNull($sensorObj->MaxHumidity) ? "N.D." : $sensorObj->MaxHumidity,
  );
 
 
  $form['editSensor']['Bias'] = array(
    '#type' => 'textfield',
    '#title' => t('Bias'),
    '#maxlength' => 64,
    '#default_value' => checkNull($sensorObj->Bias) ? "N.D." : $sensorObj->Bias,
  );
 
  $form['editSensor']['MinLoad'] = array(
    '#type' => 'textfield',
    '#title' => t('Min Load'),
    '#maxlength' => 64,
    '#default_value' => checkNull($sensorObj->MinLoad) ? "N.D." : $sensorObj->MinLoad,
  );
 
  $form['editSensor']['MaxLoad'] = array(
    '#type' => 'textfield',
    '#title' => t('Max Load'),
    '#maxlength' => 64,
    '#default_value' => checkNull($sensorObj->MaxLoad) ? "N.D." : $sensorObj->MaxLoad,
  );
 
 
  $form['editSensor']['MaxSignalDrift'] = array(
    '#type' => 'textfield',
    '#title' => t('Max Signal Drift'),
    '#maxlength' => 64,
    '#default_value' => checkNull($sensorObj->MaxSignalDrift) ? "N.D." : $sensorObj->MaxSignalDrift,
  );
 
  $form['editSensor']['SignalDriftInterval'] = array(
    '#type' => 'textfield',
    '#title' => t('Signal Drift Interval'),
    '#maxlength' => 64,
    '#default_value' => checkNull($sensorObj->SignalDriftInterval) ? "N.D." : $sensorObj->SignalDriftInterval,
  );
 
  $form['editSensor']['ExpectedLife'] = array(
    '#type' => 'textfield',
    '#title' => t('Expected Life'),
    '#maxlength' => 64,
    '#default_value' => checkNull($sensorObj->ExpectedLife) ? "N.D." : $sensorObj->ExpectedLife,
  );
 
  $form['editSensor']['Warranty'] = array(
    '#type' => 'textfield',
    '#title' => t('Warranty'),
    '#maxlength' => 64,
    '#default_value' => checkNull($sensorObj->Warranty) ? "N.D." : $sensorObj->Warranty,
  );
 
  $form['editSensor']['saveBtn'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#name' => 'Save',
    '#attributes' => array('class' => 'sensor-search-btn sensormodule-btn')
  );

  $form['editSensor']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#name' => 'Cancel',
    '#attributes' => array('class' => 'sensor-search-btn sensormodule-btn')
  );

  $form['editSensor']['deleteBtn'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#name' => 'Delete',
    '#attributes' => array('class' => 'sensormodule-delete-btn sensormodule-btn')

  );

  db_set_active('default');
  return $form;
 
}


//**************************************************************
//  NAME: sensormodule_modifySensorForm_validate
//  USE: Passes edited values to validation function for validation
//  DATE: 27/09/2020
//**************************************************************
function sensormodule_modifySensorForm_validate($form, &$form_state) {
  $lastReviewDate = formatFormDate($form_state['values']['editSensor']['LastReview']);
  $form_state['values']['editSensor']['LastReview'] = $lastReviewDate;
  $clicked_button = $form_state['clicked_button']['#name'];
  $sensorID = $form_state['values']['editSensor']['SensorID'];

  if($clicked_button == "Delete") {
    drupal_goto("sensormodule/edit/sensor/remove", "sensorID=".$sensorID);
  }
  else if($clicked_button == "Cancel") {
    drupal_set_message("No changes made.");
    drupal_goto("sensormodule/edit/sensor");
  }
  else if ($clicked_button == "Save") {
    sensorDetailValidation($form['editSensor'], $form_state['values']['editSensor']);
  }
}

//**************************************************************
//  NAME: sensormodule_modifySensorForm_submit
//  USE: Submits the edited and validated values to the MySQL database
//  DATE: 27/09/2020
//**************************************************************
function sensormodule_modifySensorForm_submit($form, &$form_state) {

  // As Part Number is a required field, if no alias is given make the Part Number the alias
  if(checkNull($form_state['values']['editSensor']['Alias'])) {
    $form_state['values']['editSensor']['Alias'] = $form_state['values']['editSensor']['PartNumber'];
  }

  $SensorID = $form_state['values']['editSensor']['SensorID'];
  $alias = $form_state['values']['editSensor']['Alias'];
  $partNum = $form_state['values']['editSensor']['PartNumber'];
  $tech = $form_state['values']['editSensor']['Technology'];
  $activePart = $form_state['values']['editSensor']['ActivePart'];
  $lastReview = $form_state['values']['editSensor']['LastReview'];
  $datasheet = $form_state['values']['editSensor']['DatasheetReview'];
  $series = $form_state['values']['editSensor']['Series'];
  $diameter = $form_state['values']['editSensor']['Diameter'];
  $height = $form_state['values']['editSensor']['Height'];
  $pinCount = $form_state['values']['editSensor']['PinCount'];
  $pinLength = $form_state['values']['editSensor']['PinLength'];
  $maxWeight = $form_state['values']['editSensor']['MaxWeight'];
  $minTemp = $form_state['values']['editSensor']['MinTemperature'];
  $maxTemp = $form_state['values']['editSensor']['MaxTemperature'];
  $minPressure = $form_state['values']['editSensor']['MinPressure'];
  $maxPressure = $form_state['values']['editSensor']['MaxPressure'];
  $minHumidity = $form_state['values']['editSensor']['MinHumidity'];
  $maxHumidity = $form_state['values']['editSensor']['MaxHumidity'];
  $bias = $form_state['values']['editSensor']['Bias'];
  $minLoad = $form_state['values']['editSensor']['MinLoad'];
  $maxLoad = $form_state['values']['editSensor']['MaxLoad'];
  $maxSigDrift = $form_state['values']['editSensor']['MaxSignalDrift'];
  $sigDriftInt = $form_state['values']['editSensor']['SignalDriftInterval'];
  $expectedLife = $form_state['values']['editSensor']['ExpectedLife'];
  $warranty = $form_state['values']['editSensor']['Warranty'];
  $gasCount = $form_state['values']['editSensor']['GasCount'];
  $brand = $form_state['values']['editSensor']['BrandID'];

  //To increase performance of program, rather than changing the active database twice per field it is easier to set it at the start and end of all upated fields.
  db_set_active('gas_sensor_db');
  updateSensorField('Alias', $alias, $SensorID);
  updateSensorField('PartNumber', $partNum, $SensorID);
  updateSensorField('Technology', $tech, $SensorID);
  updateSensorField('ActivePart', $activePart, $SensorID);
  updateSensorField('LastReview', $lastReview, $SensorID);
  updateSensorField('DatasheetReview', $datasheet, $SensorID);
  updateSensorField('Series', $series, $SensorID);
  updateSensorField('Diameter', $diameter, $SensorID);
  updateSensorField('Height', $height, $SensorID);
  updateSensorField('PinCount', $pinCount, $SensorID);
  updateSensorField('PinLength', $pinLength, $SensorID);
  updateSensorField('MaxWeight', $maxWeight, $SensorID);
  updateSensorField('MinTemperature', $minTemp, $SensorID);
  updateSensorField('MaxTemperature', $maxTemp, $SensorID);
  updateSensorField('MinPressure', $minPressure, $SensorID);
  updateSensorField('MaxPressure', $maxPressure, $SensorID);
  updateSensorField('MinHumidity', $minHumidity, $SensorID);
  updateSensorField('MaxHumidity', $maxHumidity, $SensorID);
  updateSensorField('Bias', $bias, $SensorID);
  updateSensorField('MinLoad', $minLoad, $SensorID);
  updateSensorField('MaxLoad', $maxLoad, $SensorID);
  updateSensorField('MaxSignalDrift', $maxSigDrift, $SensorID);
  updateSensorField('SignalDriftInterval', $sigDriftInt, $SensorID);
  updateSensorField('ExpectedLife', $expectedLife, $SensorID);
  updateSensorField('Warranty', $warranty, $SensorID);
  updateSensorField('GasCount', $gasCount, $SensorID);
  updateSensorField('BrandID', $brand, $SensorID);
  db_set_active('default');

  drupal_set_message("Sensor changes saved!");
  drupal_goto("sensormodule/edit/sensor/");
}

//***************************************************************
// NAME: sensormodule_modifyDetectForm
// USE: Loads existing Detect entry values into the Detect table fields
// DATE: 12/10/2020
//***************************************************************
function sensormodule_modifyDetectForm(&$form_state, $SensorID, $GasID){

  $detectInfo =  getRelatedDetectsInfo($SensorID, $GasID);
  $measurementOptions['ppm'] = t('ppm');
  $measurementOptions['%Vol'] =t('%Vol');
  $measurementOptions['mg/m^3']=t('mg/m^3');
  $measurementOptions[$detectInfo->MeasurementUnits] =t($detectInfo->MeasurementUnits);

  $sensitivityOptions['nA/ppm'] = t('nA/ppm');
  $sensitivityOptions['nA/mg/m^3'] = t('nA/mg/m^3');
  $sensitivityOptions['mA/ppm'] = t('mA/ppm');
  $sensitivityOptions['µA/ppm'] = t('µA/ppm');
  $sensitivityOptions['µA'] = t('µA');
  $sensitivityOptions[$detectInfo->SensitivityRatio] =t($detectInfo->SensitivityRatio);

  $form['editDetect'] = array(
    '#type' => 'fieldset',
    '#title' => t('Detects Gas Details'),
    '#tree' => TRUE,
    '#attributes' => array('class' => 'form-fieldset'),
  );
  
  $form['editDetect']['SensorID'] = array(
    '#type' => 'textfield',
    '#title' => t('Sensor ID'),
    '#value' => $SensorID,
    '#disabled' => TRUE,
  );

  $form['editDetect']['GasID'] = array(
    '#type' => 'textfield',
    '#title' => t('Gas ID'),
    '#value' => $GasID,
    '#disabled' => TRUE,
  );
    
  $form['editDetect']['Name'] = array(
    '#type' => 'textfield',
    '#title' => t('Empirical Gas Name'),
    '#value' => $detectInfo->Name,
    '#disabled' => TRUE,
    '#maxlength' => 64,
  );

  $form['editDetect']['MeasurementUnits'] = array(
    '#type' => 'select',
    '#title' => t('Measurement Units'),
    '#default_value' => checkNull($measurementOptions[$detectInfo->MeasurementUnits]) ? "N.D." : $measurementOptions[$detectInfo->MeasurementUnits],
    '#options' => $measurementOptions,
    
  );

  $form['editDetect']['MinReading'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum Reading'),
    '#default_value' => checkNull($detectInfo->MinReading) ? "N.D." : $detectInfo->MinReading,
    '#maxlength' => 64,
  );

  $form['editDetect']['MaxReading'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum Reading'),
    '#default_value' => checkNull($detectInfo->MaxReading) ? "N.D." : $detectInfo->MaxReading,
    '#maxlength' => 64,
  );

  $form['editDetect']['Resolution'] = array(
    '#type' => 'textfield',
    '#title' => t('Resolution'),
    '#default_value' => checkNull($detectInfo->Resolution) ? "N.D." : $detectInfo->Resolution,
    '#maxlength' => 64,
  );

  $form['editDetect']['OvergasLimit'] = array(
    '#type' => 'textfield',
    '#title' => t('Overgas Limit'),
    '#default_value' => checkNull($detectInfo->OvergasLimit) ? "N.D." : $detectInfo->OvergasLimit,
    '#maxlength' => 64,
  );

  $form['editDetect']['MinSensitivity'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum Sensitivity'),
    '#default_value' => checkNull($detectInfo->MinSensitivity) ? "N.D." : $detectInfo->MinSensitivity,
    '#maxlength' => 64,
  );

  $form['editDetect']['MaxSensitivity'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum Sensitivity'),
    '#default_value' => checkNull($detectInfo->MaxSensitivity) ? "N.D." : $detectInfo->MaxSensitivity,
    '#maxlength' => 64,
  );

  $form['editDetect']['SensitivityRatio'] = array(
    '#type' => 'select',
    '#title' => t('Sensitivity Ratio'),
    '#default_value' => variable_get('SensitivityRatio', $detectInfo->SensitivityRatio),
    '#options' => $sensitivityOptions,
  );

  $form['editDetect']['SensitivityTestConcentration'] = array(
    '#type' => 'textfield',
    '#title' => t('Sensitivity Test Concentration'),
    '#default_value' => checkNull($detectInfo->SensitivityTestConcentration) ? "N.D." : $detectInfo->SensitivityTestConcentration,
    '#maxlength' => 64,
  );

  $form['editDetect']['TTestType'] = array(
    '#type' => 'textfield',
    '#title' => t('TTest Type'),
    '#default_value' => checkNull($detectInfo->TTestType) ? "N.D." : $detectInfo->TTestType,
    '#maxlength' => 64,
  );

  $form['editDetect']['TTestResponse'] = array(
    '#type' => 'textfield',
    '#title' => t('TTest Response'),
    '#default_value' => checkNull($detectInfo->TTestResponse) ? "N.D." : $detectInfo->TTestResponse,
    '#maxlength' => 64,
  );

  $form['editDetect']['ZeroCurrent'] = array(
    '#type' => 'textfield',
    '#title' => t('Zero Current'),
    '#default_value' => checkNull($detectInfo->ZeroCurrent) ? "N.D." : $detectInfo->ZeroCurrent,
    '#maxlength' => 64,
  );

  $form['editDetect']['ResponseStartLevel'] = array(
    '#type' => 'textfield',
    '#title' => t('Response Start Level'),
    '#default_value' => checkNull($detectInfo->ResponseStartLevel) ? "N.D." : $detectInfo->ResponseStartLevel,
    '#maxlength' => 64,
  );

  $form['editDetect']['ResponseEndLevel'] = array(
    '#type' => 'textfield',
    '#title' => t('Response End Level'),
    '#default_value' => checkNull($detectInfo->ResponseEndLevel) ? "N.D." : $detectInfo->ResponseEndLevel,
    '#maxlength' => 64,
  );

  $form['editDetect']['saveBtn'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#name' => 'Save',
    '#attributes' => array('class' => 'sensor-search-btn sensormodule-btn')
  );

  $form['editDetect']['backBtn'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#name' => 'Cancel',
    '#attributes' => array('class' => 'sensor-search-btn sensormodule-btn')
  );
  $form['editDetect']['sensorID'] = array(
    '#type' => 'hidden',
    '#value' => $SensorID,
  );
  $form['editDetect']['gasID'] = array(
    '#type' => 'hidden',
    '#value' => $GasID,
  ); 
   return $form;
}

//***************************************************************
// NAME: sensormodule_modifyDetectForm_validate
// USE: Performs validation on all detects data
// DATE: 12/10/2020
//***************************************************************
function sensormodule_modifyDetectForm_validate($form, &$form_state) {
  
  $clicked_button = $form_state['clicked_button']['#name'];
  $sensorID = $form_state['values']['editDetect']['sensorID'];

  if($clicked_button == "Cancel") {
    drupal_set_message("No changes made.");
    drupal_goto("sensormodule/edit/sensor/modify/", "sensorID=".$sensorID);
  }
  else if($clicked_button == "Save") {
    sensorDetectDetailValidation($form['editDetect'], $form_state['values']['editDetect']);
  }
}
 
//***************************************************************
// NAME: sensormodule_modifyDetectForm_submit
// USE: Button to validate the users input, then implements the changes to Detects table
// DATE: 12/10/2020
//***************************************************************
function sensormodule_modifyDetectForm_submit($form, &$form_state){

  $SensorID = $form_state['values']['editDetect']['sensorID'];
  $GasID = $form_state['values']['editDetect']['gasID'];
  $Name = $form_state['values']['editDetect']['Name'];
  $Measurement = $form_state['values']['editDetect']['MeasurementUnits'];
  $MinReading = $form_state['values']['editDetect']['MinReading'];
  $MaxReading = $form_state['values']['editDetect']['MaxReading'];
  $Resolution = $form_state['values']['editDetect']['Resolution'];
  $OvergasLimit = $form_state['values']['editDetect']['OvergasLimit'];
  $MinSensitivity = $form_state['values']['editDetect']['MinSensitivity'];
  $MaxSensitivity = $form_state['values']['editDetect']['MaxSensitivity'];
  $SensitivityRatio = $form_state['values']['editDetect']['SensitivityRatio'];
  $SensitivityTestConcentration = $form_state['values']['editDetect']['SensitivityTestConcentration'];
  $TTestType = $form_state['values']['editDetect']['TTestType'];
  $TTestResponse = $form_state['values']['editDetect']['TTestResponse'];
  $ResponseStartLevel = $form_state['values']['editDetect']['ResponseStartLevel'];
  $ResponseEndLevel = $form_state['values']['editDetect']['ResponseEndLevel'];
  $ZeroCurrent = $form_state['values']['editDetect']['ZeroCurrent'];
  
  db_set_active('gas_sensor_db');
  updateDetectsField('MeasurementUnits', $Measurement, $SensorID, $GasID);
  updateDetectsField('MinReading', $MinReading, $SensorID, $GasID);
  updateDetectsField('MaxReading', $MaxReading, $SensorID, $GasID);
  updateDetectsField('Resolution', $Resolution, $SensorID, $GasID);
  updateDetectsField('OvergasLimit', $OvergasLimit, $SensorID, $GasID);
  updateDetectsField('MinSensitivity', $MinSensitivity, $SensorID, $GasID);
  updateDetectsField('MaxSensitivity', $MaxSensitivity, $SensorID, $GasID);
  updateDetectsField('SensitivityRatio', $SensitivityRatio, $SensorID, $GasID);
  updateDetectsField('SensitivityTestConcentration', $SensitivityTestConcentration, $SensorID, $GasID);
  updateDetectsField('TTestType', $TTestType, $SensorID, $GasID);
  updateDetectsField('TTestResponse', $TTestResponse, $SensorID, $GasID);
  updateDetectsField('ResponseStartLevel', $ResponseStartLevel, $SensorID, $GasID);
  updateDetectsField('ResponseEndLevel', $ResponseEndLevel, $SensorID, $GasID);
  updateDetectsField('ZeroCurrent', $ZeroCurrent, $SensorID, $GasID);
  db_set_active('default');

  $alias = getSensorInfo($SensorID)->Alias;
  
  drupal_set_message("Changes to $alias / $Name have been saved!");
  drupal_goto('sensormodule/edit/sensor/modify/', "sensorID=".$SensorID);
}

//==============================================================
// REMOVE
//==============================================================
//**************************************************************
// NAME: sensormodule_removeSensorConfirmation()
// USE: A form for the user to confirm the deletion of a sensor
// DATE: 28/9/2020
//**************************************************************
function sensormodule_removeSensorConfirmation(&$form_state, $sensorID) {
  $form['deleteSensor'] = array(
    '#type' => 'fieldset',
    '#title' => t('Are you sure?'),
    '#tree' => TRUE,
    '#attributes' => array('class' => 'delete-form-fieldset'),
  );

  $form['deleteSensor']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#name' => 'Delete',
    '#attributes' => array(
      'onclick' => 'if(!confirm("Are you really sure?")) {return false;}',
      'class' => 'sensormodule-delete-btn sensormodule-btn'),
  );

  $form['deleteSensor']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#name' => 'Cancel',
    '#attributes' => array('class' => 'sensor-search-btn sensormodule-btn')
  );

  $form['deleteSensor']['sensorID'] = array(
    '#type' => 'hidden',
    '#value' => $sensorID,
  );

  return $form;
}

//***************************************************************
// NAME: sensormodule_removeSensorConfirmation_submit_delete
// USE: submit function for back button redirects to edit sensor page
// DATE: 28/9/2020
//***************************************************************
function sensormodule_removeSensorConfirmation_submit($form, &$form_state) {
  $clicked_button = $form_state['clicked_button']['#name'];
  $sensorID = $form_state['values']['deleteSensor']['sensorID'];
  if($clicked_button == "Cancel") {
    drupal_set_message("No changes made.");
    drupal_goto("sensormodule/edit/sensor");
  }
  else if($clicked_button == "Delete") {
    db_set_active('gas_sensor_db');
    db_query("DELETE FROM Sensor WHERE SensorID=$sensorID");
    db_set_active('default');
    drupal_set_message("Sensor was successfully deleted.");
    drupal_goto("sensormodule/edit/sensor");
  }
}


//***************************************************************
// NAME: sensormodule_removeDetectConfirmation
// USE:  Form containing all the fields for the delete confirmation page
// DATE: 14/10/2020
//***************************************************************
function sensormodule_removeDetectConfirmation(&$form_state, $sensorID, $gasID) {
  $form['deleteDetects'] = array(
   '#type' => 'fieldset',
   '#title' => t('Are you sure?'),
   '#tree' => TRUE,
   '#attributes' => array('class' => 'delete-form-fieldset'),
   );

  $form['deleteDetects']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#name' => 'Delete',
    '#attributes' => array('class' => 'sensormodule-delete-btn sensormodule-btn'), 
  );

  $form['deleteDetects']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#name' => 'Cancel',
    '#attributes' => array('class' => 'sensor-search-btn sensormodule-btn'), 
  );

  $form['deleteDetects']['SensorID'] = array(
    '#type' => 'hidden',
    '#value' => $sensorID,
  );

  $form['deleteDetects']['GasID'] = array(
    '#type' => 'hidden',
    '#value' => $gasID,
  );

  return $form;
}

//***************************************************************
// NAME: sensormodule_removeDetectConfirmation_submit
// USE:  Submit function to remove the detect entry from the Detects table
// DATE: 14/10/2020
//***************************************************************
function sensormodule_removeDetectConfirmation_submit($form, &$form_state) {
  $clicked_button = $form_state['clicked_button']['#name'];
  $sensorID = $form_state['values']['deleteDetects']['SensorID'];
  $gasID = $form_state['values']['deleteDetects']['GasID'];

  if($clicked_button == "Cancel") {
    drupal_set_message("No changes made.");
    drupal_goto("sensormodule/edit/sensor/modify/", "sensorID=$sensorID");
  }
  else if($clicked_button == "Delete") {
    db_set_active('gas_sensor_db');
    db_query("DELETE FROM Detects WHERE SensorID='%s' AND GasID='%s';",$sensorID, $gasID);
    db_query("UPDATE Sensor SET GasCount = GasCount-1 WHERE SensorID='%s';", $sensorID);
    db_set_active('default');
  
    drupal_set_message("Sucessfully removed");
    drupal_goto('sensormodule/edit/sensor/modify/', "sensorID=".$sensorID);
  }
}

//==============================================================
// MISC. FUNCTIONS
//==============================================================
//**************************************************************
// NAME: getSensor
// USE: Gets the sensor information to output on the removes sensor page
// DATE: 28/9/2020
//**************************************************************
function getRemoveSensorInfoTable($ID) {
  db_set_active('gas_sensor_db');
  $header = array('SensorID','Alias');
  $results = db_query("SELECT SensorID, Alias FROM Sensor WHERE SensorID='%s';", $ID);
  $data = array();
  while($row = db_fetch_array($results)) {
    $data[] = array($row['SensorID'], $row['Alias']);
  }
  db_set_active('default');

  $table = theme('table', $header, $data);
  $page_content = $table;
  
  return $page_content;

}

//**************************************************************
//  NAME: createSensorInsertQuery
//  USE: Dynamically creates a insert query for the
//       Sensor table. Relies on the linear layout of the
//        add sensor form
//  DATE: 27/09/2020
//**************************************************************
function createSensorInsertQuery($data) {

  // List of fields which are strings (or dates) and thus need single quotes ' ' around their values in query 
  $stringFields = array( 'GasCount', 'Alias', 'PartNumber', 'Technology', 'Series', 'LastReview');
  // List of fields to ignore
  $ignoreFields = array( 'finishBtn', 'cancelBtn', 'Gasses', 'file_found', 'fatal_error', 'format_parser_detected');

  // Init field and value parts of query
  $insertFields = "INSERT INTO Sensor (";
  $insertValues = "VALUES (";

  // Loop through each field in data
  // Check if field should be ignored
  //   Add field name
  //   Check if value null then add a NULL to query
  //   Else check if its a string and add single quotes
  //   Else just add the value
  //   If its the last element of the query no comma and add closing brackets
  foreach($data as $fieldname => $value) {
    
    if( !in_array($fieldname, $ignoreFields) && $fieldname != "Warranty") {
      $insertFields .= "$fieldname, ";
      if(checkNull($value)) {
        $insertValues .= "NULL, ";
      }
      else if( in_array($fieldname, $stringFields) ) {
        $insertValues .= "'$value', ";
      }
      else {
        $insertValues .= "$value, ";
      }
    }
    else if($fieldname == "Warranty") {
      $insertFields .= "Warranty)";
      if(checkNull($value)) {
        $insertValues .= "NULL);";
      }
      else {
        $insertValues .= "$value);";
      }
    }
  }

  // Concat insert and field parts of query
  $query = $insertFields.$insertValues;
  return $query;
}

//**************************************************************
//  NAME: createSensorDetectInsertQuery
//  USE: Dynamically creates a insert query for the
//       Detects table.
//  DATE: 14/10/2020
//**************************************************************
function createSensorDetectInsertQuery($data) {

  // List of fields which are strings (or dates) and thus need single quotes ' ' around their values in query 
  $stringFields = array( 'MeasurementUnits', 'SensitivityRatio', 'TTestType', 'ZeroCurrent' );
  // List of fields to ignore
  $ignoreFields = array( 'addBtn', 'cancelBtn' , 'AliasField', 'Fullname', 'Name', 'GasName');
  // Last field to know when to close query
  $lastField = "GasID";

  // Init field and value parts of query
  $insertFields = "INSERT INTO Detects (";
  $insertValues = "VALUES (";

  // Loop through each field in data
  // Check if field should be ignored
  //   Add field name
  //   Check if value null then add a NULL to query
  //   Else check if its a string and add single quotes
  //   Else just add the value
  //   If not the last field add a comma
  //   If the last field add closing bracket
  foreach($data as $fieldname => $value) {
    if( !in_array($fieldname, $ignoreFields) ) {
      $insertFields .= $fieldname;

      if(checkNull($value)) {
        $insertValues .= "NULL";
      }
      else if( in_array($fieldname, $stringFields) ) {
        $insertValues .= "'$value'";
      }
      else {
        $insertValues .= "$value";
      }

      if($fieldname != $lastField) {
        $insertValues .= ", ";
        $insertFields .= ", ";
      }
      else {
        $insertFields .= ")";
        $insertValues .= ");";
      }
    }
  }

  // Concat insert and field parts of query
  $query = $insertFields.$insertValues;
  return $query;
}


//**************************************************************
//  NAME: createSensorDetailsTable
//  USE: Creates a table filled with all sensor info for a given ID
//  DATE: 23/10/2020
//**************************************************************
function createSensorDetailsTable($sensorID) {
  // Get all sensors data
  $sensorObj = getSensorInfo($sensorID);
  $numGas = intval($sensorObj->GasCount);
  // Format the data for display
  $sensorObj = formatSensorDataForDisplay($sensorObj);
  // Fill the table
  $table = '
    <div id="sensor-info-btns">
    </div>
    <hr class="sensormodule-hr">
    <div id="sensor-info-header">
    <h1 class="sensor-info-table-title">'.$sensorObj->PartNumber.' ( '.$sensorObj->Alias.' )</h1>
    </div>
    <hr class="sensormodule-hr">
    <table class="sensor-info-table">
      <thead><tr><th colspan="2">Sensor Info</th><tr></thead>
      <tbody>
        <tr>
          <td class="sensor-info-table-label" colspan="1">Techhnology</td>
          <td class="sensor-info-table-value" colspan="1">'.$sensorObj->Technology.'</td>
        </tr>
        <tr>
          <td class="sensor-info-table-label" colspan="1">Active Part</td>
          <td class="sensor-info-table-value" colspan="1">'.$sensorObj->ActivePart.'</td>
        </tr>
        <tr>
          <td class="sensor-info-table-label" colspan="1">Last Review</td>
          <td class="sensor-info-table-value" colspan="1">'.$sensorObj->LastReview.'</td>
        </tr>
        <tr>
          <td class="sensor-info-table-label" colspan="1">Datasheet Version</td>
          <td class="sensor-info-table-value" colspan="1">'.$sensorObj->DatasheetReview.'</td>
        </tr>
        <tr>
          <td class="sensor-info-table-label" colspan="1">Expected Life</td>
          <td class="sensor-info-table-value" colspan="1">'.$sensorObj->ExpectedLife.'</td>
        </tr>
        <tr>
          <td class="sensor-info-table-label" colspan="1">Warranty</td>
          <td class="sensor-info-table-value" colspan="1">'.$sensorObj->Warranty.'</td>
        </tr>
      </tbody>
      <thead><tr><th colspan="2">Dimensions</th><tr></thead>
        <tbody>
          <tr>
            <td class="sensor-info-table-label" colspan="1">Height</td>
            <td class="sensor-info-table-value" colspan="1">'.$sensorObj->Height.'</td>
          </tr>
          <tr>
            <td class="sensor-info-table-label" colspan="1">Pin Count</td>
            <td class="sensor-info-table-value" colspan="1">'.$sensorObj->PinCount.'</td>
          </tr>
          <tr>
            <td class="sensor-info-table-label" colspan="1">Pin Length</td>
            <td class="sensor-info-table-value" colspan="1">'.$sensorObj->PinLength.'</td>
          </tr>
          <tr>
            <td class="sensor-info-table-label" colspan="1">Weight</td>
            <td class="sensor-info-table-value" colspan="1">'.$sensorObj->MaxWeight.'</td>
          </tr>
        </tbody>
      <thead><tr><th colspan="2">Environment</th><tr></thead>
        <tbody>
          <tr>
            <td class="sensor-info-table-label" colspan="1">Temperature</td>
            <td class="sensor-info-table-value" colspan="1">'.$sensorObj->Temperature.'</td>
          </tr>
          <tr>
            <td class="sensor-info-table-label" colspan="1">Pressure</td>
            <td class="sensor-info-table-value" colspan="1">'.$sensorObj->Pressure.'</td>
          </tr>
          <tr>
            <td class="sensor-info-table-label" colspan="1">Humidity</td>
            <td class="sensor-info-table-value" colspan="1">'.$sensorObj->Humidity.'</td>
          </tr>
        </tbody>
      <thead><tr><th colspan="2">Electrical</th><tr></thead>
        <tbody>
          <tr>
            <td class="sensor-info-table-label" colspan="1">Load</td>
            <td class="sensor-info-table-value" colspan="1">'.$sensorObj->Load.'</td>
          </tr>
          <tr>
            <td class="sensor-info-table-label" colspan="1">Bias</td>
            <td class="sensor-info-table-value" colspan="1">'.$sensorObj->Bias.'</td>
          </tr>
          <tr>
            <td class="sensor-info-table-label" colspan="1">Max Signal Drift</td>
            <td class="sensor-info-table-value" colspan="1">'.$sensorObj->MaxSignalDrift.'</td>
          </tr>
          <tr>
            <td class="sensor-info-table-label" colspan="1">Signal Drift Interval</td>
            <td class="sensor-info-table-value" colspan="1">'.$sensorObj->SignalDriftInterval.'</td>
          </tr>
        </tbody>
  </table>';

  // If there is gas detection info, get it and append formatted data to table
  if($numGas > 0) {
    $table .= createDetectsDetailsTable($sensorID, $numGas);
  }
  return $table;
}


//**************************************************************
//  NAME: createDetectsDetailsTable
//  USE: Creates a table filled with all detects info for a given sensorID
//  DATE: 23/10/2020
//**************************************************************
function createDetectsDetailsTable($sensorID, $numGas) {
  // Table title
  $table = '
  <hr class="sensormodule-hr">
  <h1 class="sensor-info-table-title">Gas Detection Info</h1>
  <hr class="sensormodule-hr">';
  
  // Get detection info
  $detectsArray = getAllRelatedDetectsInfo($sensorID);
  // For each gas detected append to table
  for($i=0; $i<$numGas; $i++) {
    $data = formatDetectsDataForDisplay($detectsArray[$i]);
    $table .= '
    <table class="sensor-info-table">
    <thead><tr><th colspan="2">'.$data->Fullname.'</th><tr></thead>
      <tbody>
        <tr>
          <td class="sensor-info-table-label" colspan="1">Empirical</td>
          <td class="sensor-info-table-value" colspan="1">'.$data->Name.'</td>
        </tr>
        <tr>
          <td class="sensor-info-table-label" colspan="1">Reading Range</td>
          <td class="sensor-info-table-value" colspan="1">'.$data->Reading.'</td>
        </tr>
        <tr>
          <td class="sensor-info-table-label" colspan="1">Resolution</td>
          <td class="sensor-info-table-value" colspan="1">'.$data->Resolution.'</td>
        </tr>
        <tr>
          <td class="sensor-info-table-label" colspan="1">Overgas Limit</td>
          <td class="sensor-info-table-value" colspan="1">'.$data->OvergasLimit.'</td>
        </tr>
        <tr>
          <td class="sensor-info-table-label" colspan="1">Sensitivity</td>
          <td class="sensor-info-table-value" colspan="1">'.$data->Sensitivity.'</td>
        </tr>
        <tr>
          <td class="sensor-info-table-label" colspan="1">Sensitivity Test Concentration</td>
          <td class="sensor-info-table-value" colspan="1">'.$data->SensitivityTestConcentration.'</td>
        </tr>
        <tr>
          <td class="sensor-info-table-label" colspan="1">TTest Type</td>
          <td class="sensor-info-table-value" colspan="1">'.$data->TTestType.'</td>
        </tr>
        <tr>
          <td class="sensor-info-table-label" colspan="1">'.$data->TTestType.' Response Time</td>
          <td class="sensor-info-table-value" colspan="1">'.$data->TTestResponse.'</td>
        </tr>
        <tr>
          <td class="sensor-info-table-label" colspan="1">T Response Range</td>
          <td class="sensor-info-table-value" colspan="1">'.$data->TTestRange.'</td>
        </tr>
        <tr>
          <td class="sensor-info-table-label" colspan="1">Zero Current</td>
          <td class="sensor-info-table-value" colspan="1">'.$data->ZeroCurrent.'</td>
        </tr>
      </tbody>
    </table>';
  }
  return $table;
}

//**************************************************************
//  NAME: formatSensorDataForDisplay
//  USE: Formats a sensor db object's values for display, with correct
//        units etc.
//  DATE: 23/10/2020
//**************************************************************
function formatSensorDataForDisplay($sensorObj) {
  // Technology
  if( is_null($sensorObj->Technology) ) { $sensorObj->Technology = "N.D.";}

  // Active Part
  if( is_null($sensorObj->ActivePart) ) { $sensorObj->ActivePart = "N.D.";}
  else {
    switch($sensorObj->ActivePart) {
      case '0': $sensorObj->ActivePart = "No"; break;
      case '1': $sensorObj->ActivePart = "Yes"; break;
    }
  }

  // Last Review
  if( is_null($sensorObj->LastReview) ) { $sensorObj->LastReview = "N.D.";}
  else {
    $timeZone = 'Australia/Perth';
    date_default_timezone_set($timeZone);
    $sensorObj->LastReview = date_format(date_create($sensorObj->LastReview), 'd/m/Y');
  }

  // Datasheet Review
  if( is_null($sensorObj->DatasheetReview) ) { $sensorObj->DatasheetReview = "N.D.";}
  else { $sensorObj->DatasheetReview = "Ver. ".$sensorObj->DatasheetReview; }

  // Expected Life
  if( is_null($sensorObj->ExpectedLife) ) { $sensorObj->ExpectedLife = "N.D.";}
  else { $sensorObj->ExpectedLife = $sensorObj->ExpectedLife." months"; }

  // Warranty
  if( is_null($sensorObj->Warranty) ) { $sensorObj->Warranty = "N.D.";}
  else { $sensorObj->Warranty = $sensorObj->Warranty." months"; }

  // Diameter
  if( is_null($sensorObj->Diameter) ) { $sensorObj->Diamater = "N.D.";}
  else { $sensorObj->Diameter = $sensorObj->Diameter. " mm"; }

  // Height
  if( is_null($sensorObj->Height) ) { $sensorObj->Height = "N.D.";}
  else { $sensorObj->Height = $sensorObj->Height. " mm"; }

  // Pin Count
  if( is_null($sensorObj->PinCount) ) { $sensorObj->PinCount = "N.D.";}

  // Pin Length
  if( is_null($sensorObj->PinLength) ) { $sensorObj->PinLength = "N.D.";}
  else { $sensorObj->PinLength = $sensorObj->PinLength. " mm"; }

  // Weight
  if( is_null($sensorObj->MaxWeight) ) { $sensorObj->MaxWeight = "N.D.";}
  else { $sensorObj->MaxWeight = $sensorObj->MaxWeight. " g"; }

  // Temperature Range
  // If both Min and Max Values exist
  if( !is_null($sensorObj->MinTemperature) && !is_null($sensorObj->MaxTemperature)) { $sensorObj->{"Temperature"} = $sensorObj->MinTemperature." to ".$sensorObj->MaxTemperature." C"; }
  // If Max Exists but not Min
  else if( is_null($sensorObj->MinTemperature) && !is_null($sensorObj->MaxTemperature)){ $sensorObj->{"Temperature"} = "Unknown to ".$sensorObj->MaxTemperature." C"; }
  // If Min Exists but not Max
  else if( is_null($senosrObj->MaxTemperature) && !is_null($sensorObj->MinTemperature)){ $sensorObj->{"Temperature"} = $sensorObj->MinTemperature." to Unknown C"; }
  // If both don't exists
  else { $sensorObj->{"Temperature"} = "N.D.";}

  // Pressure Range
  // If both Min and Max Values exist
  if( !is_null($sensorObj->MinPressure) && !is_null($sensorObj->MaxPressure)) { $sensorObj->{"Pressure"} = $sensorObj->MinPressure." to ".$sensorObj->MaxPressure." kPa"; }
  // If Max Exists but not Min
  else if( is_null($sensorObj->MinPressure) && !is_null($sensorObj->MaxPressure)){ $sensorObj->{"Pressure"} = "Unknown to ".$sensorObj->MaxPressure." kPa"; }
  // If Min Exists but not Max
  else if( is_null($senosrObj->MaxPressure) && !is_null($sensorObj->MinPressure)){ $sensorObj->{"Pressure"} = $sensorObj->MinPressure." to Unknown kPa"; }
  // If both don't exists
  else { $sensorObj->{"Pressure"} = "N.D.";}

  // Humidity Range
  // If both Min and Max Values exist
  if( !is_null($sensorObj->MinHumidity) && !is_null($sensorObj->MaxHumidity)) { $sensorObj->{"Humidity"} = $sensorObj->MinHumidity." to ".$sensorObj->MaxHumidity."% of RH"; }
  // If Max Exists but not Min
  else if( is_null($sensorObj->MinHumidity) && !is_null($sensorObj->MaxHumidity)){ $sensorObj->{"Humidity"} = "Unknown to ".$sensorObj->MaxHumidity."% of RH"; }
  // If Min Exists but not Max
  else if( is_null($senosrObj->MaxHumidity) && !is_null($sensorObj->MinHumidity)){ $sensorObj->{"Humidity"} = $sensorObj->MinHumidity." to Unknown% of RH"; }
  // If both don't exists
  else { $sensorObj->{"Humidity"} = "N.D.";}

  // Load
  // If both Min and Max Values exist
  if( !is_null($sensorObj->MinLoad) && !is_null($sensorObj->MaxLoad)) {
    $minLoad = $sensorObj->MinLoad;
    $maxLoad = $sensorObj->MaxLoad;
    // Only display one if Max and Min load are the same
    if( strcmp($minLoad, $maxLoad) == 0 ) { $sensorObj->{"Load"} = $sensorObj->MaxLoad." Ω";   }
    else { $sensorObj->{"Load"} = $sensorObj->MinLoad." to ".$sensorObj->MaxLoad." Ω"; }
  }
  // If Max Exists but not Min
  else if( is_null($sensorObj->MinLoad) && !is_null($sensorObj->MaxLoad)){ $sensorObj->{"Load"} = "Unknown to ".$sensorObj->MaxLoad." Ω"; }
  // If Min Exists but not Max
  else if( is_null($sensorObj->MaxLoad) && !is_null($sensorObj->MinLoad)){ $sensorObj->{"Load"} = $sensorObj->MinLoad." to Unknown Ω"; }
  // If both don't exists
  else { $sensorObj->{"Load"} = "N.D.";}

  // Bias
  if( is_null($sensorObj->Bias) ) { $sensorObj->Bias = "N.D.";}
  else { $sensorObj->Bias = $sensorObj->Bias." mV"; }

  // Max Signal Drift
  if( is_null($sensorObj->MaxSignalDrift) ) { $sensorObj->MaxSignalDrift = "N.D."; }
  else { $sensorObj->MaxSignalDrift = $sensorObj->MaxSignalDrift." months"; }

  // Signal Drift Interval
  if( is_null($sensorObj->SignalDriftInterval) ) { $sensorObj->SignalDriftInterval = "N.D."; }
  else { $sensorObj->SignalDriftInterval = $sensorObj->SignalDriftInterval." %"; }
  
  return $sensorObj;
}

//**************************************************************
//  NAME: formatSensorDataForDisplay
//  USE: Formats a detects db object's values for display, with correct
//        units etc.
//  DATE: 23/10/2020
//**************************************************************
function formatDetectsDataForDisplay($detectObj) {
  // Empirical
  if( is_null($detectObj->Name) ) { $detectObj->Name = "N.D.";}
  
  //Measurement units
  if( is_null($detectObj->MeasurementUnits) ) { $detectObj->MeasurementUnits = "(Unknown Units)";}

  // Reading Range
  // If both Min and Max Values exist
  if( !is_null($detectObj->MinReading) && !is_null($detectObj->MaxReading)) { 
    $detectObj->{"Reading"} = $detectObj->MinReading." to ".$detectObj->MaxReading." ".$detectObj->MeasurementUnits; 
  }
  // If Max Exists but not Min
  else if( is_null($detectObj->MinReading) && !is_null($detectObj->MaxReading)){ 
    $detectObj->{"Reading"} = "Unknown to ".$detectObj->MaxReading." ".$detectObj->MeasurementUnits; 
  }
  // If Min Exists but not Max
  else if( is_null($senosrObj->MaxReading) && !is_null($detectObj->MinReading)){ 
    $detectObj->{"Reading"} = $detectObj->MinReading." to Unknown ".$detectObj->MeasurementUnits; 
  }
  // If both don't exists
  else { $detectObj->{"Reading"} = "N.D.";}

  // Resolution
  if( is_null($detectObj->Resolution) ) { $detectObj->Resolution = "N.D.";}
  else { $detectObj->Resolution = $detectObj->Resolution.' '.$detectObj->MeasurementUnits; }

  // Overgas Limit
  if( is_null($detectObj->OvergasLimit) ) { $detectObj->OvergasLimit = "N.D.";}
  else { $detectObj->OvergasLimit = $detectObj->OvergasLimit." ".$detectObj->MeasurementUnits; }

  // If Sensitivity Ratio Null set Unknown Ratio
  if( is_null($detectObj->SensitivityRatio) ) { $detectObj->SensitivityRatio = "Unknown Ratio"; }

  // Sensitivity Range
  // If both Min and Max Values exist
  if( !is_null($detectObj->MinSensitivity) && !is_null($detectObj->MaxSensitivity)) { 
    $detectObj->{"Sensitivity"} = $detectObj->MinSensitivity." to ".$detectObj->MaxSensitivity." ".$detectObj->SensitivityRatio; 
  }
  // If Max Exists but not Min
  else if( is_null($detectObj->MinSensitivity) && !is_null($detectObj->MaxSensitivity)){ 
    $detectObj->{"Sensitivity"} = "Unknown to ".$detectObj->MaxSensitivity." ".$detectObj->SensitivityRatio; 
  }
  // If Min Exists but not Max
  else if( is_null($senosrObj->MaxSensitivity) && !is_null($detectObj->MinSensitivity)){ 
    $detectObj->{"Sensitivity"} = $detectObj->MinSensitivity." to Unknown ".$detectObj->SensitivityRatio; 
  }
  // If both don't exists
  else { $detectObj->{"Sensitivity"} = "N.D.";}

  // Sensitivity Test Concentration
  if( is_null($detectObj->SensitivityTestConcentration) ) { $detectObj->SensitivityTestConcentration = "N.D.";}
  else { $detectObj->SensitivityTestConcentration = $detectObj->SensitivityTestConcentration." ". $detectObj->MeasurementUnits; }

  // TTest Type
  if( is_null($detectObj->TTestType) ) { $detectObj->TTestType = "T??";}

  // TTest Response
  if( is_null($detectObj->TTestResponse) ) { $detectObj->TTestResponse = "N.D.";}
  else { $detectObj->TTestResponse = $detectObj->TTestResponse. " s"; }

  // Response Range
  // If both Min and Max Values exist
  if( !is_null($detectObj->ResponseStartLevel) && !is_null($detectObj->ResponseEndLevel)) { 
    $detectObj->{"TTestRange"} = $detectObj->ResponseStartLevel." to ".$detectObj->ResponseEndLevel." ".$detectObj->MeasurementUnits; 
  }
  // If Max Exists but not Min
  else if( is_null($detectObj->ResponseStartLevel) && !is_null($detectObj->ResponseEndLevel)) { 
    $detectObj->{"TTestRange"} = "Unknown to ".$detectObj->ResponseEndLevel." ".$detectObj->MeasurementUnits; 
  }
  // If Min Exists but not Max
  else if( is_null($senosrObj->ResponseEndLevel) && !is_null($detectObj->ResponseStartLevel)) { 
    $detectObj->{"TTestRange"} = $detectObj->ResponseStartLevel." to Unknown ".$detectObj->MeasurementUnits; 
  }
  // If both don't exists
  else { $detectObj->{"TTestRange"} = "N.D.";}

  // Zero Current
  if( is_null($detectObj->ZeroCurrent) ) { $detectObj->ZeroCurrent = "N.D.";}
  else { $detectObj->ZeroCurrent = $detectObj->ZeroCurrent. " nA"; }

  return $detectObj;
}


?>
