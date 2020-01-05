//=============================================================================
// Name        : Customer.cpp 
//                                                   George Giovanis, 301254694
//=============================================================================



#include "Customer.h"
#include <string>
using namespace std;


//======================== Protected Utility Functions ========================

// Set the Customer's account balance to new_bal.
void Customer::set_balance(const int new_bal) 
{ 
    this->account_balance = new_bal; 
}


// Return Customer's last name 
string Customer::get_surname() const 
{ 
    return this->surname; 
}               


// Return the initial of the Customer's first name.
char Customer::get_first_initial() const 
{ 
    return this->first_initial; 
}


// Return the Customer's account balance.
int Customer::get_account_balance() const 
{ 
    return this->account_balance; 
}


//============================ Public Functions ===============================

// Default Constructor
Customer::Customer() 

    : surname("EMPTY"),
      first_initial(' '),
      account_balance(0) { }


// Constructor with supplied Customer member values
Customer::Customer(const string last_name, const char initial, const int bal) 

    : surname(last_name),
      first_initial(initial),
      account_balance(bal) { }


// Copy Constructor
// Creates a deep copy of the supplied Customer
Customer::Customer(const Customer& c)

    : surname(c.get_surname()),
      first_initial(c.get_first_initial()),
      account_balance(c.get_account_balance()) { }


// Destructor
Customer::~Customer() { }


//========================== Overloaded Operators =============================

// Print Customer information in the following format:
// last_name, first_initial. (account_balance)
ostream& operator << (ostream& os,  const Customer& c)
{
    os << c.get_surname() << ", " << c.get_first_initial()
       << ". (" << c.get_account_balance() << ")";
    return os;
}


// Returns true if last name, first initial of a is less than that of b.
bool operator < (const Customer& a, const Customer& b)
{
    string a_name = a.get_surname() + a.get_first_initial();
    string b_name = b.get_surname() + b.get_first_initial();
    return (a_name < b_name);
}


// Returns true if last name, first initial of a 
// is less than or equal to that of b.
bool operator <= (const Customer& a, const Customer& b)
{
    string a_name = a.get_surname() + a.get_first_initial();
    string b_name = b.get_surname() + b.get_first_initial();
    return (a_name <= b_name);
}


// Returns true if last name, first initial of a is greater than that of b.
bool operator > (const Customer& a, const Customer& b)
{
    string a_name = a.get_surname() + a.get_first_initial();
    string b_name = b.get_surname() + b.get_first_initial();
    return (a_name > b_name);
}


// Returns true if last name, first initial of a 
// is greater than or equal to that of b.
bool operator >= (const Customer& a, const Customer& b)
{
    string a_name = a.get_surname() + a.get_first_initial();
    string b_name = b.get_surname() + b.get_first_initial();
    return (a_name >= b_name);
}


// Returns true if last name, first initial of a is equal to that of b.
bool operator == (const Customer& a, const Customer& b)
{
    string a_name = a.get_surname() + a.get_first_initial();
    string b_name = b.get_surname() + b.get_first_initial();
    return (a_name == b_name);   
}


// Returns true if last name, first initial of a is not equal to that of b.
bool operator != (const Customer& a, const Customer& b)
{
    string a_name = a.get_surname() + a.get_first_initial();
    string b_name = b.get_surname() + b.get_first_initial();
    return (a_name != b_name);
}