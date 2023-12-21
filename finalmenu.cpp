/* phase4menu - Matthew, Bakr, Nolan

This menu implements customerClass, orderClass, and transactionClass
to carry out 6 main operations, all of which the user can select as
many times as needed:

   1.) Add a customer to company records
   2.) Search for a customer by last name or ID #
   3.) Sell 1-5 tribbles to any given customer in
       company records
   4.) Manage rainbow tribble sales and waitlisting
   5.) Print a sales report for all sales staff
   6.) Customers can log in to their customer portal
       using their id/password. From here, they can 
       review order history, place an order, or change
       their password

 Upon starting the program, data is read from customers.txt, orders.txt,
 transactions.txt, salesStaff.txt and rainbowList.txt. After the user terminates the program,
 all data is loaded back out to the same files including any newly created data
 from the options above.

 Further specifications can be read about in function-level documentation below.

*/

#include "customerClass.cpp"
#include "orderClass.cpp"
#include "transactionClass.cpp"
#include "workerClass.cpp"
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <queue>
#include <ctime>
#include <iomanip>
#include <cctype>

using namespace std;

//descriptions can be found by the function's actual implementation
void printMainMenu();
void openAppendMenu(vector<customer>& customerList);
void printAppendMenu();
void openSearchMenu(vector<customer>& customerList, vector<order>& orderList, vector<transaction>& transactionList);
void printSearchMenu();
void openCustomerSearchMenu(vector<customer>& customerList, vector<order>& orderList, vector<transaction>& transactionList);
void printCustomerSearchMenu();
void openRainbowMenu(queue<string>& rainbowList, vector<transaction>& transactionList, vector<customer>& customerList, vector<order>& orderList);
void printRainbowMenu();
void customerSale(vector<order>& orderList, vector<transaction>& transactionList, vector<customer>& customerList, vector<Worker*>& staffList);
void salesCalc(Worker* staff, vector<Worker*>& staffList, vector<transaction>& transactionList, vector<order>& orderList);
void findManagerEmployees(Manager* staff, const vector<Worker*>& staffList, const vector<transaction>& transactionList, const vector<order>& orderList);
void findSupervisorEmployees(Supervisor* staff, const vector<Worker*>& staffList, const vector<transaction>& transactionList, const vector<order>& orderList);
void writeCustomerData(string, vector<customer>& customerList);
void readCustomerData(string, vector<customer>& customerList);
void writeTransactionData(string, vector<transaction>& transactionList);
void readTransactionData(string, vector<transaction>& transactionList);
void writeRainbowData(string, queue<string>& rainbowList);
void readRainbowData(string, queue<string>& rainbowList);
void writeOrderData(string, vector<order>& orderList);
void readOrderData(string, vector<order>& orderList);
void readSalesStaffData(string, vector<Worker*>& staffList);
void addCustomerbyHand(vector<customer>& customerList);
void addRainbowWaitingList(queue<string>& rainbowList, vector<customer>& customerList);
bool customerInWaitingList(queue<string> rainbowList, string ID);
void sellRainbowTribble(queue<string>& rainbowList, vector<transaction>& transactionList, vector<customer>& customerList, vector<order>& orderList);
void displayWaitList(queue<string> rainbowList, vector<customer>& customerList);
int menuValidityCheck(string&,int);
int oneToFiveValidityCheck(string&,int);
customer lastNameSearch(string lName, vector<customer>& customerList);
customer idSearch(string, vector<customer>& customerList);
void displayInfo(customer, vector<order>& orderList, vector<transaction>& transactionList);
int displayOrders(string, vector<order>& orderList, vector<transaction>& transactionList);
void createSalesReport(vector<Worker*>& workerList);
void updateSales(Worker* staff, vector<Worker*>& workerList, double price);
void userLogin(vector<order>& orderList, vector<transaction>& transactionList, vector<customer>& customerList);
void customerPortal(customer, vector<order>& orderList, vector<transaction>& transactionList, vector<customer>& customerList);
string encryptPassword(string);
bool isValid(string);



int main() {


   // The file that the customers and their information are read from/written to.
   const string customerFileName = "customers.txt";

   // The file that transaction data is read from/written to.
   const string transactionFileName = "transactions.txt";

   // The file that the rainbow tribble order line is read from/written to.
   const string rainbowFileName = "rainbowList.txt";

   // The file that all order information is read from/written to.
   const string orderFileName = "orders.txt";

   // The file that all sales staff information is read from.
   const string salesFileName = "salesStaff.txt";

   // customerList is a vector that is constantly added to. It's a complete list
   // of all customer objects.
   vector<customer> customerList;

   // transactionList is a vector that is constantly added to. It's a complete list
   // of all transaction objects.
   vector<transaction> transactionList;

   // orderList is a vector that is constantly added to. It's a complete list
   // of all order objects.
   vector<order> orderList;

   // queue to store line of customers waiting for a rainbow tribble. Only customer ids are stored.
   queue<string> rainbowList;

   // staffList is a vector that holds all sales staff, their position, their ids, and their boss's ids.
   vector<Worker*> staffList;

   cout<<"This program is for appending, modifying, and retrieving data related to\n"
      	<<"Tribble & Co.\n";

   // Populates customerList with all customers
   readCustomerData(customerFileName, customerList);

   // Populates transactionList with all transactions
   readTransactionData(transactionFileName, transactionList);

   // Populates rainbowList with all customer IDs of those ordering
   // rainbow tribbles
   readRainbowData(rainbowFileName, rainbowList);

   // Populates orderList with all orders
   readOrderData(orderFileName, orderList);

   // Populates staffList with all sales staff
   readSalesStaffData(salesFileName, staffList);

   // Loops through staffList, sending all Worker objects to salesCalc where they can have their total sales calculated/set.
   for(int i = 0; i < staffList.size(); i++){
      salesCalc(staffList[i], staffList, transactionList, orderList);
   }

      // Identifies managers and supervisors, sending them to findSupervisorEmployees/findManagerEmployees
      // to calculate/set their employees' total sales sum
   for(int i = 0; i < staffList.size(); i++){
      if (staffList[i]->getPosition() == "Manager"){
         Manager* staff = static_cast<Manager*> (staffList[i]);
         findManagerEmployees(staff, staffList, transactionList, orderList);
      }
      else if (staffList[i]->getPosition() == "Supervisor"){
         Supervisor* staff = static_cast<Supervisor*> (staffList[i]);
         findSupervisorEmployees(staff, staffList, transactionList, orderList);
      }
   }

   printMainMenu();

	//initialize the variable that will be used throughout the program to select parts of program
   int userInput;
   string inputLine;
   getline(cin,inputLine);

   //check for valid response, 0 to 6, and assign it to variable
   userInput=menuValidityCheck(inputLine,6);

	//continue the following loop unless terminate number 0, is inputted
   while(userInput!=0){
   
   	//decide which sub-menu to go to
      switch(userInput){
      
      //case for menu for adding a customer
         case 1:
            openAppendMenu(customerList);
            break;
      
      //case for opening the search menu (retrieving data)
         case 2:
            openSearchMenu(customerList, orderList, transactionList);
            break;
      
      //case for opening customer sale menu
         case 3:
            customerSale(orderList, transactionList, customerList, staffList);
            break;
      
      //case for opening rainbow tribble menu
         case 4:
            openRainbowMenu(rainbowList, transactionList, customerList, orderList);
            break;
      //case for retrieving sales report
         case 5:
            createSalesReport(staffList);
            break;
      //case for accessing customer portal login
         case 6:
            userLogin(orderList, transactionList, customerList);
            break;
      }
   
      printMainMenu();
      //Get the new userInput value
      getline(cin,inputLine);
      //check for valid response, 0 to 6, and assign it to variable
      userInput=menuValidityCheck(inputLine,6);
   }
   // Writes data to files upon exit (0 is entered)
   writeCustomerData(customerFileName, customerList);
   writeTransactionData(transactionFileName, transactionList);
   writeRainbowData(rainbowFileName, rainbowList);
   writeOrderData(orderFileName, orderList);

   // Frees up dynamic memory from staffList (no need to write back out to file)
   for(int i = 0; i < staffList.size(); i++){
      delete staffList[i];
   }

   //Say a termination message before ending main program
   cout<<"\nProgram terminated, goodbye.";
   return 0;
}



