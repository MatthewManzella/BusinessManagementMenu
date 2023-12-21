   /*
Inheritance Hierarchy:

BASE CLASS: Worker
   - Members: position, name, id, bossID, total sales
   - Functions: 
      - virtual getCommission
      - Getters for all of the above
      - incrtSales (adds onto total sales)
      
INHERITS FROM WORKER: Sales
   -  Only add on: getCommission = gettSales()*.05
   
INHERITS FROM SALES: SuperSales
   -  Only add on: getCommission = gettSales()*.065

INHERITS FROM SUPERSALES: Supervisor
   - Add ons:
      - new field: double empSales
      - getEmpSales()
      - incrEmpSales (adds onto empSales)
      - getCommission = gettSales()*.05 + getEmpSales()*.03
      
INHERITS FROM SUPERVISOR: Manager
   - only add on: getCommission = gettSales()*.05 + getEmpSales()*.03

*/
using namespace std;


class  Worker
{
private:
   string position;
   string name;
   string id;
   string bossID;
   double tSales;
public:
   Worker(string p,string n, string i,string b) {
      position = p;
      name = n;
      id = i;
      bossID = b;
      tSales = 0;
   }
   virtual double getCommission() = 0;

   string getPosition(){
      return position;
   }
   string getName(){
      return name;
   }

   string getID(){
      return id;
   }

   string getBossID(){
      return bossID;
   }
   
   double gettSales() {
      return tSales;
   }
   
   void incrSales(double t) {
      tSales += t;
   }

   ~Worker(){}
};



class Sales :public Worker
{

public:
   Sales(string p, string n, string i, string b):Worker( p,  n,  i, b) {}


   virtual double getCommission() {
      return gettSales() * .05;
   }

   ~Sales(){}
};



class  SuperSales:public Sales
{
public:
   SuperSales(string p, string n, string i, string b) :Sales(p, n, i, b) {}


   virtual double getCommission() {
      return gettSales() * .065;
   }
   ~SuperSales(){}
};



class  Supervisor:public SuperSales
{
private:
   double empSales;
   
   
public:
   Supervisor(string p, string n, string i, string b) :SuperSales(p, n, i, b) {
      empSales = 0;
   }
   void incrEmpSales(double e)
   {
      empSales += e;
   }

   double getEmpSales()
   {
      return empSales;
   }

   virtual double getCommission() {
      return gettSales() * .065 + empSales * 0.03;
   }
   ~Supervisor(){}
};



class  Manager:public Supervisor
{
public:
   Manager(string p, string n, string i, string b) :Supervisor(p, n, i, b) {}

   virtual double getCommission() {
      return gettSales() * .065 + getEmpSales() * 0.03;
   }
   
   ~Manager(){}
};

