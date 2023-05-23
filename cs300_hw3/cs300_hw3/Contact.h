//
//  Contact.h
//  cs300_hw3
//
//  Created by Doruk Dülger on 29.04.2023.
//

#ifndef Contact_h
#define Contact_h
#include <string>

using namespace std;

struct Contact{
    string firstName;
    string lastName;
    string phoneNumber;
    string city;
    Contact(string f,string l,string p,string c){
        firstName = f;
        lastName = l;
        phoneNumber = p;
        city = c;
        for (auto & x: firstName) x = toupper(x);
        for (auto & x: lastName) x = toupper(x);
    }
    
    
    bool operator<(const Contact& other) const {
        return (this->firstName + " " + this->lastName) < (other.firstName + " " + other.lastName);
    }

    bool operator>(const Contact& other) const {
        return (this->firstName + " " + this->lastName) > (other.firstName + " " + other.lastName);
    }

    bool operator== (const Contact& rhs){
        return (firstName==rhs.firstName && lastName==rhs.lastName);
    }
    string getFullname(){
        return firstName + " " + lastName;
    }
    
    friend ostream& operator<<(ostream& os, const Contact& contact) {
            os << contact.firstName << " " << contact.lastName << endl;
            return os;
        }
};
#endif /* Contact_h */