//function for printing out main menu with options listed
void printMainMenu(){
   cout<<"\n\n--------------------------Main Menu--------------------------\n"
      <<"1. Add New Customer Menu\n"
      <<"2. Search Menu\n"
      <<"3. Customer Sale Menu\n"
      <<"4. Rainbow Tribble Menu\n"
      <<"5. Print Sales Report\n"
      <<"6. Customer Portal\n"
      <<"0. Terminate Program\n"
      <<"\nPlease input the number associated with what you want from above: ";
   return;
}



 /*
function to open appending menu if selected in main()

Parameters:
   - customerList - vector containing all customer objects (passed by ref)

   CALLS: addCustomerbyHand, menuValidityCheck, printAppendMenu
   CALLED BY: main menu
*/
void openAppendMenu(vector<customer>& customerList){

   printAppendMenu();

	//reads the user selection
   int secondUserInput;
   string secondInputLine;
   getline(cin,secondInputLine);

   //check for valid response, 0 or 1, and assigns it to variable
   secondUserInput=menuValidityCheck(secondInputLine,1);

   while(secondUserInput!=0){
   
      switch(secondUserInput){
      //case for appending customer information
         case 1:
            addCustomerbyHand(customerList);
            break;
      }
      //give users options to select from and records selection
      printAppendMenu();
      getline(cin,secondInputLine);
   
      //check for valid response, 0 or 1, and assigns it to variable
      secondUserInput=menuValidityCheck(secondInputLine,1);
   }
   return;
}




//prints out append menu with option(s) listed
void printAppendMenu(){
   cout<<"\n\n--------------------------Add New Customer Menu--------------------------\n"
      <<"1. Add New Customer\n"
      <<"0. Return to Main Menu\n"
      <<"\nPlease input the number associated with what you want from above: ";
   return;
}



/*
function to open searching menu if selected in main()
Parameters:
   - customerList - vector containing all customer objects (passed by ref)
   - orderList - vector containing all order objects (passed by ref)
      - transactionList - vector containing all transaction objects (passed by ref)

   CALLS: openCustomerSearchMenu, menuValidityCheck, printSearchMenu
   CALLED BY: main menu
*/
void openSearchMenu(vector<customer>& customerList, vector<order>& orderList, vector<transaction>& transactionList){
	//function to give user options to select from
   printSearchMenu();

	//read user selection for switch
   int secondUserInput;
   string secondInputLine;
   getline(cin,secondInputLine);

   //check for valid response, 0 to 1, and assign it to variable
   secondUserInput=menuValidityCheck(secondInputLine,1);

   while(secondUserInput!=0){
   
      switch(secondUserInput){
      //case for searching for customer
         case 1:
            openCustomerSearchMenu(customerList, orderList, transactionList);
            break;
      }
   	//give users options to select from and records selection
      printSearchMenu();
      getline(cin,secondInputLine);
   
   //check for valid response, 0 to 1, and assign it to variable
      secondUserInput=menuValidityCheck(secondInputLine,1);
   }
   return;
}



//give user options to search
void printSearchMenu(){
   cout<<"\n\n--------------------------Search Menu--------------------------\n"
      <<"1. Search For a Customer\n"
      <<"0. Return to Main Menu\n"
      <<"\nPlease input the number associated with what you want from above: ";
   return;
}



/*
function for running the customer search menu if selected in the openSearchMenu()

Parameters:
- customerList - vector containing all customer objects (passed by ref)
- transactionList - vector containing all transaction objects (passed by ref)
- orderList - vector containing all order objects (passed by ref)

CALLED BY: openSearchMenu
CALLS: printCustomerSearchMenu, menuValidityCheck, idSearch, lastNameSearch, displayInfo
*/
void openCustomerSearchMenu(vector<customer>& customerList, vector<order>& orderList, vector<transaction>& transactionList){
	//give user options to select from
   printCustomerSearchMenu();

	//read in their selection to our variable to use with switch
   int thirdUserInput;
   string thirdInputLine;
   getline(cin,thirdInputLine);

   //check for valid response, 0 to 2, and assign it to variable
   thirdUserInput=menuValidityCheck(thirdInputLine,2);

   while(thirdUserInput!=0){
      string numID, lastName;
      customer target;
      switch(thirdUserInput){
         //case for searching by ID number
         case 1:
            cout<<"\nPlease enter the 6 digit ID number: ";
            getline(cin, numID);
            target = idSearch(numID, customerList);
            // Info is displayed based on if the customer is found or not.
            if(target.getID() == "")
               cout << "\n\nCustomer not found." << endl;
            else
               displayInfo(target, orderList, transactionList);
            break;
         //case for searching by last name
         case 2:
            cout<<"\nPlease enter the customer's last name (case sensitive): ";
            getline(cin, lastName);
            target = lastNameSearch(lastName, customerList);
            // Info is displayed based on if the customer is found or not.
            if(target.getID() == "")
               cout << "\n\nCustomer not found." << endl;
            else
               displayInfo(target, orderList, transactionList);
            break;
      }
      //give user options again and read their selection
      printCustomerSearchMenu();
      getline(cin,thirdInputLine);
   
      //check for valid response, 0 to 2, and assign it to variable
      thirdUserInput=menuValidityCheck(thirdInputLine,2);
   }
   return;
}



