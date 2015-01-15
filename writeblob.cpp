#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <occi.h>
using namespace oracle::occi;
using namespace std;
int main () {
string username;
string password;
Environment *env;
Connection *conn;
Statement *stmt;
username = "exp1";
password = "exp1";
env = Environment::createEnvironment (Environment::DEFAULT); //allows connection to database
try{
	conn = env -> createConnection(username, password, ""); 
	stmt = conn -> createStatement("insert into Table01(id,image) VALUES(:x,:y)"); //what you are sending to the connection
	stmt -> setInt(1,111);
	streampos size;
  	char * memblock;
  	ifstream fileIn ("A.bmp", ios::in|ios::binary|ios::ate); //This block of code is reading in the file A.bmp
  		if (fileIn.is_open())
  			{
    			size = fileIn.tellg();
    			memblock = new char [size];
    			fileIn.seekg (0, ios::beg);
    			fileIn.read (memblock, size);
    			fileIn.close();
			}

	Bytes *image = new Bytes((unsigned char*) memblock, size); //Converts 

	
	stmt -> setBytes(2, *image); 
	stmt -> executeUpdate();
	conn->commit();
    	conn->terminateStatement (stmt);
}
	catch (SQLException ea)
	 {
	cout<<ea.what();			
	}

//g++ -I /u01/app/oracle/product/12.1.0/dbhome_1/rdbms/public -o writeblob  writeblob.cpp -L /u01/app/oracle/product/12.1.0/dbhome_1/lib -locci -lm -lclntsh 

Environment::terminateEnvironment (env);
return 0;}
