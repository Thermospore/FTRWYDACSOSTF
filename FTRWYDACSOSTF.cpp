// To start, I'm just gonna have it hardcoded to do babies subsplits

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

int main()
{
	// Create and open debug output file
	ofstream debugOutput("debugOutput.txt");
	
	// Initialize vars
	bool inStretch = false; // true when you are in the stretch you are timing
	bool inSegmentHistory = false; // true when you are in a segment history tag
	int curId = -1; // current run id. -1 when not inside a <Time> tag
	double run[99999]; // compiler is being an absolute bitch in every imaginable way,
	for (int i = 0; i < 99999; i++) // so I'm just going to have a massive array whatever geez
		run[i] = 0; // stores the time in seconds for the stretch you are timing for each run id
	
	// Read splits file line by line
	string line = "";
	ifstream lss("Croc 2 - Any% (PC).lss");
	while (getline(lss, line))
	{
		// Flag when you are in the stretch you are timing
		if (line.find("<Name>-Babies A</Name>") != -1)
			inStretch = true;
		if (line.find("<Name>-</Name>") != -1)
			inStretch = false;
		
		// Flag when you are in a segment history tag
		if (line.find("<SegmentHistory>") != -1)
			inSegmentHistory = true;
		if (line.find("</SegmentHistory>") != -1)
			inSegmentHistory = false;
		
		// Extract times
		if (inStretch && inSegmentHistory)
		{
			// Store run id
			if (line.find("<Time id=\"") != -1)
			{
				int pos = line.find("<Time id=\"") + 10;
				int len = line.find("\"", pos) - pos;
				stringstream ss;
				ss << line.substr(pos,len);
				ss >> curId;
			}
			// Clear run id once we leave the current <Time> tag
			else if (line.find("</Time>") != -1)
				curId = -1;
			// Add time to run, for the current run id
			else if (line.find("<GameTime>") != -1 && curId != -1)
			{
				// Gradually shave off the line and extract time as you go
				double time = 0;
				double ssReciever = 0;
				string SHAVEITUP = line;
				
				// Hours
				SHAVEITUP = SHAVEITUP.substr(SHAVEITUP.find("<GameTime>") + 10);
				stringstream hrs;
				hrs << SHAVEITUP.substr(0,SHAVEITUP.find(":"));
				hrs >> ssReciever;
				time += ssReciever*60*60;
				
				// Minutes
				SHAVEITUP = SHAVEITUP.substr(SHAVEITUP.find(":") + 1);
				stringstream mins;
				mins << SHAVEITUP.substr(0,SHAVEITUP.find(":"));
				mins >> ssReciever;
				time += ssReciever*60;
				
				// Seconds
				SHAVEITUP = SHAVEITUP.substr(SHAVEITUP.find(":") + 1);
				stringstream secs;
				secs << SHAVEITUP.substr(0,SHAVEITUP.find("</GameTime>"));
				secs >> ssReciever;
				time += ssReciever;
				
				// Add time to run
				run[curId] += time;
				
				debugOutput<<curId<<"/"<<time<<"/"<<run[curId]<<"|";
			}
		}
		
		debugOutput<<line<<endl;
	}
	
	// Close files
	debugOutput.close();
	lss.close();
	
	return 0;
}