//function to print customer search options, within openCustomerSearchMenu()
void printCustomerSearchMenu(){
   cout<<"\n\n--------------------------Customer Search Menu--------------------------\n"
      <<"1. Search by ID Number\n"
      <<"2. Search by Last Name\n"
      <<"0. Return to Search Menu\n"
      <<"\nPlease input the number associated with what you want from above: ";
}



/*
function for running the rainbow tribble search, within the mainMenu()

Parameters:
- customerList - vector containing all customer objects (passed by ref)
- transactionList - vector containing all transaction objects (passed by ref)
- rainbowList - queue containing rainbow tribble waiting list (passed by ref)
- orderList - vector containing all order objects (passed by ref)

CALLED BY: main menu
CALLS: printRainbowMenu, menuValidityCheck, addRainbowWaitingList, sellRainbowTribble
*/
void openRainbowMenu(queue<string>& rainbowList, vector<transaction>& transactionList, vector<customer>& customerList, vector<order>& orderList){
   //give user options to select
   printRainbowMenu();

	//read the user selection
   int userInput;
   string userInputLine;
   getline(cin,userInputLine);

   //check for valid response, 0 to 3, and assign it to variable
   userInput=menuValidityCheck(userInputLine,3);

   while(userInput!=0){
   
      switch(userInput){
      //case for adding to waiting list
         case 1:
            addRainbowWaitingList(rainbowList, customerList);
            break;
      //case for selling to next in line
         case 2:
            sellRainbowTribble(rainbowList, transactionList, customerList, orderList);
            break;
      //case for displaying waitlist
         case 3:
            displayWaitList(rainbowList, customerList);
            break;
      }
      //give users options to select from menu and records selection
      printRainbowMenu();
      getline(cin,userInputLine);
   
      //check for valid response, 0 to 3, and assign it to variable
      userInput=menuValidityCheck(userInputLine,3);
   }
   return;
}



 //function to print rainbow tribble options, within openRainbowMenu()
void printRainbowMenu(){
   cout<<"\n\n--------------------------Rainbow Tribble Menu--------------------------\n"
         <<"1. Add Customer to Waiting List\n"
         <<"2. Sell to the Next Person in Line\n"
         <<"3. Display Waiting List\n"
         <<"0. Return to Main Menu\n"
         <<"\nPlease input the number associated with what you want from above: ";
}



/*
function for calculating/setting the total sales sum achieved by an employee.

Parameters:
- staff - the Worker* object of the staff that is having their total sales calculated
- staffList - vector containing pointers to all Worker objects (passed by const ref)
- transactionList - vector containing all transaction objects (passed by const ref)
- orderList - vector containing all order objects (passed by const ref)

CALLED BY: main menu
*/
void salesCalc(Worker* staff, vector<Worker*>& staffList, vector<transaction>& transactionList, vector<order>& orderList){
   // vector to contain all orderIDs correlating to the staff's sales
   vector<string> ordersMadeByStaff;

   // loops through transactionList searching for matching IDs
   for(int i = 0; i < transactionList.size(); i++){
      // if the staff ID == the transaction's salesperson ID, the orderID is added to the vector
      if(staff->getID() == transactionList[i].getSalesPersonID()){
         ordersMadeByStaff.push_back(transactionList[i].getOrderID());
      }
   }

   // loops through all orderIDs correlating to staff's sales
   for(int i = 0; i < ordersMadeByStaff.size(); i++){
      // loops through orderList... when an orderID match is found, the staff's total
      // sales are incremented by the amount paid.
      for(int j = 0; j < orderList.size(); j++){
         if(ordersMadeByStaff[i] == orderList[j].getOrderID()){
            staff->incrSales(orderList[j].getAmountPaid());
            // break = to avoid unnecessary repetition
            break;
         }
      }
   }

   return;
}



/*
function for calculating/setting the employee sales sum for supervisors.

Parameters:
- staff - the Supervisor* object that is having their employee sales calculated
- staffList - vector containing pointers to all Worker objects (passed by const ref)
- transactionList - vector containing all transaction objects (passed by const ref)
- orderList - vector containing all order objects (passed by const ref)

CALLED BY: main menu
*/
void findSupervisorEmployees(Supervisor* staff, const vector<Worker*>& staffList, const vector<transaction>& transactionList, const vector<order>& orderList){
   // employeeList stores all employees who work for the supervisor passed as an argument
   vector<Worker*> employeeList;
   double empSales;

   // loops through all staff
   for(int i = 0; i < staffList.size(); i++){
      // adds employee to employeeList if the supervisor is his/her boss
      if(staffList[i]->getBossID() == staff->getID()){
         employeeList.push_back(staffList[i]);
      }
   }

   // finally loops through employeeList to increment empSales with
   // the total Sales of all subordinate employees
   for(int i = 0; i < employeeList.size(); i++){
      empSales = employeeList[i]->gettSales();
      staff->incrEmpSales(empSales);
   }
}



/*
function for calculating/setting the employee sales sum for managers

Parameters:
- staff - the Manager* object that is having their employee sales calculated
- staffList - vector containing pointers to all Worker objects (passed by const ref)
- transactionList - vector containing all transaction objects (passed by const ref)
- orderList - vector containing all order objects (passed by const ref)

CALLED BY: main menu
*/
void findManagerEmployees(Manager* staff, const vector<Worker*>& staffList, const vector<transaction>& transactionList, const vector<order>& orderList){
   vector<Worker*>employeeList;
   double empSales = 0;
    // loops through all staff twice nested: once to find supervisors under the manager and another to find
    // the sales staff under the matching supervisor
   for(int i = 0; i < staffList.size(); i++){
         // if supervisor reports to manager they are added
      if(staffList[i]->getBossID() == staff->getID()){
         employeeList.push_back(staffList[i]);
            //second loop (inside conditional)
         for(int j = 0; j < staffList.size(); j++){
               // if the Sales/SuperSales reports to the selected supervisor, then
               // they are added
            if(staffList[j]->getBossID() == staffList[i]->getID()){
               employeeList.push_back(staffList[j]);
            }
         }
      }
   }

   // finally loops through employeeList to increment empSales with
   // the total Sales of all subordinate employees
   for(int i = 0; i < employeeList.size(); i++){
      empSales = employeeList[i]->gettSales();
      staff->incrEmpSales(empSales);
   }
}



