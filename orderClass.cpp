#include <ctime>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

class order{
//make all our private data fields
private:
   string orderID;
   string date;
   int quantity;
   double amountPaid;

	//generates new orderID for constructor
   string setOrderID(vector<order>, int);

	//generates string of current date for constructor
   string setDate();

	//determines the amountPaid based off of the quantity
   double setAmountPaid(int);

public:
	//default constructor
   order();
	//manual input constructor
   order(int,vector<order>);
	//data file read constructor
   order(string,string,int,double);

	//getters for each variable in object
   string getOrderID();
   string getDate();
   int getQuantity();
   double getAmountPaid();

	//destructor
   ~order();

};  

/* 
   Generates a unique orderID
   
   PARAMETERS:
      - orderList - a vector of all customer objects
      - iteration - the (n+1)th time that this function is running
        for the same object.
   
   CALLS: setOrderID(recursively)
   
   CALLED BY: order constructor (2 parameters)
   
   returns: orderID (as a string) - the newly created order's unique ID #.
*/
string order::setOrderID(vector<order> orderList,int iteration){
	// create a temporary order ID that is intended to be the most 
   // recent one plus 1
   int tempOrderID=orderList.size()+1+iteration;
   
	//check to make sure the order number isn't already taken.
	//In theory it shouldn't be taken since we are counting up
   bool unique=true;
   for (int i = 0; i<orderList.size(); i++){
      if(stoi(orderList[i].getOrderID()) == tempOrderID){
         unique = false;
         break;
      }
   }
 
   // formats id number as a string, adding leading 0s
   // before the tempOrderID generated above
   string orderID = "";
   int zeros = 6 - to_string(tempOrderID).size();
   
   for(int i = 0; i<zeros; i++){
      orderID += "0";
      if(i == zeros - 1)
         orderID += to_string(tempOrderID);
   }
   
   //Returns orderID if unique. If not, it is called recursively and the
   // process repeats.
   if(unique)
      return orderID;
   else
      return setOrderID(orderList,iteration+1);
}



string order::setDate(){
	//read in current system time
   time_t currentTime=time(nullptr);
	//put the current time into a time structure
   tm* cTime=localtime(&currentTime);
	//make list of month abbreviations in order to change number month to string, rather than ifs
   const char* monthAbbr[]={"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	//concatenate parts into one date in correct format
   string date;
   date=to_string(cTime->tm_mday) + "-" + monthAbbr[cTime->tm_mon] + "-" + to_string(cTime->tm_year%100);
   return date;
}



//Assigns dollar amount to quantity, only works up to quantity = 5
double order::setAmountPaid(int quantity){
   if(quantity==1){
      return 9.50;
   }
   if(quantity==2){
      return 16.15;
   }
   if(quantity==3){
      return 25.88;
   }
   if(quantity==4){
      return 28.15;
   }
   return 30.00;
}



//default constructor
order::order(){
   orderID="0";
   date="0";
   quantity=0;
   amountPaid=0;
}
//constructor for manual input
order::order(int q,vector<order> orderList){
   orderID=setOrderID(orderList,0);
   date=setDate();
   quantity=q;
   amountPaid=setAmountPaid(q);
}
//constructor for reading in from file
order::order(string id, string d, int q, double paid){
   orderID=id;
   date=d;
   quantity=q;
   amountPaid=paid;
}



//getters for each data field
string order::getOrderID(){
   return orderID;
}

string order::getDate(){
   return date;
}

int order::getQuantity(){
   return quantity;
}

double order::getAmountPaid(){
   return amountPaid;
}


order::~order(){

}
