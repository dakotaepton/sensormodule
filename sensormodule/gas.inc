<?php
//==============================================================
// SEARCH
//==============================================================
//**************************************************************
//  NAME: sensormodule_gasSearchBar
//  USE: Produces HTML for the Search Gas Form
//  DATE: 07/09/2020
//**************************************************************
function sensormodule_gasSearchBar(&$form_state) {
  
  $form['search'] = array(
    '#type' => 'fieldset',
    '#title' => t('Gas Search'),
    '#tree' => TRUE,
    '#attributes' => array('class' => 'sensor-search-fieldset'),
  );
  
  $form['search']['gasName'] = array(
    '#type' => 'textfield',
    '#title' => t('Search'),
    '#size' => 50,
    '#maxlength' => 50,
    '#description' => t('Enter the empirical name for the gas.'),
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
//  NAME: sensormodule_gasSearchBar_validate
//  USE: Validation function for searchGasForm
//  DATE: 07/09/2020
//**************************************************************
function sensormodule_gasSearchBar_validate($form, &$form_state) {
  if($form_state['clicked_button']['#name'] == 'Search') {
    $searchTerm = $form_state['values']['search']['gasName'];
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
    drupal_goto("sensormodule/search/gas");
  }
}

//**************************************************************
//  NAME: sensormodule_gasSearchBar_submit
//  USE: Submit function for searchGasForm, redirects to result page
//  DATE: 07/09/2020
//**************************************************************
function sensormodule_gasSearchBar_submit($form, &$form_state) {
  $searchTerm = $form_state['values']['search']['gasName'];
  drupal_goto("sensormodule/search/gas/", "search=".$searchTerm);  
}


//**************************************************************
//  NAME: sensormodule_gasSearchResultsTable
//  USE: Performs DB query and produces a HTML table populated with results
//  DATE: 22/10/2020
//**************************************************************
function sensormodule_gasSearchResultsTable($form_state, $searchTerm='') {

  $limit = 25;
  db_set_active('gas_sensor_db');
  
  $header = array(
    array('data' => t('Empirical'), 'field' => 'Name'),
    array('data' => t('Fullname'), 'field' => 'Fullname'),
  );
  
  $tablesort = tablesort_sql($header);
  if($searchTerm != "") {
    $query = "SELECT Name, Fullname, GasID FROM Gas WHERE (Name LIKE '%".$searchTerm."%' OR Fullname LIKE '%".$searchTerm."%')";
    //$result = pager_query($query.$tablesort, $limit, 0, NULL, $searchTerm);
  } else {
    $query = "SELECT Name, Fullname, GasID FROM Gas";
  }
  $result = pager_query($query.$tablesort, $limit);
  
  $form['tableButton'] = array(
    '#theme' => 'gasSearchResultsTable_theme',
    '#tree' => TRUE,
  );
  
  $count = 0;
  while($item =db_fetch_object($result))
  {
    $key= $item->GasID;
    $form['tableButton'][$key]['Name'] = array( '#value' => $item->Name);
    $form['tableButton'][$key]['Fullname'] = array( '#value' =>$item->Fullname);
    $form['tableButton'][$key]['RelatedSensor'] = array (
      '#type' => 'submit', 
      '#value' => t('All Sensors'), 
      '#name' => $item->GasID,
      '#attributes' => array('class' => 'sensormodule-btn sensormodule-table-btn'),
    );
    $count++;
  }
  db_set_active('default');
  $form['tableButton']['pager'] = array('#value' => theme('pager', NULL, $limit, 0));
  
  //Display message if none found
  if($count == 0) {
    drupal_set_message("No gasses like: '".$searchTerm."' were found.");
    drupal_goto("sensormodule/search/gas");
  }
  return $form;
}

function sensormodule_gasSearchResultsTable_submit($form, &$form_state) {
  $gasID = $form_state['clicked_button']['#name'];
  $gasName = getGasName($gasID);

  if($gasName == "") {
    drupal_set_message("Error, could not find GasID ($gasID) in the database.", "error");
    drupal_goto("sensormodule/search/gas");
  }
  else {
    drupal_goto("sensormodule/search/sensor/", "searchTerm=$gasName&filter=gas");
  }
}

//**************************************************************
//  NAME: theme_searchSensorTableForm_theme
//  USE: Theme function for the table form above
//  DATE: 30/10/2020
//**************************************************************
function theme_gasSearchResultsTable_theme($form) {
  $header = array(
    #data
    array('data' => t('Empirical'), 'field' => 'Name', 'class' => 'sensormodule-sensor-table-header'),
    array('data' => t('Fullname'),'field' => 'Fullname', 'class' => 'sensormodule-sensor-table-header'),
    array('data' => t('List Of Sensors Which Detect'), 'class' => 'sensormodule-sensor-table-header'),
    theme('table_select_header_cell'),
  );
  $rows = array();
  foreach (element_children($form)as $index) {
    $row = array();
    $row[] = drupal_render($form[$index]['Name']);
    $row[] = drupal_render($form[$index]['Fullname']);
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

//==============================================================
// ADD
//==============================================================
//***************************************************************
//  NAME: sensormodule_addGasForm
//  USE: Loads gas's stored in the database
//  DATE: 21/10/2020
//***************************************************************
function sensormodule_addGasForm(&$form_state) {
  $form['addGas'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#title' => t('Add Gas'),
    '#attributes' => array('class' => 'form-fieldset'),
  );
  $form['addGas']['GasName'] = array(
    '#type' => 'textfield',
    '#title' => t('Empirical Formula'),
    '#maxlength' => 64,
    '#description' => t('eg: CO')
  );

  $form['addGas']['Fullname'] = array(
    '#type' => 'textfield',
    '#title' => t('Gas Name'),
    '#size' => 80,
    '#maxlength' => 84,
    '#description' => t('eg: Carbon Monoxide'),
  );

  $form['addGas']['Add'] = array(
   '#type' => 'submit',
   '#value' => t('Add'),
   '#name' => 'Add',
   '#attributes' => array('class' => 'sensor-search-btn sensormodule-btn')
  );

  $form['addGas']['Cancel'] = array(
   '#type' => 'submit',
   '#value' => t('Cancel'),
   '#name' => 'Cancel',
   '#attributes' => array('class' => 'sensor-search-btn sensormodule-btn')
  );
  return $form;
}

//***************************************************************
//  NAME: sensormodule_addGasForm_validate
//  USE: Performs necessary validation on add gas form
//  DATE: 21/10/2020
//***************************************************************
function sensormodule_addGasForm_validate($form, &$form_state) {
  $clicked_button = $form_state['clicked_button']['#name'];
  $gasName = $form_state['values']['addGas']['GasName'];
  $gasFullName = $form_state['values']['addGas']['Fullname'];
  if($clicked_button == "Cancel") {
    drupal_set_message("No changes made.");
    drupal_goto('sensormodule/edit/gas');
  }
  else if($clicked_button == "Add") {
    if(checkNull($gasName)) {
      form_error($form['addGas']['GasName'], "Name cannot be NULL.");
    }
    else if(!checkAlphanumeric($gasName)) {
      form_error($form['addGas']['GasName'], "Please enter only safe characters for the name.");
    }
    else if(getGasFullnameID($gasName) != -1) {
      if(getGasName($gasID) != $gasName) {
        form_error($form['addGas']['GasName'], "'$gasName' already exists in the database.");
      }
    }
    if(checkNull($gasFullName)) {
      form_error($form['addGas']['Fullname'], "Fullname cannot be NULL.");
    }
    else if(!checkAlphanumeric($gasFullName)) {
      form_error($form['addGas']['Fullname'], "Please enter only safe characters for the full name.");
    }
    else if(getGasFullnameID($gasFullName) != -1) {
      if(getGasFullname($gasID) != $gasFullName) {
        form_error($form['addGas']['Fullname'], "'$gasFullName' already exists in the database.");
      }
      
    }
  }
}

//***************************************************************
//  NAME: sensormodule_addGasForm_submit
//  USE: Performs necessary validation then adds Gas into Gas table in database
//  DATE: 21/10/2020
//***************************************************************
function sensormodule_addGasForm_submit($form, &$form_state)
{
  $gasName = $form_state['values']['addGas']['GasName'];
  $gasFullname = $form_state['values']['addGas']['Fullname'];
  // CHANGE FOR FULLNAME
  db_set_active('gas_sensor_db');
  db_query("INSERT INTO Gas (Name, Fullname) VALUES ('".$gasName."','".$gasFullname."');");
  db_set_active("default");

  drupal_set_message("Successfully added '$gasName' to the database.");
  drupal_goto('sensormodule/edit/gas');
}

//==============================================================
// EDIT
//==============================================================
//***************************************************************
//  NAME: sensormodule_modifyGasForm
//  USE: Loads the edit page for a gas's name
//  DATE: 21/10/2020
//***************************************************************
function sensormodule_modifyGasForm(&$form_state, $gasID){
  $name = getGasName($gasID);
  $fullname = getGasFullname($gasID);
  $form['modifyGasEdit'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#attributes' => array('class' => 'form-fieldset'),
  );

  $form['modifyGasEdit']['gasName'] = array(
    '#type' => 'textfield',
    '#title' => t('Gas Name'),
    '#default_value' => $name,
    '#size' => 60,
    '#maxlength' => 64,
    '#description' => t('Enter new gas name.'),
  );

  $form['modifyGasEdit']['Fullname'] = array(
    '#type' => 'textfield',
    '#title' => t('Gas Full Name'),
    '#default_value' => $fullname,
    '#size' => 60,
    '#maxlength' => 64,
    '#description' => t('Enter the full gas name.'),
  );

  $form['modifyGasEdit']['Save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#name' => 'Save',
    '#attributes' => array('class' => 'sensor-search-btn sensormodule-btn')
  );

  $form['modifyGasEdit']['Cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#name' => 'Cancel',
    '#attributes' => array('class' => 'sensor-search-btn sensormodule-btn')
  );
  
  $form['modifyGasEdit']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#name' => 'Delete',
    '#attributes' => array('class' => 'sensormodule-delete-btn sensormodule-btn')
  );

  $form['modifyGasEdit']['gasID'] = array(
    '#type' => 'hidden',
    '#value' => $gasID,
  );
  
  return $form;
}

//***************************************************************
//  NAME: sensormodule_modifyGasForm_validate
//  USE: Loads the edit page for a brand's name
//  DATE: 21/10/2020
//***************************************************************
function sensormodule_modifyGasForm_validate($form, &$form_state) {
  $clicked_button = $form_state['clicked_button']['#name'];
  $gasID = $form_state['values']['modifyGasEdit']['gasID'];
  $gasName = $form_state['values']['modifyGasEdit']['gasName'];
  $gasFullName = $form_state['values']['modifyGasEdit']['Fullname'];
  if($clicked_button == 'Delete'){
    drupal_goto('sensormodule/edit/gas/remove/', 'gasID='.$gasID);
  }
  else if($clicked_button == "Cancel") {
    drupal_set_message("No changes made.");
    drupal_goto('sensormodule/edit/gas');
  }
  else if($clicked_button == "Save") {
    if(checkNull($gasName)) {
      form_error($form['modifyGasEdit']['gasName'], "Name cannot be NULL.");
    }
    else if(!checkAlphanumeric($gasName)) {
      form_error($form['modifyGasEdit']['gasName'], "Please enter only safe characters.");
    }
    else if(getGasID($gasName) != -1) {
      if(getGasName($gasID) != $gasName) {
        form_error($form['modifyGasEdit']['gasName'], "'$gasName' already exists in the database.");
      }
    }
    if(checkNull($gasFullName)) {
      form_error($form['modifyGasEdit']['Fullname'], "Name cannot be NULL.");
    }
    else if(!checkAlphanumeric($gasFullName)) {
      form_error($form['modifyGasEdit']['Fullname'], "Please enter only safe characters.");
    }
    else if(getGasID($gasFullName) != -1) {
      if(getGasFullname($gasID) != $gasFullName) {
        form_error($form['modifyGasEdit']['Fullname'], "'$gasFullName' already exists in the database.");
      }
    }
  }
}


//***************************************************************
//  NAME: sensormodule_modifyGasForm_submit
//  USE: Saves the new name of the gas
//  DATE: 21/10/2020
//***************************************************************
function sensormodule_modifyGasForm_submit($form, &$form_state) {
  $newGasName = $form_state['values']['modifyGasEdit']['gasName'];
  $newGasFullname = $form_state['values']['modifyGasEdit']['Fullname'];
  $gasID = $form_state['values']['modifyGasEdit']['gasID'];

  db_set_active('gas_sensor_db');
  db_query("UPDATE Gas SET Name='%s' WHERE GasID='%s'", $newGasName, $gasID);
  db_query("UPDATE Gas SET Fullname='%s' WHERE GasID='%s'", $newGasFullname, $gasID);
  db_set_active('default');
  drupal_set_message("Saved changes.");
  drupal_goto('sensormodule/edit/gas');
}


//***************************************************************
//  NAME: sensormodule_editGasSearchBar
//  USE: Search bar for the edit gasses page
//  DATE: 21/10/2020
//***************************************************************
function sensormodule_editGasSearchBar(&$form_state) {
  $form['searchGasEdit'] = array(
    '#type' => 'fieldset',
    '#title' => t('Edit Gas'),
    '#tree' => TRUE,
    '#attributes' => array('class' => 'sensor-search-fieldset'),
  );

  $form['searchGasEdit']['searchTerm'] = array(
    '#type' => 'textfield',
    '#title' => t('Search'),
    '#size' => 60,
    '#maxlength' => 64,
    '#description' => t('Enter the desired gas name or ID.'),
    '#attributes' => array('class' => 'sensor-search-bar'),
  );

  $form['searchGasEdit']['editSearchBtn'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
    '#name' => 'Search',
    '#attributes' => array('class' => 'sensor-search-btn sensormodule-btn')
  );
  
  $form['searchGasEdit']['editShowAllBtn'] = array(
    '#type' => 'submit',
    '#value' => t('Show All'),
    '#name' => 'ShowAll',
    '#attributes' => array('class' => 'sensor-search-btn sensormodule-btn')
  );
  return $form;
}

//**************************************************************
//  NAME: searchEditGasForm_submit
//  USE: Submit function for searchSensorForm, redirects to result page
//  DATE: 16/09/2020
//**************************************************************
function sensormodule_editGasSearchBar_submit($form, &$form_state) {
  $searchTerm = $form_state['values']['searchGasEdit']['searchTerm'];
  drupal_goto("sensormodule/edit/gas/", "search=".$searchTerm);
}

//**************************************************************
//  NAME: searchEditSensorForm_validate
//  USE: Validation function for searchSensorForm
//  DATE: 16/09/2020
//**************************************************************
function sensormodule_editGasSearchBar_validate($form, &$form_state) {
  $clicked_button = $form_state['clicked_button']['#name'];
  $searchTerm = $form_state['values']['searchGasEdit']['searchTerm'];
  
  if($clicked_button == "ShowAll") {
    drupal_goto("sensormodule/edit/gas");
  }
  if($clicked_button == "Search") {
    if($searchTerm == "")
    {
      form_error($form['searchGasEdit']['searchTerm'], t('Please enter a valid search term.'));
    }
    else if(!checkAlphanumeric($searchTerm)) {
      form_error($form['searchGasEdit']['searchTerm'], 'Please enter only safe characters.');
    }
  }
}

//***************************************************************
//  NAME: sensormodule_editGasSearchResultsTable
//  USE: The form for the Edit Gas table 
//  DATE: 21/10/2020
//***************************************************************
function sensormodule_editGasSearchResultsTable($form_state, $searchTerm) {
  
  $limit = 25;
  db_set_active('gas_sensor_db');
  $header = array(
    array('data' => t('GasID'), 'field' => 'GasID'),
    array('data' => t('Empirical'), 'field' => 'Name'),
    array('data' => t('Fullname'), 'field' => 'Fullname'),
    array('Edit' => t('Edit'), 'field' => 'Edit'),
    array('Delete' => t('Delete'), 'field' => 'Delete'),
  );
  if($searchTerm!="") {
    $query = "SELECT GasID, Name, Fullname FROM Gas 
              WHERE (Name LIKE '%".$searchTerm."%' OR
                     GasID='".$searchTerm."' OR Fullname LIKE '%".$searchTerm."%')";
  }
  else {    
    $query = "SELECT GasID, Name, Fullname FROM Gas";
  }

  $tablesort = tablesort_sql($header);
  $result = pager_query($query.$tablesort, $limit);
  $form['tableGasButton'] = array(
    '#theme' => 'editGasSearchResultsTable_theme',
    '#tree' => TRUE,
  );

  // Search result count
  $count = 0;
  while($item =db_fetch_object($result))
  {
    $submitKey=0;
    $key= $item->GasID;  
    
    $form['tableGasButton'][$key]['GasID'] = array( '#value' => $item->GasID);
    $form['tableGasButton'][$key]['Name'] = array( '#value' => $item->Name);
    $form['tableGasButton'][$key]['Fullname'] = array('#value' => $item->Fullname);
    $form['tableGasButton'][$key]['Edit'] = array(
      '#type' => 'submit',
      '#value' => t('Edit'),
      '#name' => $item->GasID,
      '#submit' => array('sensormodule_editGasSearchResultsTable_edit'),
      '#attributes' => array('class' => 'sensormodule-table-btn sensormodule-btn'),
    );
    $form['tableGasButton'][$key]['Delete'] = array (
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#name' => $item->GasID,
      '#submit' => array('sensormodule_editGasSearchResultsTable_delete'),
      '#attributes' => array('class' => 'sensormodule-delete-btn sensormodule-table-btn'),
    );
    $count++;
  }
  db_set_active('default');
  $form['tableGasButton']['pager'] = array('#value' => theme('pager', NULL, $limit, 0));

  // Display message if none found
  if($count == 0) {
    drupal_set_message("No gas like: '".$searchTerm."' were found.");
    drupal_goto("sensormodule/edit/gas");
  }

  return $form;
}

//***************************************************************
//  NAME: theme_editGasSearchResultsTable_theme
//  USE: Applies a theme to the Edit Gas table
//  DATE: 21/10/2020
//***************************************************************
function theme_editGasSearchResultsTable_theme($form) {
  $header = array(
  array('data' => t('GasID'), 'field' => 'GasID'),
  array('data' => t('Empirical'), 'field' => 'Name'),
  array('data' => t('Fullname'), 'field' => 'Fullname'),
  array('Edit' => t('Edit'), 'class' => 'sensormodule-sensor-table-header'),
  array('Delete' => t('Delete'), 'class' => 'sensormodule-sensor-table-header'),
  theme('table_select_header_cell'),
  );
  $rows = array();
  foreach (element_children($form)as $index) {
    $row = array();
    $row[] = drupal_render($form[$index]['GasID']);
    $row[] = drupal_render($form[$index]['Name']);
    $row[] = drupal_render($form[$index]['Fullname']);
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
//  NAME: sensormodule_editGasPageForm_submit_delete
//  USE: Takes user to the gas deletion confirmation page
//  DATE: 21/10/2020
//***************************************************************
function sensormodule_editGasSearchResultsTable_delete($form, &$form_state) {
  $gasName = $form_state['clicked_button']['#name'];
  drupal_goto("sensormodule/edit/gas/remove/", "gasID=".$gasName);
}

//***************************************************************
//  NAME: sensormodule_editGasPageForm_submit_edit
//  USE: Takes user to the gas edit page
//  DATE: 21/10/2020
//***************************************************************
function sensormodule_editGasSearchResultsTable_edit($form, &$form_state){
  $gasID = $form_state['clicked_button']['#name'];
  drupal_goto("sensormodule/edit/gas/modify/", "gasID=".$gasID);
}


//==============================================================
// REMOVE
//==============================================================
//***************************************************************
//  NAME: sensormodule_removeGasConfirmation
//  USE: Confirmation page for a gas deletion
//  DATE: 21/10/2020
//***************************************************************
function sensormodule_removeGasConfirmation(&$form_state, $gasID="") {
  $form['deleteGas'] = array(
   '#type' => 'fieldset',
   '#title' => t('Are you sure?'),
   '#tree' => TRUE,
   '#attributes' => array('class' => 'delete-form-fieldset'),
  );

  $form['deleteGas']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#name' => 'Delete',
    '#attributes' => array(
      'onclick' => 'if(!confirm("Are you really sure?")) {return false;}',
      'class' => 'sensormodule-delete-btn sensormodule-btn'),
  );

  $form['deleteGas']['cancel'] = array(
    '#type' => 'submit',
    '#name' => 'Cancel',
    '#value' => t('Cancel'),
    '#attributes' => array('class' => 'sensor-search-btn sensormodule-btn')
  );

  $form['deleteGas']['gasID'] = array(
    '#type' => 'hidden',
    '#value' => $gasID,
  );
  return $form;
}

//***************************************************************
//  NAME: sensormodule_removeGasConfirmation_submit
//  USE: Removes the Gas from the Gas table in the database
//  DATE: 21/10/2020
//***************************************************************
function sensormodule_removeGasConfirmation_submit($form, &$form_state) {
  $clicked_button = $form_state['clicked_button']['#name'];
  $gasID = $form_state['values']['deleteGas']['gasID'];

  if($clicked_button == "Cancel") {
    drupal_set_message("No changes made.");
    drupal_goto("sensormodule/edit/gas");
  }
  else if($clicked_button == "Delete") {
    db_set_active('gas_sensor_db');
    db_query("DELETE FROM Gas WHERE GasID=".$gasID);
    db_set_active('default');
    drupal_set_message("Sucessfully removed");
    drupal_goto("sensormodule/edit/gas"); 
  }
}

?>