/*
Sells X amount of tribbles to the customer specified by ID number. The results are stored
in order and transaction objects.

Parameters:
- customerList - vector containing all customer objects (passed by ref)
- transactionList - vector containing all transaction objects (passed by ref)
- orderList - vector containing all order objects (passed by ref)

CALLED BY: main menu
CALLS: idSearch, oneToFiveValidityCheck
*/
void customerSale(vector<order>& orderList, vector<transaction>& transactionList, vector<customer>& customerList, vector<Worker*>& staffList) {
   string customerID;
   string tempQuantity;
   string salesID;
   int quantity;
   customer saleCustomer;
   bool properID;

   cout << "\nEnter Customer ID: ";
   getline(cin, customerID);

   // Customer is searched for by ID. If not found, the function is ended early.
   saleCustomer = idSearch(customerID, customerList);

   if (saleCustomer.getID() == "") {
      cout << "\nCustomer not found!" << endl;
      cout << "\nTo add a new customer, select 'Add New Customer'." << endl;
      return;
   }

   cout << "\nEnter Quantity (1-5): ";
   getline(cin, tempQuantity);

   // Quantity re-prompted for if not a number 1-5.
   quantity = oneToFiveValidityCheck(tempQuantity, 5);


   // Prompted for the ID of the person who made the sale so that
   // they get commission.
   cout << "\nEnter Sales Person ID (000000 if N/A): ";
   getline(cin, salesID);

   Worker* seller;

   // Verifies that the seller ID is valid, reprompting if necessary
   while(salesID != "000000" && properID == false){
      // Loops through staffList, searching for seller
      for(int i = 0; i<staffList.size(); i++){
         // loop broken if a seller is found
         if(salesID == staffList[i]->getID()){
            properID = true;
            seller = staffList[i];
         }
      }
      if(properID == false){
         cout<<"\nPlease enter an existing ID: ";
         getline(cin, salesID);
      }
   }

   order newOrder(quantity, orderList);

   // Display confirmation message
   std::cout << "\nConfirmation Details:\n"
                 << "Customer: " << saleCustomer.getFullName() << "\n"
                 << "ID #: " << saleCustomer.getID() << "\n"
                 << "Quantity: " << quantity << "\n"
                 << "Price: $" << fixed << setprecision(2) << newOrder.getAmountPaid() << endl;

   // order and transaction records added to their respective vectors
   orderList.push_back(newOrder);

   transaction newTrans(saleCustomer.getID(), salesID, newOrder.getOrderID());

   transactionList.push_back(newTrans);

   // if a sales staff id was given, their sales and all boss's empSales
   // are updated
   if(salesID!="000000"){
      updateSales(seller, staffList, newOrder.getAmountPaid());
   }
}



/*
Opens customer.txt and writes every customer's information to the file
separated by semicolons. This is called when the user exits the main
menu in menu.cpp.

Parameters:
- string customerFileName - name of the file containing all customer information
- customerList& - vector containing all customer objects (passed by ref)

CALLED BY: menu.cpp

- utilizes ofstream

*/
void writeCustomerData(string customerFileName, vector<customer>& customerList){
  // Opens customers.txt for writing.
   ofstream outfile(customerFileName, ios::out);

   // Loops through customerList and writes every customer's information to the file
   // separated by semicolons.
   for(int i = 0; i<customerList.size(); i++){
      outfile << customerList[i].getID() << ";";
      outfile << customerList[i].getPassword() << ";";
      outfile << customerList[i].getFirstName() << ";";
      outfile << customerList[i].getLastName() << ";";
      outfile << customerList[i].getAddress() << ";";
      outfile << customerList[i].getCity() << ";";
      outfile << customerList[i].getState() << ";";
      outfile << customerList[i].getZip() << endl;
   }

   // FILE CLOSED
   outfile.close();
}



/*
   Reads all data (; separated) from customers.txt to customerList.

   Parameters:
   - string customerFileName - name of the file containing all customer information
   - customerList& - vector containing all customer objects (passed by ref)

   CALLED BY: menu.cpp.

   - utilizes ifstream and stringstream to receive/strip lines from customers.txt.
*/
void readCustomerData(string customerFileName, vector<customer>& customerList){
   ifstream infile(customerFileName, ios::in);

   // Executes if customers.txt is not found.
   if(infile.fail()){
      cerr << "\n\nCustomer data file not found.";
      exit(1);
   }

   string line;
   string id, password, firstName, lastName, address, city, state, zip;

   // Gets first line from customers.txt + stores to line
   getline(infile, line);

   // Continues until end of the file is reached
   while (!infile.eof() && !line.empty()){
      // Controls line via 'stream'
      stringstream stream(line);
   
      // Sets all necessary variables for customer constructor from customers.txt line
      getline(stream, id, ';');
      getline(stream, password, ';');
      getline(stream, firstName, ';');
      getline(stream, lastName, ';');
      getline(stream, address, ';');
      getline(stream, city, ';');
      getline(stream, state, ';');
      getline(stream, zip);
   
      // new customer is constructed via the fields read above and added to customerList
      customer newCustomer(id, password, firstName, lastName, address, city, state, zip);
      customerList.push_back(newCustomer);
   
      // Gets next line from customers.txt + stores to line
      getline(infile, line);
   }

   // FILE CLOSED
   infile.close();
}



 /*
Opens transactions.txt and writes every transaction to the file
separated by a semicolon. This is called when the user exits the main
menu in menu.cpp.

Parameters:
- string transactionFileName - name of the file containing all transaction information
- transactionList& - vector containing all transaction objects (passed by ref)

CALLED BY: menu.cpp

- utilizes ofstream
*/
void writeTransactionData(string transactionFileName, vector<transaction>& transactionList){
  // Opens transactions.txt for writing.
   ofstream outfile(transactionFileName, ios::out);

   // Loops through transactionList and writes all transaction information to the file
   // separated by a semicolon.
   for(int i = 0; i<transactionList.size(); i++){
      outfile << transactionList[i].getCustomerID() << ";";
      outfile << transactionList[i].getSalesPersonID() << ";";
      outfile << transactionList[i].getOrderID() << endl;
   }

   // FILE CLOSED
   outfile.close();
}



/*
   Reads all data (; separated) from transactions.txt to transactionList.

   Parameters:
      - string transactionFileName - name of the file containing all transaction information
      - transactionList& - vector containing all transaction objects (passed by ref)

   CALLED BY: menu.cpp.

   - utilizes ifstream and stringstream to receive/strip lines from transactions.txt.
*/
void readTransactionData(string transactionFileName, vector<transaction>& transactionList){
   ifstream infile(transactionFileName, ios::in);

   // Executes if transactions.txt is not found.
   if(infile.fail()){
      cerr << "\n\nTransaction data file not found.";
      exit(1);
   }

   string line;
   string customerID, sellerID, orderID;

   // Gets first line from transactions.txt + stores to line
   getline(infile, line);

   // Continues until end of the file is reached
   while (!infile.eof() && !line.empty()){
      // Controls line via 'stream'
      stringstream stream(line);
   
      // Sets all necessary variables for transactions constructor from transactions.txt line
      getline(stream, customerID, ';');
      getline(stream, sellerID, ';');
      getline(stream, orderID);
   
      // new transactions is constructed via the fields read above and added to transactionsList
      transaction newTransaction (customerID, sellerID, orderID);
      transactionList.push_back(newTransaction);
   
      // Gets next line from transactions.txt + stores to line
      getline(infile, line);
   }

   // FILE CLOSED
   infile.close();
}



