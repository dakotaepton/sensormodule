<?php

//**************************************************************
//  NAME: run_tests()
//  USE: Runs all the test functions and returns the results
//  DATE: 2/10/2020
//**************************************************************
function run_tests() {
    $results = "";
    $results .= "<br><hr style='background-color:black;height:3px;'/>";
    $results .= "<div style='text-align:center;'><br><h1>TEST RESULTS</h1></div>";
    $results .= "<br><hr style='background-color:black;height:3px;'/>";
    $results .= test_checkAlphanumeric();
    $results .= test_checkDouble();
    $results .= test_checkDateString();
    $results .= test_checkInt();
    $results .= test_userAccess();
    $results .= test_adminAccess();
    return $results;
}

//**************************************************************
//  NAME: test_checkAlphanumeric()
//  USE: Runs all test cases on the alphanumeric validation function
//        returns the resutlts
//  DATE: 2/10/2020
//**************************************************************
function test_checkAlphanumeric() {
    $func = "checkAlphanumeric";

    //CASE #1 - VALID
    $caseNum = 1;
    $test_desc[$caseNum] = "Valid Input";
    $inputs[$caseNum] = "MH-213";
    $expected[$caseNum] = TRUE;
            
    //CASE #2 - INVALID
    $caseNum++;
    $test_desc[$caseNum] = "Invalid Input";
    $inputs[$caseNum] = "___scaryInput!!@@";
    $expected[$caseNum] = FALSE;

    //CASE #3 - NO INPUT
    $caseNum++;
    $test_desc[$caseNum] = "No Input";
    $inputs[$caseNum] = "";
    $expected[$caseNum] = TRUE;
    
    $num_tests = $caseNum;

    $results = boolTestMaker($func, $num_tests, $test_desc, $inputs, $expected);

    return $results;
}

//**************************************************************
//  NAME: test_checkDateString()
//  USE: Runs all the test cases on the checkDateString validation function
//        and returns the results
//  DATE: 2/10/2020
//**************************************************************
function test_checkDateString() {
    $func = "checkDateString";

    //CASE #1 - VALID
    $caseNum = 1;
    $test_desc[$caseNum] = "Valid Input";
    $inputs[$caseNum] = "2020-01-20";
    $expected[$caseNum] = TRUE;
            
    //CASE #2 - DIFFERENT FORMAT BUT VALID DATE
    $caseNum++;
    $test_desc[$caseNum] = "Different format but valid date";
    $inputs[$caseNum] = "20-02-2011";
    $expected[$caseNum] = FALSE;

    //CASE #3 - ONLY NUMBERS AND DASHES BUT NOT A DATE
    $caseNum++;
    $test_desc[$caseNum] = "Legal date characters but not a date";
    $inputs[$caseNum] = "233-233-1111";
    $expected[$caseNum] = FALSE;

    //CASE #4 - ONLY LETTERS
    $caseNum++;
    $test_desc[$caseNum] = "Mix of letters, numbers and characters";
    $inputs[$caseNum] = "23e.34as???!!";
    $expected[$caseNum] = FALSE;

    //CASE #5 - NO INPUT
    $caseNum++;
    $test_desc[$caseNum] = "No input";
    $inputs[$caseNum] = "";
    $expected[$caseNum] = FALSE;

    $num_tests = $caseNum;

    $results = boolTestMaker($func, $num_tests, $test_desc, $inputs, $expected);

    return $results;
}

