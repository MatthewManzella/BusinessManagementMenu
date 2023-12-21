#include<string>

using namespace std;

//This class is really basic as it is essentially only for storing
//data. Search algorithms are within menu.cpp
class transaction{

   //make all data under private/functions under public
   private:
   string orderID, salesPersonID, customerID;
      
   public:
   transaction();
   transaction(string,string, string);
   string getOrderID();
   string getSalesPersonID();
   string getCustomerID();
   ~transaction();
};

//default constructor
transaction::transaction(){
   orderID=customerID=salesPersonID="0";
}

//constructor that takes in both data fields
transaction::transaction(string customer, string seller, string order){
   customerID=customer;
   salesPersonID = seller;
   orderID=order;
}

//getter for each data field
string transaction::getOrderID(){
   return orderID;
}
string transaction::getCustomerID(){
   return customerID;
}

string transaction::getSalesPersonID(){
   return salesPersonID;
}

//destructor for object
transaction::~transaction(){
}