/*
Opens rainbowList.txt and writes customer IDs in FIFO order. This is called when the user
exits the main menu in menu.cpp.

Parameters:
- string rainbowListFileName - name of the file containing all customer IDs for those in line
to buy rainbow tribbles
- rainbowList& - queue containing all customer IDs for those in line
to buy rainbow tribbles (passed by ref)

CALLED BY: menu.cpp

- utilizes ofstream
*/
void writeRainbowData(string rainbowFileName, queue<string>& rainbowList){
  // Opens rainbowList.txt for writing.
   ofstream outfile(rainbowFileName, ios::out);

   // Writes customer IDs to rainbowList.txt in FIFO order
   while (!rainbowList.empty()){
      outfile << rainbowList.front() << endl;
      rainbowList.pop();
   }

   // FILE CLOSED
   outfile.close();
}



/*
   Reads all id #s from rainbowList.txt to rainbowList.

   Parameters:
      - string rainbowFileName - name of the file containing all rainbow order information
      - rainbowList& - queue to contain all IDs of those in line (passed by ref)

   CALLED BY: menu.cpp.

   - utilizes ifstream and stringstream to receive/strip lines from rainbowList.txt.
*/
void readRainbowData(string rainbowFileName, queue<string>& rainbowList){
   ifstream infile(rainbowFileName, ios::in);

   // Executes if rainbowList.txt is not found.
   if(infile.fail()){
      cerr << "\n\nRainbow Tribble data file not found.";
      exit(1);
   }

   string line;
   string customerID;

   // Gets first line from rainbowList.txt + stores to line
   getline(infile, line);

   // Continues until end of the file is reached
   while (!infile.eof() && !line.empty()){
      // Controls line via 'stream'
      stringstream stream(line);
   
      getline(stream, customerID);
   
      // Adds ID to queue
      rainbowList.push(customerID);
   
      // Gets next line from rainbowList.txt + stores to line
      getline(infile, line);
   }

   // FILE CLOSED
   infile.close();
}



 /*
Opens orders.txt and writes all order information to the file
separated by semicolons. This is called when the user exits the main
menu in menu.cpp.

Parameters:
- string orderFileName - name of the file containing all order information
- orderList& - vector containing all order objects (passed by ref)

CALLED BY: menu.cpp

- utilizes ofstream

*/
void writeOrderData(string orderFileName, vector<order>& orderList){
   // Opens orders.txt for writing.
   ofstream outfile(orderFileName, ios::out);

   // Loops through orderList and writes all order information to the file
   // separated by semicolons.
   for(int i = 0; i<orderList.size(); i++){
      outfile << orderList[i].getOrderID() << ";";
      outfile << orderList[i].getDate() << ";";
      outfile << orderList[i].getQuantity() << ";";
      outfile << orderList[i].getAmountPaid() << endl;
   }

   // FILE CLOSED
   outfile.close();
}



/*
Reads all data (; separated) from orders.txt to orderList.

Parameters:
   - orderFileName: name of orderFile
   - orderList&: vector containing all order objects (passed by ref)

CALLED BY: menu.cpp.

utilizes ifstream and stringstream to receive/strip lines from orders.txt.
*/
void readOrderData(string orderFileName, vector<order>& orderList){
   ifstream infile(orderFileName, ios::in);

   // Executes if orders.txt is not found.
   if(infile.fail()){
      cerr << "\n\nOrder data file not found.";
      exit(1);
   }

   string line;
   string orderID, date, quantity, price;

   // Gets first line from orders.txt + stores to line
   getline(infile, line);


   // Continues until end of the file is reached
   while (!infile.eof() && !line.empty()){
      // Controls line via 'stream'
      stringstream stream(line);
   
      // Sets all necessary variables for order constructor from orders.txt line
      getline(stream, orderID, ';');
      getline(stream, date, ';');
      getline(stream, quantity, ';');
      getline(stream, price);
   
      // new order is constructed via the fields read above and added to orderList
      order newOrder(orderID, date, stoi(quantity), stod(price));
      orderList.push_back(newOrder);
   
      // Gets next line from orders.txt + stores to line
      getline(infile, line);
   }

   // FILE CLOSED
   infile.close();

}



/*
Reads all data (; separated) from salesStaff.txt to staffList.

Parameters:
   - fileName: name of sales staff file
   - staffList&: vector containing pointers to all sales staff objects (passed by ref)

CALLED BY: menu.cpp.

utilizes ifstream and stringstream to receive/strip lines from salesStaff.txt.
*/
void readSalesStaffData(string fileName, vector<Worker*>& staffList){
   ifstream infile(fileName, ios::in);

   // Executes if salesStaff.txt is not found.
   if(infile.fail()){
      cerr << "\n\nSales Staff data file not found.";
      exit(1);
   }

   string line;
   string position, name, id, bossID;

   // Gets first line from salesStaff.txt + stores to line
   getline(infile, line);


   // Continues until end of the file is reached
   while (!infile.eof() && !line.empty()){
      // Controls line via 'stream'
      stringstream stream(line);
   
      // Sets all necessary variables for Worker constructor from salesStaff.txt line
      getline(stream, position, ';');
      getline(stream, name, ';');
      getline(stream, id, ';');
      getline(stream, bossID, ';');
   
      Worker* newWorker;
   
      // Assigns pointer to correct class based on positionn field
      if (position == "Sales"){
         newWorker = new Sales(position, name, id, bossID);
      }
      else if (position == "SuperSales"){
         newWorker = new SuperSales(position, name, id, bossID);
      }
      else if (position == "Supervisor"){
         newWorker = new Supervisor(position, name, id, bossID);
      }
      else if (position == "Manager"){
         newWorker = new Manager(position, name, id, bossID);
      }
   
      // Adds pointer from above to staffList
      staffList.push_back(newWorker);
   
      // Gets next line from salesStaff.txt + stores to line
      getline(infile, line);
   }

   // FILE CLOSED
   infile.close();

}



/* Prompts the user to enter the customer's first name, last name, address, city, state, and zip
   in order to create a customer object to be added to the customerList.

   Parameter: customerList& - vector containing all customer objects (passed by ref)

   CALLED BY: openAppendMenu
 */
void addCustomerbyHand(vector<customer>& customerList){

   string fn, ln, address, city, state, zip;

   cin.clear();

   // Retrieves every variable from the user's input.
   cout << "\n\nEnter the customer's first name: ";
   getline(cin, fn);

   cout << "\n\nEnter the customer's last name: ";
   getline(cin, ln);

   cout << "\n\nEnter the customer's address (no city/state): ";
   getline(cin, address);

   cout << "\n\nEnter the customer's city of residence: ";
   getline(cin, city);

   cout << "\n\nEnter the customer's state of residence: ";
   getline(cin, state);

   cout << "\n\nEnter the customer's zip code: ";
   getline(cin, zip);

   // Creates new customer object with the information entered above + adds
   // this customer to the customerList vector.
   customer newCustomer(customerList, fn, ln, address, city, state, zip);
   customerList.push_back(newCustomer);
}



