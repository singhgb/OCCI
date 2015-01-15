// reading an entire binary file
#include <iostream>
#include <fstream>
using namespace std;

int main () {
  streampos size;
  char * memblock;

  ifstream fileIn ("A.bmp", ios::in|ios::binary|ios::ate);
  ofstream fileOut ("B.bmp", ios::out|ios::binary|ios::ate);
  if (fileIn.is_open())
  {
    size = fileIn.tellg();
    memblock = new char [size];
    fileIn.seekg (0, ios::beg);
    fileIn.read (memblock, size);
    fileIn.close();

    cout << "the entire file content is in memory";

  }
  else cout << "Unable to open file";

  if (fileOut.is_open())
  {
    //fileOut.seekg (0, ios::beg);
    fileOut.write (memblock, size);
    fileOut.close();

    cout << "the entire file content is in written";

    delete[] memblock;
  }
  else cout << "Unable to open output  file";
  return 0;
}