//**************************************************************
//  NAME: test_checkDouble()
//  USE: Runs all the test cases on the checkDouble validation function
//       and returns the results
//  DATE: 2/10/2020
//**************************************************************
function test_checkDouble() {
    $func = "checkDouble";

    //CASE #1 - VALID
    $caseNum = 1;
    $test_desc[$caseNum] = "Valid Input";
    $inputs[$caseNum] = "23.01";
    $expected[$caseNum] = TRUE;
            
    //CASE #2 - LETTERS ONLY
    $caseNum++;
    $test_desc[$caseNum] = "Letters only";
    $inputs[$caseNum] = "Notadouble.com";
    $expected[$caseNum] = FALSE;

    //CASE #3 - INTEGER
    $caseNum++;
    $test_desc[$caseNum] = "Integer value, but can be parsed to double";
    $inputs[$caseNum] = "23";
    $expected[$caseNum] = TRUE;

    //CASE #4 - LETTERS AND NUMBERS
    $caseNum++;
    $test_desc[$caseNum] = "Mix of letters and numbers";
    $inputs[$caseNum] = "23e.34as";
    $expected[$caseNum] = FALSE;

    //CASE #5 - NEGATIVE DOULBE
    $caseNum++;
    $test_desc[$caseNum] = "Negative double value";
    $inputs[$caseNum] = "-23.33";
    $expected[$caseNum] = TRUE;

    //CASE #6 - NO INPUT
    $caseNum++;
    $test_desc[$caseNum] = "No input";
    $inputs[$caseNum] = "";
    $expected[$caseNum] = FALSE;

    $num_tests = $caseNum;

    $results = boolTestMaker($func, $num_tests, $test_desc, $inputs, $expected);

    return $results;
}

//**************************************************************
//  NAME: test_checkInt()
//  USE: Runs all the test cases on the checkInt validation function
//       and returns the results
//  DATE: 2/10/2020
//**************************************************************
function test_checkInt() {
    $func = "checkInt";

    //CASE #1 - VALID
    $caseNum = 1;
    $test_desc[$caseNum] = "Valid Input";
    $inputs[$caseNum] = "23";
    $expected[$caseNum] = TRUE;
            
    //CASE #2 - LETTERS ONLY
    $caseNum++;
    $test_desc[$caseNum] = "Letters only";
    $inputs[$caseNum] = "Notaint.com";
    $expected[$caseNum] = FALSE;

    //CASE #3 - DOUBLE THAT CAN BE CAST TO INT
    $caseNum++;
    $test_desc[$caseNum] = "Double value, but can be parsed to int";
    $inputs[$caseNum] = "23.0";
    $expected[$caseNum] = FALSE;

    //CASE #4 - LETTERS AND NUMBERS
    $caseNum++;
    $test_desc[$caseNum] = "Mix of letters and numbers";
    $inputs[$caseNum] = "23e.34as";
    $expected[$caseNum] = FALSE;

    //CASE #5 - NEGATIVE INTEGER
    $caseNum++;
    $test_desc[$caseNum] = "Negative integer value";
    $inputs[$caseNum] = "-34";
    $expected[$caseNum] = TRUE;

    //CASE #6 - NO INPUT
    $caseNum++;
    $test_desc[$caseNum] = "No input";
    $inputs[$caseNum] = "";
    $expected[$caseNum] = FALSE;

    $num_tests = $caseNum;

    $results = boolTestMaker($func, $num_tests, $test_desc, $inputs, $expected);

    return $results;
}

//**************************************************************
//  NAME: testTableMaker()
//  USE:  Nicely formats test results in a table and returns the 
//         HTML for it
//  DATE: 2/10/2020
//**************************************************************
function testTableMaker($func_name, $rows, $num_tests, $num_success) {
    $html = 
          "<table style='width:100%;'>
            <tr><th colspan=4 style='text-align: center;'>".$func_name."</th></tr>";
    $html .= $rows;
    $html .= "<tr><th colspan=4 style='text-align: center;'>RESULTS: ".$num_success."/".$num_tests." WERE SUCCESSFUL.</th></tr>";
    $html .= "</table>";
    $html .= "<hr style='background-color:black;height:3px;'/>";
    
    return $html;
}