/*
Adds customer to the end of the rainbow tribble wait line in rainbowList.

Parameters:
- customerList - vector containing all customer objects (passed by ref)
- rainbowList - queue containing rainbow tribble waiting list (passed by ref)

CALLED BY: rainbowMenu()
CALLS: customerInWaitingList, idSearch.
*/
void addRainbowWaitingList(queue<string>& rainbowList, vector<customer>& customerList){
   string ID;
   bool inList;

   cout << "\n\nEnter a customer ID: ";
   getline(cin, ID);

   // Stores customer with entered ID to waitingCustomer if he/she is found
   customer waitingCustomer = idSearch(ID, customerList);

   // idSearch returns a blank object if no customer is found
   if(waitingCustomer.getID() == ""){
      cout << "\n\nCustomer not found. Please add a new customer via the "
         << "'Add New Menu' or enter a proper ID." << endl;
   }
   else{
      // Adds customer to waiting list if found. If not found, he/she isn't added
      inList = customerInWaitingList(rainbowList, ID);
      if(inList == false){
         rainbowList.push(ID);
         cout << "\n\n" << waitingCustomer.getFullName() << " has been added to the waiting list." << endl;
      }
      else {
         cout << "\n\n" << waitingCustomer.getFullName() << " is already on the waiting list." << endl;
      }
   }
   return;
}



/*
Checks if the customer is already on the rainbow tribble waiting list.

Parameters:
- rainbowList - queue containing rainbow tribble waiting list
- ID - string of customer ID to be checked

CALLED BY: addRainbowWaitingList

returns: bool found - represents if the customer ID already exists in the
waiting list or not
*/
bool customerInWaitingList(queue<string> rainbowList, string ID){
   bool found = false;
   string currentID;
   //pops front ID until it matches the parameter passed ID or the queue is empty.
   while (!rainbowList.empty() && found == false){
      currentID = rainbowList.front();
      if (currentID == ID){
         found = true;
      }
      rainbowList.pop();
   }
   return found;
}



/*
Sells a rainbow tribble to the first customer in line in rainbowList.

Parameters:
- customerList - vector containing all customer objects (passed by ref)
- transactionList - vector containing all transaction objects (passed by ref)
- rainbowList - queue containing rainbow tribble waiting list (passed by ref)
- orderList - vector containing all order objects (passed by ref)

CALLED BY: rainbowMenu()
CALLS: idSearch.
*/
void sellRainbowTribble(queue<string>& rainbowList, vector<transaction>& transactionList, vector<customer>& customerList, vector<order>& orderList){
   string newOwnerID;
   if(!rainbowList.empty()){
      newOwnerID = rainbowList.front();
      rainbowList.pop();
   // Controls for the case of if the queue is empty (no one in line)
   }
   else{
      cout << "\n\nNo one is in line" << endl;
      return;
   }
   // Stores customer with entered ID to newOwner
   customer newOwner = idSearch(newOwnerID, customerList);
   // new order object created + added to list
   order newOrder(1, orderList);
   orderList.push_back(newOrder);
   // Display confirmation message
   std::cout << "\nConfirmation Details:\n"
                 << "Customer: " << newOwner.getFullName() << "\n"
                 << "ID #: " << newOwner.getID() << "\n"
                 << "Quantity: " << newOrder.getQuantity() << "\n"
                 << "Price: $" << fixed << setprecision(2) << newOrder.getAmountPaid() << endl;
   // new transaction object created + added to list
   transaction newTransaction(newOwnerID, "000000", newOrder.getOrderID());
   transactionList.push_back(newTransaction);
   return;
}


 /*
Displays full name + id numbers of those waiting in line for a rainbow tribble

Parameters:
- customerList - vector containing all customer objects (passed by ref)
- rainbowList - queue containing rainbow tribble waiting list

CALLED BY: rainbowMenu()
*/

void displayWaitList(queue<string> rainbowList, vector<customer>& customerList){
   string ID;
   int count = 0;
   while (!rainbowList.empty()){
      ID = rainbowList.front();
      rainbowList.pop();
      for(int i = 0; i < customerList.size(); i++){
         if (customerList[i].getID() == ID){
            cout << "\n" << ++count << ": " << customerList[i].getFullName() << " (" << customerList[i].getID() << ")" << endl;
            break;
         }
      }
   
   }

   return;
}


/* checks the validity of inputted string. Once a valid response (#0-6) is found it returns it as an integer.
   Handles up to maxOption of 6.

   Parameter:
      - string& inputLine - the user's initial line of input (passed by ref)
      - int maxOption - the highest option that can be entered by the user

   CALLED BY: all menus

   returns: int userInput - the user's final. properly formatted selection
 */
int menuValidityCheck(string& inputLine, int maxOption){
  // Initializes userInput so that 0 isn't automatically returned (while loop isn't skipped).
   int userInput = 999;

   // Converts string to int if it is [0,6]
   if(inputLine=="0"|| inputLine=="1"||inputLine=="2"||inputLine=="3"||inputLine=="4"||inputLine=="5" ||inputLine=="6"){
      userInput=stoi(inputLine);
   }
   else {
      userInput = 999;
   }

   // While loop continues until a proper menu option is entered.
   while(inputLine.length()!=1||userInput>maxOption || userInput < 0){
      cout<<"\n\nInvalid input. Enter your selection: ";
      getline(cin,inputLine);
      if(inputLine=="0"|| inputLine=="1"||inputLine=="2"||inputLine=="3"||inputLine=="4"||inputLine=="5" ||inputLine=="6"){
         userInput=stoi(inputLine);
      }
      else {
         inputLine = "999";
      }
   }

   return userInput;
}



 /* checks the validity of inputted string. Once a valid response (#1-5) is found it returns it as an integer.
   Handles up to maxOption of 5.

   Parameter:
      - string& inputLine - the user's initial line of input (passed by ref)
      - int maxOption - the highest option that can be entered by the user

   CALLED BY: lastNameSearch and customerSale

   returns: int userInput - the user's final. properly formatted selection
 */

int oneToFiveValidityCheck(string& inputLine, int maxOption){
   int userInput = 999;
   if(inputLine=="1"||inputLine=="2"||inputLine=="3"||inputLine=="4"||inputLine=="5"){
      userInput=stoi(inputLine);
   }
   while(inputLine.length()!=1|| userInput>maxOption || userInput < 1){
      cout<<"\n\nInvalid input. Enter your selection: ";
      getline(cin,inputLine);
      if(inputLine=="1"||inputLine=="2"||inputLine=="3"||inputLine=="4"||inputLine=="5"){
         userInput=stoi(inputLine);
      }
      else {
         inputLine = "999";
      }
   }

   return userInput;
}



