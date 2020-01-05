//Grab Weights
const row1_weight = document.querySelector('#weight-1');
const row2_weight = document.querySelector('#weight-2');
const row3_weight = document.querySelector('#weight-3');
const row4_weight = document.querySelector('#weight-4');

//Grab Grade Inputs
const row1_num = document.querySelector('#Grade-1-num');
const row1_den = document.querySelector('#Grade-1-den');
const row2_num = document.querySelector('#Grade-2-num');
const row2_den = document.querySelector('#Grade-2-den');
const row3_num = document.querySelector('#Grade-3-num');
const row3_den = document.querySelector('#Grade-3-den');
const row4_num = document.querySelector('#Grade-4-num');
const row4_den = document.querySelector('#Grade-4-den');

//Grab outputs
const output1 = document.querySelector('#output1');
const output2 = document.querySelector('#output2');
const output3 = document.querySelector('#output3');
const output4 = document.querySelector('#output4');
const output5 = document.querySelector('#resultOutput');

//Grab Buttons
const weighted_button = document.querySelector(".leftButton");
const mean_button = document.querySelector(".rightButton");
const hidden_button = document.querySelector(".hiddenButton");

//Global function variables
var fn_condition = 0;
var grade1 = 0;
var grade2 = 0;
var grade3 = 0;
var grade4 = 0;
var result = 0;

//Input event listeners
row1_num.addEventListener('input', update_single_grades);
row1_den.addEventListener('input', update_single_grades);
row2_num.addEventListener('input', update_single_grades);
row2_den.addEventListener('input', update_single_grades);
row3_num.addEventListener('input', update_single_grades);
row3_den.addEventListener('input', update_single_grades);
row4_num.addEventListener('input', update_single_grades);
row4_den.addEventListener('input', update_single_grades);

//Button event listeners
weighted_button.addEventListener('click', calculate_weighted);
mean_button.addEventListener('click', calculate_mean);

//Calculate and display the weighted average
function calculate_weighted(e) {
    e.preventDefault();
    //Check that all inputs are entered
    if (!check_grade_entries() || !check_weighted_entries()) {
        alert("To calculate weighted average, input numerals into each field");
        return;
    }
    //Check that all inputs are numeric
    if (!check_weighted_numeric() || !check_grades_numeric()) {
        alert("Please enter only numeric values into input fields");
        return;
    }
    //Check for division by zero
    if (!check_nonzero_denominators()) {
        alert("Please ensure grade denominators are nonzero");
        return;     
    }

    //Calculate grades
    grade1 = Number(row1_num.value) / Number(row1_den.value) * 100;
    grade2 = Number(row2_num.value) / Number(row2_den.value) * 100;
    grade3 = Number(row3_num.value) / Number(row3_den.value) * 100;
    grade4 = Number(row4_num.value) / Number(row4_den.value) * 100;
    
    //Calculate weights
    let weight1 = grade1 * Number(row1_weight.value);
    let weight2 = grade2 * Number(row2_weight.value);
    let weight3 = grade3 * Number(row3_weight.value);
    let weight4 = grade4 * Number(row4_weight.value);

    //Calculate weighted mean
    let sum_weights = Number(row1_weight.value) + Number(row2_weight.value) +
    Number(row3_weight.value) + Number(row4_weight.value);
    result = (weight1 + weight2 + weight3 + weight4) / sum_weights;

    //Output to fields
    output1.innerHTML = grade1.toFixed(2) + ' %';
    output2.innerHTML = grade2.toFixed(2) + ' %';
    output3.innerHTML = grade3.toFixed(2) + ' %';
    output4.innerHTML = grade4.toFixed(2) + ' %';
    output5.innerHTML = result.toFixed(2) + ' %';

    //Change color of text
    change_element_color(output1, grade1);
    change_element_color(output2, grade2);
    change_element_color(output3, grade3);
    change_element_color(output4, grade4);
    change_element_color(output5, result);

    //Display congratulations gif for good mark
    if (result >= 85) {
        document.getElementById("highScore").style.display = "inline";
    }
    else if (result < 85) {
        document.getElementById("highScore").style.display = "none";
    }

    //Set print function condition and reveal the hidden button
    fn_condition = 1;
    reveal_button();
}

