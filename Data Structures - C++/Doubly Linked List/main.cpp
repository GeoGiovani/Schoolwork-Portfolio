#include "CharList.h"
#include <iostream>
#include <string>
using namespace std;

void failed(CharList* failedList, string correctList)
{
    cout << "failed" << endl;
    cout << "Your list:    " << failedList->toString() << endl;
    cout << "Correct list: " << correctList << endl;
    delete failedList;
}

int main() 
{
    //-------------CharList(string s), toString() with empty list--------------
    CharList* testList = new CharList("");
    if (testList->toString() != "") {
        failed(testList, "");
        return -1;
    }

    //---------------------------empty() when empty----------------------------
    if (testList->empty() != true) {
        failed(testList, "");
        return -2;
    }

    //---------------------------size() when empty-----------------------------
    if (testList->size() != 0) {
        failed(testList, "");
        return -3;
    }

    //------append(char c), insert(char c), toString() with 1, 2 el list-------
    testList->append('P');
    if (testList->toString() != "P") {
        failed(testList, "P");
        return -4;
    }
    testList->append('T');
    if (testList->toString() != "PT") {
        failed(testList, "PT");
        return -5;
    }
    testList->insert('M');
    testList->insert('a');

    //-------------append(char c, char d), insert(char c, char d)--------------
    testList->append('2', 'T'); //append after last element
    testList->append('i', 'a'); //append after first element
    testList->append('C', 'i'); //append in the middle
    testList->insert('2', '2'); //insert before last element
    testList->insert('v', 'a'); //insert before first element
    testList->insert('s', 'C'); //insert in the middle

    // insert at the end of the list
    testList->insert('3', 'L');
    testList->insert('0', 'L');  

    //-------------append(char c, char d), insert(char c, char d)--------------
    testList->append(' ', 'T', 1); //append after last element
    testList->append('n', 'a', 1); //append after first element
    testList->append(' ', 's', 1); //append in the middle
    testList->append('5', '2', 2); //append with n > 1
    testList->append('?', '2', 0); //append with n == 0
    testList->append('?', '2', -2); //append with n < 0

    testList->insert(' ', '3', 1); //insert before last element
    testList->insert('o', 'v', 1); //insert before first element
    testList->insert('[', 'C', 1); //insert in the middle
    testList->insert(']', ' ', 3); //insert with n > 1
    testList->insert('?', '2', 0); //insert with n == 0
    testList->insert('?', '2', -1); //insert with n < 1

    // insert at the end of the list
    testList->insert('1', 'L', 1); //insert at end when char isn't in list
    testList->insert('2', '2', 4); //insert at end when n is too large
    testList->append('5', 'L', 1); //insert at end when char isn't in list
    testList->append('4', '2', 3); //insert at end when n is too large
    testList->insert('6', '2', 4);
    testList->append('9', '2', 4);

    // see if append(char c) and insert(char c) work after all that
    testList->insert('i');
    testList->insert('G');
    testList->append('4');

    //------------------remove(char c), remove(char c, int n)------------------
    testList->remove('['); //remove from middle of list
    testList->remove(']'); //remove from middle of list

    testList->insert('G');
    testList->append('7');

    testList->remove('G'); //remove from front of list
    testList->remove('7'); //remove from end of list

    testList->remove(' ', 2); //remove from front of list
    
    testList->insert('G');
    testList->append('7');

    testList->remove('G', 1); //remove from front of list
    testList->remove('7', 1); //remove from end of list
    
    testList->append('2');

    testList->remove('2', 4); //remove with n > 1
    testList->remove('2', 0); //remove with n == 0
    testList->remove('2', -1); //remove with n < 0

    //----------------------------empty() when full----------------------------
    if (testList->empty() != false) {
        failed(testList, "Giovanis CMPT225 301245694");
        return -6;
    }

    //----------------------------size() when full-----------------------------
    if (testList->size() != 26) {
        failed(testList, "Giovanis CMPT225 301245694");
        return -7;
    }

    //-----------------------------success check-------------------------------
    if (testList->toString() != "Giovanis CMPT225 301245694") {
        failed(testList, "Giovanis CMPT225 301245694");
        return -8;
    }
    else {
        delete testList;
        cout << "passed" << endl;
        return 1;
    }
}