//=============================================================================
// Name        : Customer.h
//                                                   George Giovanis, 301254694
//=============================================================================



#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <string>
#include <iostream>
using namespace std;


/*       The Customer class below represents a Customer of some financial 
 * enterprise. Every function defined in the class runs in O(1) time.
 *
 *       The class contains a default constructor, a constructor 
 * that takes a values for Customer data members, a copy constructor that
 * creates a deep copy of another Customer, and a destructor. Getters are 
 * supplied for each data member and a setter is supplied for account_balance.
 * 
 *       The class overloads the ostream << operator for printing a Customer's 
 * data. In addition, the class provides overloaded comparison operators for
 * <, <=, >, >=, ==, !=, which compare the surname and first initial values
 * of two Customer objects. */


class Customer {

// Data Members 
private:

	string surname;       // Customer's last name
	char first_initial;   // Customer's first initial
	int account_balance;  // Customer's balance owing


// Protected Utility Functions
protected:

	string get_surname() const;           // Return Customer's last name                          
    char get_first_initial() const;       // Return Customer's first initial
    int get_account_balance() const;      // Return Customer's account balance
    void set_balance(const int new_bal);  // Set Customer's account balance


// Public Functions
public:

    Customer();  // Default Constructor

    // Constructor with supplied Customer member values
	Customer(const string last_name, const char initial, const int bal);
    
    Customer(const Customer& c);  // Copy Constructor
	~Customer();                  // Destructor

    //Friend declarations
    friend class Node;
    friend class BSTree;


// Overloaded Operators
public:

	friend ostream& operator << (ostream& os,  const Customer& c);
	friend bool operator < (const Customer& a, const Customer& b);
	friend bool operator <= (const Customer& a, const Customer& b);
	friend bool operator > (const Customer& a, const Customer& b);
	friend bool operator >= (const Customer& a, const Customer& b);
	friend bool operator == (const Customer& a, const Customer& b);
	friend bool operator != (const Customer& a, const Customer& b);
};

#endif /* CUSTOMER_H_ */