//Calculate and display the mean of the four grades
function calculate_mean(e) {
    e.preventDefault();
    //Check that all grade inputs are entered
    if (!check_grade_entries()) {
        alert("To calculate mean, input numerals into each grade field");
        return;
    }
    //Check that all grade inputs are numeric
    if (!check_grades_numeric()) {
        alert("Please enter only numeric values into input fields");
        return;
    }
    //Check for division by zero
    if (!check_nonzero_denominators()) {
        alert("Please ensure grade denominators are nonzero");
        return;     
    }

    //Calculate grades
    grade1 = Number(row1_num.value) / Number(row1_den.value) * 100;
    grade2 = Number(row2_num.value) / Number(row2_den.value) * 100;
    grade3 = Number(row3_num.value) / Number(row3_den.value) * 100;
    grade4 = Number(row4_num.value) / Number(row4_den.value) * 100;

    //Calculate mean
    result = (grade1 + grade2 + grade3 + grade4) / 4;

    //Output to fields
    output1.innerHTML = grade1.toFixed(2) + ' %';
    output2.innerHTML = grade2.toFixed(2) + ' %';
    output3.innerHTML = grade3.toFixed(2) + ' %';
    output4.innerHTML = grade4.toFixed(2) + ' %';
    output5.innerHTML = result.toFixed(2) + ' %';

    //Change color of text
    change_element_color(output1, grade1);
    change_element_color(output2, grade2);
    change_element_color(output3, grade3);
    change_element_color(output4, grade4);
    change_element_color(output5, result);

    //Display congratulations gif for good mark
    if (result >= 85) {
        document.getElementById("highScore").style.display = "inline";
    }
    else if (result < 85) {
        document.getElementById("highScore").style.display = "none";
    }
    
    //Set print function condition and reveal the hidden button
    fn_condition = 0;
    reveal_button();
}

//Updates a single grade as the user types into the input field
function update_single_grades(e) {

    //Determine which element is being filled and update accordingly
    //Grade 1
    if(e.target == row1_num || e.target == row1_den) {
        //Error check
        if(isNaN(row1_num.value) || row1_num.value === '' || 
                isNaN(row1_den.value) || row1_den.value === '' || 
                row1_den.value[0] == 0) {
            output1.innerHTML = "NaN";
            change_element_color(output1, 0);
            return;
        }
        else {
            grade1 = Number(row1_num.value) / Number(row1_den.value) * 100;
            output1.innerHTML = grade1.toFixed(2) + ' %';
            change_element_color(output1, grade1);
        }
    }
    //Grade 2
    if(e.target == row2_num || e.target == row2_den) {
        //Error check
        if(isNaN(row2_num.value) || row2_num.value === '' || 
                isNaN(row2_den.value) || row2_den.value === '' || 
                row2_den.value[0] == 0) {
            output2.innerHTML = "NaN";
            change_element_color(output2, 0);
            return;
        }
        else {
            grade2 = Number(row2_num.value) / Number(row2_den.value) * 100;
            output2.innerHTML = grade2.toFixed(2) + ' %';
            change_element_color(output2, grade2);
            return;
        }
    }
    //Grade 3
    if(e.target == row3_num || e.target == row3_den) {
        //Error check
        if(isNaN(row3_num.value) || row3_num.value === '' || 
                isNaN(row3_den.value) || row3_den.value === '' || 
                row3_den.value[0] == 0) {
            output3.innerHTML = "NaN";
            change_element_color(output3, 0);
            return;
        }
        else {
            grade3 = Number(row3_num.value) / Number(row3_den.value) * 100;
            output3.innerHTML = grade3.toFixed(2) + ' %';
            change_element_color(output3, grade3);
            return;
        }
    }
    //Grade 4
    if(e.target == row4_num || e.target == row4_den) {
        //Error check
        if(isNaN(row4_num.value) || row4_num.value === '' || 
                isNaN(row4_den.value) || row4_den.value === '' || 
                row4_den.value[0] == 0) {
            output4.innerHTML = "NaN";
            change_element_color(output4, 0);
            return;
        }
        else {
            grade4 = Number(row4_num.value) / Number(row4_den.value) * 100;
            output4.innerHTML = grade4.toFixed(2) + ' %';
            change_element_color(output4, grade4);
            return;
        }
    }
}