//**************************************************************
//  NAME: testTableRowMaker()
//  USE: Creates the HTML for test result table rows
//  DATE: 2/10/2020
//**************************************************************
function testTableRowMaker($caseNum, $test_desc, $input, $expected, $output, $result) {
    $rows = 
        "<tr><th>CASE #".$caseNum.":</th><th colspan=3 style='color: blue;'>".$test_desc."</th></tr>
         <tr><th>INPUT</th><th>EXPECTED</th><th>OUTPUT</th><th>RESULT</th></tr>";
    if($result == "SUCCESS") {
        $rows .= 
        "<tr><td>".$input."</td><td>".$expected."</td><td>".$output."</td><td style='color:green;font-weight:bold;'>".$result."</td></tr>";
    }
    else {
        $rows .= 
        "<tr><td>".$input."</td><td>".$expected."</td><td>".$output."</td><td style='color:red;font-weight:bold;'>".$result."</td></tr>";
    }
    return $rows;
}

//**************************************************************
//  NAME: testUserAccess()
//  USE: Runs the given tests on the the given function, the function
//        must return boolean though
//  DATE: 20/10/2020
//**************************************************************
function test_userAccess()
{
    $func="getUserAccess";

    #case 1
    $caseNum = 1;
    $test_desc[$caseNum] = "User can access search";
    $inputs[$caseNum] = "access sensormodule content";
    $expected[$caseNum] = TRUE;

    $caseNum++;
    $test_desc[$caseNum] = "User can access edit";
    $inputs[$caseNum] = "edit sensormodule content";
    $expected[$caseNum] = FALSE;


   

    $num_tests = $caseNum;
    $results = boolTestMaker($func, $num_tests, $test_desc, $inputs, $expected);
    return $results;
}
//**************************************************************
//  NAME: getUserAccess()
//  USE: Passes the user access function, the function
//        must return boolean though
//  DATE: 20/10/2020
//**************************************************************
function getUserAccess($role='')
{
    $account = user_load(array('name'=>'Regular User'));
   
    $allowed = user_access($role,$account);
    
    return $allowed;
}

//**************************************************************
//  NAME: testUserAccess()
//  USE: Runs the given tests on the the given function, the function
//        must return boolean though
//  DATE: 20/10/2020
//**************************************************************
function test_adminAccess()
{
    $func="getAdminAccess";

    #case 1
    $caseNum=1;
    $test_desc[$caseNum] = "Admin can access search";
    $inputs[$caseNum] = "access sensormodule content";
    $expected[$caseNum] = TRUE;

    $caseNum++;
    $test_desc[$caseNum] = "Admin can access edit";
    $inputs[$caseNum] = "edit sensormodule content";
    $expected[$caseNum] = TRUE;

    $num_tests = $caseNum;
    $results = boolTestMaker($func, $num_tests, $test_desc, $inputs, $expected);
    return $results;
}
//**************************************************************
//  NAME: getUserAccess()
//  USE: Passes the user access function, the function
//        must return boolean though
//  DATE: 20/10/2020
//**************************************************************
function getAdminAccess($role='')
{
    $account = user_load(array('name'=>'Root'));
    $allowed = user_access($role,$account);
    return $allowed;
}
//**************************************************************
//  NAME: boolTestMaker()
//  USE: Runs the given tests on the the given function, the function
//        must return boolean though
//  DATE: 2/10/2020
//**************************************************************
function boolTestMaker($func, $num_tests, $test_desc, $inputs, $expected) {
    $num_success = 0;
    for($i=1; $i<=$num_tests; $i++) {
        
        if($func($inputs[$i]) && $expected[$i]) {
            $output = "TRUE";
            $result = "SUCCESS";
            $num_success++;
        }
        else if(!$func($inputs[$i]) && !$expected[$i]) {
            $output = "FALSE";
            $result = "SUCCESS";
            $num_success++;
        }
        else {
            $result = "FAILED";
            if($expected[$i]) {
                $output = "FALSE";
            }
            else {
                $output = "TRUE";
            }
        }
        if($expected[$i]) { $expected[$i] = "TRUE"; }
        else { $expected[$i] = "FALSE"; }

        $rows .= testTableRowMaker($i, $test_desc[$i], $inputs[$i], $expected[$i],$output, $result);
    }
    $table = testTableMaker($func."()", $rows, $num_tests, $num_success);
    return $table;
}


?>