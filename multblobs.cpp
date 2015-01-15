#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <occi.h>
using namespace oracle::occi;
using namespace std;
void storeImageAsBlob(Connection* conn, char* imgFileName, long identifier, string tableName);
void readBlobAsImage(Connection* conn, long identifier, string tableName, char* imgFileName);

int main (int argc, char* argv[]) {

Environment *env;
Connection *conn;
clock_t t1,t2,diff;

string username = argv[1];
string password = argv[2];
int startIndex = atoi (argv[3]);
int endIndex = atoi (argv[4]);


env = Environment::createEnvironment (Environment::DEFAULT); 
conn = env -> createConnection(username, password, "");

cout << "starting timer ..." << endl;
t1=time(NULL);

for (long i=startIndex; i<endIndex;i++){
	if (i%75 == 0){
		sleep(1);
		conn->commit();
		cout<<"check1"<<endl;
	}
	storeImageAsBlob(conn, (char*)"A000000.bmp",i,"TABLE01");
}
conn->commit();
sleep(5);
for (long i=startIndex; i<endIndex;i++){
	if (i % 75 == 0){
		sleep(1);
		conn->commit();
		cout<<"check2"<<endl;
	}
	storeImageAsBlob(conn, (char*)"A000000.bmp",i,"TABLE02");
}
conn->commit();
sleep(5);
for (long i=startIndex; i<endIndex;i++){
	if (i % 75 == 0){
		sleep(1);
		conn->commit();
		cout<<"check3"<<endl;
	}
	storeImageAsBlob(conn, (char*)"A000000.bmp",i,"TABLE03");
}
conn->commit();

t2=time(NULL);
cout << "timer ended..." << endl;
diff = difftime (t2, t1);
//float seconds = (float) diff/CLOCKS_PER_SEC;
//cout << CLOCKS_PER_SEC << endl;
cout <<diff<<endl;
Environment::terminateEnvironment (env);

//g++ -I /oracle/product/11.2.0/dbhome_1/rdbms/public -o multblobs multblobs.cpp -L /oracle/product/11.2.0/dbhome_1/lib -locci -lm -lclntsh

//g++ -I /u01/app/oracle/product/12.1.0/dbhome_1/rdbms/public -o multblobs multblobs.cpp -L /u01/app/oracle/product/12.1.0/dbhome_1/lib -locci -lm -lclntsh 

return 0; }

void storeImageAsBlob(Connection *conn, char* imgFileName, long identifier, string tableName)
{
Statement *stmt;
string sqlstmt;
sqlstmt = "insert into " + tableName + " (id,image) VALUES(:x,:y)";
try{
	 
	stmt = conn -> createStatement(sqlstmt); //what you are sending to the connection
	stmt -> setInt(1,identifier);
	streampos size;
  	char * memblock;
  	ifstream fileIn (imgFileName, ios::in|ios::binary|ios::ate); //This block of code is reading in the file A.bmp
  		if (fileIn.is_open())
  			{
    			size = fileIn.tellg();
    			memblock = new char [size];
    			fileIn.seekg (0, ios::beg);
    			fileIn.read (memblock, size);
    			fileIn.close();
			}

	Bytes *image = new Bytes((unsigned char*) memblock, size); //creates new object using paramaters given

	
	stmt -> setBytes(2, *image); 
	stmt -> executeUpdate();
	conn->commit();
    	conn->terminateStatement (stmt);
	delete[] memblock;
	if (identifier == 1) 
		cout << "Reached end after record 1" << endl;
}
catch (SQLException ea)
 {
	cout<<ea.what();			
}
}

void readBlobAsImage(Connection *conn, long identifier, string tableName, char* imgFileName)
{

char txt[32];
sprintf(txt, "%d", identifier);
Statement *stmt;
int size;
char* memblock;
string sqlstmt;
sqlstmt = "SELECT IMAGE from " + tableName + " where ID = " + txt;
try{
	stmt = conn -> createStatement(sqlstmt);	ResultSet *rset = stmt->executeQuery ();
 	while (rset->next ())
      	{
        	Blob blob = rset->getBlob (1);
        	//Opening the blob in Read only mode
        	blob.open (OCCI_LOB_READONLY);
        	size = blob.length();
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

  	ofstream fileOut (imgFileName, ios::out|ios::binary|ios::ate); //opening filestream for B.bmp
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
}