/*
Searches for a customer in customerList by their last name. If
at least 1 is found they are displayed and the user is asked to select
one for further details.

CALLED BY: customerSearchMenu

CALLS: oneToFiveValidityCheck

   parameters:
   - string lname - the last name that is being used in the search.
   - customerList& - vector containing all customer objects (passed by ref)

returns: customer object. If the specified customer is found, that customer
object is returned. If not, an empty customer is returned.
*/
customer lastNameSearch(string lName, vector<customer>& customerList){
  // lNameList is an empty list of last names matching the argument sent.
   vector<customer> lNameList;

   // found is a flag representing if a customer with the last name
   // given is found.
   bool found = false;

   // counter tracks the number of people with the last name given found.
   int counter = 0;

   // Loops through the customerList looking for customers with the given last name.
   for(int i = 0; i<customerList.size(); i++){
   
      // If a customer with the last name is found, it is added to lNameList and printed.
      // counter is increased by 1. Also, found is set to true.
      if(customerList[i].getLastName() == lName){
         counter++;
         lNameList.push_back(customerList[i]);
         cout << "\n\n" << counter << ": " << customerList[i].getFirstName() << " " <<  customerList[i].getLastName() << endl;
         found = true;
      }
   }

   string selection;
   customer nullCustomer;

   // If no customers are found, a blank customer is returned to activate a message in the
   // search menu function.
   if (found == false){
      return nullCustomer;
   }
   // If at least 1 customer is found, 'else' executes, telling the user
   // to select one of the following customers.
   else {
      cout << "\n\nEnter the number next to the customer that you were referring to: ";
   
      // Stores the user's input to selection.
      getline (cin, selection);
   
      int final_option = oneToFiveValidityCheck(selection, counter);
      // Returns selected customer.
      return lNameList[final_option - 1];
   }
}



/*
Searches for a customer in customerList by their id number. If
someone is found, they are displayed.

CALLED BY: customerSearchMenu

   parameters:
   - string id - the id that is being used in the search.
   - customerList& - vector containing all customer objects (passed by ref)

returns: customer object. If the specified customer is found, that customer
object is returned. If not, an empty customer is returned (both under target).

*/
customer idSearch(string id, vector<customer>& customerList){

   customer target;

   // Loops through all customers in customerList
   for(int i = 0; i<customerList.size(); i++){
      // If customer is found with the id, his/her information is displayed
      // and the loop is broken out of.
      if(customerList[i].getID() == id){
         target = customerList[i];
         break;
      }
   }

   return target;
}



/*
   Displays all the information of a given customer.

   CALLED BY: customerSearchMenu
   CALLS: displayOrders

   parameters: customer customer - the customer whose info is to be displayed. orderList
   and transactionList
*/
void displayInfo(customer customer, vector<order>& orderList, vector<transaction>& transactionList){
  // displays customer information
   cout << "\n\nName: " << customer.getFirstName() << " " << customer.getLastName() << endl << "Address: ";
   cout << customer.getFullAddress() << endl << "ID #: " << customer.getID() << endl << endl;
   // displays order information
   int count = displayOrders(customer.getID(), orderList, transactionList);
   // printed if no order information is found
   if (count==0)
      cout << "\nThis customer has no orders on record." << endl;
}



 /*
   Displays all the orders of a given customer.

   CALLED BY: displayInfo.

   parameters: string ID representing customer whose orders are to be displayed. orderList
   and transactionList.

   returns: int count - the number of orders the customer has on record
*/
int displayOrders(string ID, vector<order>& orderList, vector<transaction>& transactionList){

   vector<string> orderIDs;

   // populates orderIDs with all of the customer's order IDs
   for(int i = 0; i<transactionList.size(); i++){
      if(transactionList[i].getCustomerID() == ID)
         orderIDs.push_back(transactionList[i].getOrderID());
   }

   // Searches orderList to find all information regarding each of the
   // customer's orders found above. Prints anything that matches the ID.
   int count = 0;
   for(int i = 0; i<orderIDs.size(); i++){
      for(int j = 0; j<orderList.size(); j++){
         if(orderIDs[i] == orderList[j].getOrderID()){
            count++;
            cout<< "\nORDER #" << count
               << "\n***********************"
               << "\nQuantity: " << orderList[j].getQuantity()
               << "\nPrice: $" << fixed << setprecision(2) << orderList[j].getAmountPaid()
               << "\nDate: " << orderList[j].getDate() << endl;
         }
      }
   }

   return count;

}



/*
   Displays all information for all sales staff

   CALLED BY: main menu

   parameters: workerList (passed by ref) containing pointers to all workers
*/
void createSalesReport(vector<Worker*>& workerList){
   int nameSize;
   	// Loops through workerList and prints out name, id, tSales, and commission
   cout << "\nName                    ID                  Total Sales            Commission" << endl;
   for(int i = 0; i<workerList.size(); i++){
      // nameSize = the length of the employee's name (used to set width for table alignment
      nameSize = (workerList[i]->getName()).size();
      // printed: name, id, total sales, commission
      cout << workerList[i]->getName() << setw(30-nameSize) << workerList[i]->getID() << setw(15) << fixed << setprecision(2)
         << "       $" << setw(8) << workerList[i]->gettSales() << setw(15)
         << fixed << setprecision(2) << "       $" << setw(8) << workerList[i]->getCommission() << endl;
   }

   return;
}



 /*
if a sales staff id was given in customerSale, their sales and all their boss's empSales
are updated

Parameters:
- Worker* staff - a pointer to the sales staff who needs their sales updated
- staffList& - vector containing pters to all sales staff objects
- double price - the amount to be added to total sales


CALLED BY: customerSale
*/
void updateSales(Worker* staff, vector<Worker*>& staffList, double price){

   // increases the staff's total sales sum
   staff->incrSales(price);

   // loops until the manager is reached in the hierarchy
   while(staff->getBossID() != "000000"){
   
      // loops through staffList to find the staff's boss
      for(int i = 0; i<staffList.size(); i++){
      
         // once found, the boss's empSales is incremented and
         // the staff object now points at the boss to find their
         // own boss if applicable
         if(staff->getBossID() == staffList[i]->getID()){
         
            if (staffList[i]->getPosition() == "Manager"){
               Manager* superiorStaff = static_cast<Manager*> (staffList[i]);
               superiorStaff->incrEmpSales(price);
               staff = superiorStaff;
            }
            else if (staffList[i]->getPosition() == "Supervisor"){
               Supervisor* superiorStaff = static_cast<Supervisor*> (staffList[i]);
               superiorStaff->incrEmpSales(price);
               staff = superiorStaff;
            }
         
            // to avoid unnecessary for loop iterations
            break;
         }
      }
   }

   return;
}


