#include "CharList.h"
#include <iostream>
#include <string>
using namespace std;

// CharNode constructor. Initializes with character c.
CharNode::CharNode(char c)
    : data(c),
      next(NULL),
      prev(NULL) { }

// CharNode destructor.
CharNode::~CharNode() { }

// True if the list is empty 
bool CharList::empty() 
{ 
    return this->header->next == this->trailer; 
}

// Return the number of elements in the list     
int CharList::size() 
{
    return this->len;
}

// Inserts the character c at the front of the list
void CharList::insert(char c) 
{ 
    insert_after(this->header, c); 
}

// Inserts the character c at the back of the list
void CharList::append(char c) 
{ 
    insert_before(this->trailer, c); 
}

// Inserts c immediately before the first occurance of d, or
// at the end of the list if d does not occur.
void CharList::insert(char c, char d)
{
    insert(c, d, 1);
}

// Tnserts c immediately after the first occurance of d, or
// at the end of the list if d does not occur.
void CharList::append(char c, char d)
{
    append(c, d, 1);
} 

// Inserts c immediately before the nth occurance of d, or
// at the end of the list if d does not occur n times.
void CharList::insert(char c, char d, int n)
{
    // check if n is a valid number
    if (n <= 0) {
        return;
    }
    // if empty, insert at end of list
    if (empty()) {
        insert_before(this->trailer, c);
        return;
    }
    // Traverse list until the nth occurrence of c or the trailer is reached.
    // Insert before the last node encountered in the while loop.
    CharNode* ind = this->header;
    while (ind != this->trailer && n != 0) {
        ind = ind->next;
        if (ind->data == d) n--;
    }
    insert_before(ind, c);
}

// Inserts c immediately after the nth occurance of d, or
// at the end of the list if d does not occur n times.
void CharList::append(char c, char d, int n)
{
    // check if n is a valid number
    if (n <= 0) {
        return;
    }
    // return if empty
    if (empty()) {
        insert_before(this->trailer, c);
        return;
    }
    // Traverse list until the nth occurrence of c or the trailer is reached.
    // If the last node encountered in the while loop is the trailer then
    // insert before it. Otherwise, insert after it.
    CharNode* ind = this->header;
    while (ind != this->trailer && n != 0) {
        ind = ind->next;
        if (ind->data == d) n--;
    }
    if (ind == this->trailer) {
        insert_before(ind, c);
    }
    else insert_after(ind, c);
}

// Delete the first occurance of character c. Do nothing if
// c does not occur.
void CharList::remove(char c)
{
    remove(c, 1);
} 

// Delete the nth occurance of c. Do nothing if c does not
// occur n times.
void CharList::remove(char c, int n)
{
    // if empty, insert at end of list
    if (empty()) {
        return;
    }
    // Traverse list until the nth occurrence of c or the trailer is reached.
    // If the last node encountered in the while loop is the trailer then do
    // nothing. Otherwise, delete the node with the nth occurrence of c.
    CharNode* ind = this->header;
    while (ind != this->trailer && n != 0) {
        ind = ind->next;
        if (ind->data == c) n--;
    }
    if (ind != this->trailer) remove_node(ind);
} 

// Return the list as a string.
string CharList::toString()
{
    // if empty, return an empty string
    if (empty()) {
        return "";
    }
    if (this->header == NULL || this->trailer == NULL) {
        return "";
    }

    // else, move through the list node-by-node, appending each char to a 
    // growing string. When the trailer is reached, return the finished string. 
    string listString = "";
    CharNode* ind = this->header;
    while (ind->next != this->trailer) {
        ind = ind->next;
        listString += ind->data;
    }
    return listString;
}

// CharList constructor. Initializes the list to the contents of the string.
CharList::CharList(string s)
: header(new CharNode('H')),
  trailer(new CharNode('T')),
  len(0)
{
    // point header and trailer at each other. This will signify an empty list.
    this->header->next = this->trailer;
    this->trailer->prev = this->header;

    // if a non-empty string was entered into the constructor, insert new nodes
    // one at a time before the trailer to maintain proper ordering.
    for (int i = 0; i < s.length(); i++) insert_before(trailer, s[i]);
}

// CharList destructor. Returns all memory in the list.
CharList::~CharList()
{
    // traverse the list and deleve every node except header and trailer
    while (!empty()) {
        CharNode* del = this->header->next;
        this->header->next = del->next;
        delete del;
    }
    // delete header and trailer
    delete this->header; 
    delete this->trailer;
} 

// Inserts c immediately after node ind
void CharList::insert_after(CharNode* ind, char c)
{
    CharNode* insertNode = new CharNode(c);
    CharNode* nextNode = ind->next;
    // initialize the node that will be inserted
    insertNode->next = nextNode;
    insertNode->prev = ind;
    // update the pointers of the nodes positioned before and after the new node
    ind->next = insertNode;
    nextNode->prev = insertNode;
    this->len++;
}

// inserts d immediately before ind
void CharList::insert_before(CharNode* ind, char d)
{
    CharNode* insertNode = new CharNode(d);
    CharNode* beforeNode = ind->prev;
    // initialize the node that will be inserted
    insertNode->next = ind;
    insertNode->prev = beforeNode;
    // update the pointers of the nodes positioned before and after the new node
    ind->prev = insertNode;
    beforeNode->next = insertNode;
    this->len++;
}

// Link the elements before and after ind together,
// then delete ind 
void CharList::remove_node(CharNode* ind) 
{
    if (ind == this->header || ind == this->trailer) {
        return;
    }
    CharNode* beforeNode = ind->prev;
    CharNode* afterNode = ind->next;
    beforeNode->next = afterNode;
    afterNode->prev = beforeNode;
    delete ind;
    this->len--;
}