//Generates a copy/paste report of the grades
function print_window(grade1, grade2, grade3, grade4, result, fn_condition) {

    //Create a new window
    let myWindow = window.open("", "MsgWindow", "width=300,height=300");
    myWindow.document.write("<meta charset='UTF-8'></meta>");
    myWindow.document.write("<html><title>Grades Report</title></html>");

    //Set document line height
    myWindow.document.querySelector('body').style.lineHeight = '10px';
    myWindow.document.querySelector('body').style.fontSize = '12';

    //Report will not include weight values if mean button was clicked
    if (fn_condition === 1) {
        myWindow.document.write("<br><p id='Weight1'></p><p id='Grade1'></p>");
        myWindow.document.write("<p id='Weight2'></p><p id='Grade2'></p>");
        myWindow.document.write("<p id='Weight3'></p><p id='Grade3'></p>");
        myWindow.document.write("<p id='Weight4'></p><p id='Grade4'></p>");
        myWindow.document.write("<br><p id='Final'></p><br>");
    }
    else if (fn_condition === 0) {
        myWindow.document.write("<br><p id='Grade1'></p>");
        myWindow.document.write("<p id='Grade2'></p>");
        myWindow.document.write("<p id='Grade3'></p>");
        myWindow.document.write("<p id='Grade4'></p>");
        myWindow.document.write("<br><p id='Final'></p><br>");
    }
    
    //Add grade data
    //Only include weights if weight button clicked
    if (fn_condition === 1) {
        myWindow.document.getElementById('Weight1').innerHTML = 
        "[A1] Weight = " + row1_weight.value;
        myWindow.document.getElementById('Weight2').innerHTML = 
        "[A2] Weight = " + row2_weight.value;
        myWindow.document.getElementById('Weight3').innerHTML = 
        "[A3] Weight = " + row3_weight.value;
        myWindow.document.getElementById('Weight4').innerHTML = 
        "[A4] Weight = " + row4_weight.value;
    }

    myWindow.document.getElementById('Grade1').innerHTML = 
    "[A1] Grade = " + grade1.toFixed(2) + ' %';
    myWindow.document.getElementById('Grade2').innerHTML = 
    "[A2] Grade = " + grade2.toFixed(2) + ' %';
    myWindow.document.getElementById('Grade3').innerHTML = 
    "[A3] Grade = " + grade3.toFixed(2) + ' %';
    myWindow.document.getElementById('Grade4').innerHTML = 
    "[A4] Grade = " + grade4.toFixed(2) + ' %';
    myWindow.document.getElementById('Final').innerHTML = 
    "[RESULT] " + result.toFixed(2) + ' %';

    //Format final result to be bolded
    myWindow.document.getElementById('Final').style.fontWeight = "bold";

    //Add timestamp
    let today = new Date();
    let date = today.getFullYear() + '-' + (today.getMonth()+1) +
    '-' +today.getDate();
    let time = today.getHours() + ":" + today.getMinutes() + ":" + 
    today.getSeconds();
    myWindow.document.write("<p id='Msg'></p>");
    myWindow.document.getElementById('Msg').innerHTML = 
    "Generated on " + date + " at " + time + ".";

    //Format timestamp
    myWindow.document.getElementById('Msg').style.fontStyle = "italic";
}

//print_window helper
function print_window_helper(e) {
    e.preventDefault();
    print_window(grade1, grade2, grade3, grade4, result, fn_condition);
}

//Changes the color of text depending on the entered percent
function change_element_color(element, percent) {
    if (percent < 50) {
        element.style.color = "red";
    }
    else if (percent < 75) {
        element.style.color = "orange";
    }
    else {
        element.style.color = "green";
    }
    element.style.fontWeight = "bold";
}

//Reveals and adds attributes to the hidden print button
function reveal_button() {
    hidden_button.addEventListener('click', print_window_helper); 
    hidden_button.setAttribute("class", "rightButton");
}

//Returns true if all weighted inputs have entries
function check_weighted_entries() {
    if (row1_weight.value === '' || row2_weight.value === '' || 
            row3_weight.value === '' || row4_weight.value === '') {
        return false;
    }
    return true;
}

//Returns true if all grade inputs have entries
function check_grade_entries() {
    if (row1_num.value === '' || row1_den.value === '' || 
            row2_num.value === '' || row2_den.value === '' ||
            row3_num.value === '' || row3_den.value === '' ||
            row4_num.value === '' || row4_den.value === '') {
        return false;
    }
    return true;
}

//Returns true if all weighted inputs are numeric
function check_weighted_numeric() {
    if (isNaN(row1_weight.value) || isNaN(row2_weight.value) ||
            isNaN(row3_weight.value) || isNaN(row4_weight.value)) {
        return false;
    }
    return true;
}

//Returns true if all grade inputs are numeric
function check_grades_numeric() {
    if (isNaN(row1_num.value) || isNaN(row1_den.value) ||
            isNaN(row2_num.value) || isNaN(row2_den.value) ||
            isNaN(row3_num.value) || isNaN(row3_den.value) ||
            isNaN(row4_num.value) || isNaN(row4_den.value)) {
        return false;
    }
    return true;
}

//Returns true if all grade denominators are nonzero
function check_nonzero_denominators() {
    if (row1_den.value == 0 || row2_den.value == 0 ||
            row3_den.value == 0 || row4_den.value == 0) {
        return false;
    }
    return true;
}

console.log("Developed by George Giovanis in Vancouver")