/*
Manages user login. Prompts user for user ID number and password and if they match a pair of user ID
and password in customerList vector it will allow user to continue to the customer portal. Entering
000000 when prompted for user ID will return to main().

Parameters:
-vector<order>&orderList - passes along list of orders created in main()
-vector<transaction>&transactionList - passes along list of transactions created in main()
-vector<customer>&customerList - passes along list of customers created in main()

Calls: customerPortal

CALLED BY: main
*/
void userLogin(vector<order>&orderList,vector<transaction>&transactionList,vector<customer>&customerList){
   string enteredID;
   string enteredPassword;
   customer user;


	//prompt for user ID
   cout<<"\nEnter a login ID, '000000' to exit: ";
   getline(cin,enteredID);

   while(enteredID!="000000"){
   	//prompt for user password
      cout<<"\nPassword? ";
      getline(cin,enteredPassword);
   
   	//change entered password to encrypted version
      enteredPassword=encryptPassword(enteredPassword);
   
   	//search for user ID in customer list
      user=idSearch(enteredID, customerList);
   
   	//if ID isn't found reject login
      if(user.getID() == "")
         cout << "\n\nLogin or password not found." << endl;
      else if(enteredPassword!=user.getPassword()){
         cout << "\n\nLogin or password not found." << endl;
      } 
      else{
         customerPortal(user, orderList, transactionList, customerList);
         break; //break in order to return to main menu after signing out
      }
   	//reprompt for user ID
      cout<<"\nEnter a login ID, '000000' to exit: ";
      getline(cin,enteredID);
   }
}

/*
Handles basic Ceasar Cipher encryption. Returns a string of characters that are alphanumeric and
3 places to the left of initial inputed password. Alphanumeric characters are in the order of
ValidChars in the function.

Parameters:
- string password - the password that needs to be converted into encrypted version


CALLED BY: userLogin
*/
string encryptPassword(string pw)
{
	//string of all valid characters for passwords
   string ValidChars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
   string enc = "";

   for (int i = 0; i < pw.length(); i++)
   {
      int pos = ValidChars.find(pw[i], 0);
      pos += 3;
      if (pos > ValidChars.length())
      {
         pos -= ValidChars.length();
      }
   
      enc+=ValidChars[pos];
   }
   return enc;
}

/*
Menu for customer actions. Handles password changes, reviewing order history, and placing an online order.

Parameters:
-customer user - this is the customer object of the logged in user
-vector<order>& orderList - passes along list of orders created in main() for interacting w/orders
-vector<transaction>& transactionList - passes along list of orders created in main() for pairing orders
										to the customer
-vector<customer>& customerList - passes along list of orders created in main() for changing password in
								  the data that will be saved (this vector)

Calls: menuValidityCheck, encryptPassword, displayOrders, isValid

CALLED BY: userLogin
*/
void customerPortal(customer user, vector<order>& orderList, vector<transaction>& transactionList, vector<customer>& customerList){
	//these variables are only used in certain cases below
   string password1; //used if changing password
   string secondInput; //used if placing new order
   int quantity; //used if placing new order

   //Welcome message
   cout << "\nWelcome, " << user.getFullName() << "!";
   
	//prompt user with options
   cout<<"\n\n------------------------Customer Menu------------------------\n"
      <<"1. Change Password\n"
      <<"2. Review Order History\n"
      <<"3. Place New Order\n"
      <<"0. Sign Out\n"
      <<"\nPlease input the number associated with what you want from above: ";

	//read user selection
   int userInput;
   string inputLine;
   getline(cin,inputLine);

	//check for valid response, 0 to 3, and assign it to variable
   userInput=menuValidityCheck(inputLine,3);

	//continue the following loop unless terminate number 0, is inputed
   while(userInput!=0){
   
   //decide which sub-menu to go to
      switch(userInput){
      
      //case for changing password
         case 1:
            cout<<"\nEnter a new alphanumeric password with at least 4 digits: ";
            getline(cin,password1);
         //ensure password entered is valid
            while(!isValid(password1)){
               cout<<"\n\nInvalid entry. Enter a new alphanumeric password with at least 4 digits: ";
               getline(cin,password1);
            }
         //encrypt the password in the local customer
            user.setPassword(encryptPassword(password1));
         //locate user in customerList and assign password in list
            for(int i=0; i<customerList.size();i++){
               if(customerList[i].getID()==user.getID())
                  customerList[i].setPassword(user.getPassword());
            }
            cout<<"\nPassword changed!";
            break;
      
      //case for reviewing order history
         case 2:
            if(displayOrders(user.getID(), orderList, transactionList)==0)
               cout<<"\nNo orders on record.";
            break;
      
      //case for making new order
         case 3:
            string confirm;
         //Read in order
            cout<<"\nHow many would you like to order? (Up to 5 and enter 0 to cancel): ";
            getline(cin, secondInput);
            quantity=menuValidityCheck(secondInput,5);
            if(quantity==0)
               break;
            cout<<"\nHit enter with any message to confirm a purchase of " << quantity << " tribbles (enter 0 to cancel): ";
            getline(cin, confirm);
            // confirm purchase
            if(confirm=="0"){
               cout << "\nPurchase cancelled.";
               break;
            }
            order newOrder(quantity, orderList);
         
         // Display confirmation message
            std::cout << "\nConfirmation Details:\n"
                << "Customer: " << user.getFullName() << "\n"
                << "ID #: " << user.getID() << "\n"
                << "Quantity: " << quantity << "\n"
                << "Price: $" << fixed << setprecision(2) << newOrder.getAmountPaid() << endl;
         
         // order and transaction records added to their respective vectors
            orderList.push_back(newOrder);
         
            transaction newTrans(user.getID(), "000000", newOrder.getOrderID());
         
            transactionList.push_back(newTrans);
            break;
      }
   
   //prompt user with options
      cout<<"\n\n------------------------Customer Menu------------------------\n"
         <<"1. Change Password\n"
         <<"2. Review Order History\n"
         <<"3. Place New Order\n"
         <<"0. Sign Out\n"
         <<"\nPlease input the number associated with what you want from above: ";
   //Get the new userInput value
      getline(cin,inputLine);
   //check for valid response, 0 to 3, and assign it to variable
      userInput=menuValidityCheck(inputLine,3);
   }
}

/*
This function checks if the user has inputed a password that only contains alphanumeric characters.
Will return false if there are any non-alphanumeric characters in the passed string.

Parameters:
- string pw - the password that is being checked


CALLED BY: customerPortal
*/
bool isValid(string pw){
   bool valid = true;
    // check if the length is good
   if (pw.length() < 4)
   {
      valid = false;
   }
   else
   {
      for (int i=0 ; i<pw.length();i++)
      {
            // check for valid chars
         if (!isalnum(pw[i]))
         {
            valid = false;
         }
      }
   }
   return valid;
}
