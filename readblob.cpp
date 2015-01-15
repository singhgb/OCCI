#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <occi.h>
using namespace oracle::occi;
using namespace std;
int main () {
int size;
string username;
string password;
Environment *env;  
Connection *conn;
Statement *stmt;char* memblock;

username = "exp1";
password = "exp1";
env = Environment::createEnvironment (Environment::DEFAULT); 
try{
	conn = env -> createConnection(username, password, "");
	stmt = conn -> createStatement("SELECT IMAGE from TABLE01 where ID = 111");	ResultSet *rset = stmt->executeQuery ();
 	while (rset->next ())
      	{
        	Blob blob = rset->getBlob (1);
        	//Opening the blob in Read only mode
        	blob.open (OCCI_LOB_READONLY);
        	size = blob.length ();
       		//Length of the blob is: 
     	
		Stream *instream = blob.getStream (1,0); //opening stream to read blob
    		memblock = new char[size];
    		memset (memblock, NULL, size);

    		instream->readBuffer (memblock, size);
    		//dumping blob 
    
    		blob.closeStream (instream);
        	blob.close ();
      	}
      	stmt->closeResultSet (rset);

  	ofstream fileOut ("B.bmp", ios::out|ios::binary|ios::ate); //opening filestream for B.bmp
  		if (fileOut.is_open())
  			{
    			fileOut.write (memblock, size);
    			fileOut.close();
			}

    	conn->terminateStatement (stmt);
	delete memblock;
}
	catch (SQLException ea)
	 {
	cout<<ea.what();			
	}

//g++ -I /u01/app/oracle/product/12.1.0/dbhome_1/rdbms/public -o readblob  readblob.cpp -L /u01/app/oracle/product/12.1.0/dbhome_1/lib -locci -lm -lclntsh 

Environment::terminateEnvironment (env);
return 0;}
