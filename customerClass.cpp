#include <string>
#include <vector>
#include <ctime>

using namespace std;

// Declaring the Customer class.
class customer {

   private:
     // Private member variables to store the customer's name and address
     // information.
   string firstName;
   string lastName;
   string password;
   string address;
   string city;
   string state;
   string zip;
   string id;
   string setID(vector<customer>& customerList);
               
   public:
      // List of all function prototypes. See further information below the class where
      // they are implemented.
   customer();
                  
   customer(vector<customer>& customerList, string, string, string, string, string, string);
            
   customer(string, string, string, string, string, string, string, string);
      
   string getID();
   
   string getPassword();
   
   void setPassword(string);
            
   void setFirstName(const std::string& lName);
            
   string getFirstName();
            
   void setLastName(const std::string& lName);
               
   string getLastName();
            
   string getFullName();
            
   void setAddress(string);
            
   string getAddress();
            
   void setCity(const string &c);
            
   string getCity();
               
   void setState(const string &s);
            
   string getState();
               
   void setZip(const string &z);
            
   string getZip();
               
   void setFullAddress(const string &c, const string &s, const string &z);
               
   string getFullAddress();
            
   ~customer();
}; 
  

// Default constructor
customer::customer()  
{
   id = "";
   password = "";
   firstName = "";
   lastName = "";
   city = "";
   state = "";
   zip = "";   
   address = "";
}

// Constructor for customers that are manually added from the menu.
customer::customer(vector<customer>& customerList, string fn, string ln, string address, string city, string state, string zip)  
{
   this ->id = setID(customerList);
   this ->password = "wuleeohv";
   this ->firstName = fn;
   this ->lastName = ln;
   this ->address = address;
   this ->city = city;
   this ->state = state;
   this ->zip = zip; 
}

// Constructor for customers that are read in from 'customers.txt'.
customer::customer(string id, string pw, string fn, string ln, string address, string city, string state, string zip)  
{
   this ->id = id;
   this ->password = pw;
   this ->firstName = fn;
   this ->lastName = ln;
   this ->address = address;
   this ->city = city;
   this ->state = state;
   this ->zip = zip; 
}



/* 
   Sets a manually entered customer's id. This is done by generating a random number
   between 100000 and 999999. This id is then checked and changed if 
   it is already owned by a customer in customerList 
   
   PARAMETER: customerList - a list of all customer objects
   
   CALLS: setID(recursively)
   
   CALLED BY: customer constructor (6 parameters)
   
   returns: default_id (as a string) - the newly created customer's unique ID #.
*/
string customer::setID(vector<customer>& customerList){ 

   // Seeds rand()
   srand(time(0));
   
  // new customer's proposed ID.
   int default_id = 100000 + (rand() % 899999);
   
   // flag to indicate uniqueness of default_id.
   bool unique = true;
  
   // Loops through idList to compare default_id with every other
   // existing customer ID.
   for (int i = 0; i<customerList.size(); i++){
      // If the default_id is already owned by a customer, the unique flag
      // is set to false and the loop is broken out of (this default_id is 
      // not okay to use).
      if(stoi(customerList[i].getID()) == default_id){
         unique = false;
         break;
      }
   }      
   // If the default_id is not unique, the function is executed again to generate
   // a new ID number. If it is unique, it is returned to the customer constructor
   // and set to the new customer object.
   if(unique == false)
      return setID(customerList);
   else
      return to_string(default_id);
}



// Gets the customer's id.
string customer::getID(){
   return id;
}


// Gets the customer's password.
string customer::getPassword(){
   return password;
}


// Sets the customer's password.
void customer::setPassword(string pw) 
{ 
   password = pw;
   return;
}


// Sets the first name of the customer.
void customer::setFirstName(const string& fName) 
{ 
   firstName = fName;
   return;
}
  
  
// Gets the first name of the customer.
string customer::getFirstName() { 
   return firstName; 
}


// Sets the last name of the customer.
void customer::setLastName(const string& lName) 
{
   lastName = lName; 
   return;
}



// Gets the last name of the customer.
string customer::getLastName() { 
   return lastName; 
}



 // Gets the full name of the customer.
string customer::getFullName() { 
   return firstName + " " + lastName; 
}


// Sets the customer's address.
void customer::setAddress(string address){
   address = address;
}


 // Gets the customer's address.
string customer::getAddress(){
   return address;
}



// Sets the city of the customer's address.
void customer::setCity(const string &c) { 
   city = c; 
   return;
}



// Gets the city from the customer's address.
string customer::getCity(){ 
   return city; 
}



 // Sets the state of the customer's address.
void customer::setState(const string &s) { 
   state = s; 
   return;
}



 // Gets the state from the customer's address.
string customer::getState(){ 
   return state; 
}



 // Sets the zip code of the customer's address.
void customer::setZip(const string &z) { 
   zip = z; 
   return;
}



// Gets the zip code from the customer's address.
string customer::getZip() { 
   return zip; 
}



 // Sets the full address of the customer
void customer::setFullAddress(const string &c, const string &s,
                      const string &z) {
   city = c;
   state = s;
   zip = z;
   return;
}


// Forms + gets the full address of the customer.
string customer::getFullAddress(){ 
   return address + " " + city + ", " + state + " " + zip; 
}


    
   
// Destructor
customer::~customer() 
{
   
}
