// To start, I'm just gonna have it hardcoded to do babies subsplits

#include <iostream>
#include <fstream>
using namespace std;

int main()
{
	// Create and open debug output file
	ofstream debugOutput("debugOutput.txt");
	
	// Read splits file line by line
	string line = "";
	ifstream lss("Croc 2 - Any% (PC).lss");
	while (getline(lss, line))
	{
		
	}
	
	// Close files
	debugOutput.close();
	lss.close();
	
	return 0;
}
