Business Management Menu:

EXECUTE FROM finalmenu.cpp:

Users can perform the following operations:
  1. Add a Customer: Add new customers to the company records.
      - enter first name, last name, address, city, state, zip
      - id is auto-generated
      - default account password set to "tribbles"
  2  Search for a Customer: Locate customers by their last name or ID number.
  3. Sell Tribbles: Sell 1-5 tribbles to any existing customer in the company records.
      - enter customer id, quantity to be purchased, sales person ID (if applicable)
      - confirmation is printed
  4. Manage Rainbow Tribble Sales: Handle sales and waitlisting for rainbow tribbles.
      - user can view the waitlist, add a non-repeated customer to the waitlist, or sell to 
      the next person in line
  5. Print Sales Report: Generate a comprehensive sales report for all sales staff members.
      - Showcase name, id, total sales, and commission
  6. Customer Portal: Allow customers to log in using their ID/password to review order history, place orders, or modify their password.


**Upon the start of the program, data is read into vectors from .txt files and stored as objects (see bwlo) for manipulation from the options chosen above (customerList, orderList, transactionList, rainbowList (string IDs only), staffList)
**Upon termination, all the data is written back out to respective files with any changes made during the program's run.

  Classes:
    1. customer 
    2. order 
    3. transaction
    4. Worker (inheritance hierarchy where Worker <- Sales <- SuperSales <- Supervisor <- Manager)

*New transaction and order objects are created with every customer sale
*New customer object created for every customer added
*No new sales staff are added in the program
