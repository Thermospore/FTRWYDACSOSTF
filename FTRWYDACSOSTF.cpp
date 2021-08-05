// To start, I'm just gonna have it hardcoded to do babies subsplits
// Still needs to detect and ignore resets

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
	ifstream lss("target.lss");
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
			}
		}
	}
	
	// Search through array to find fastest run
	int fastestRunId = -1;
	double fastestTime = -1;
	for (int i = 0; i < 99999; i++)
	{
		if (run[i] != 0 &&
			(run[i] <= fastestTime || fastestTime == -1))
			{
				fastestRunId = i;
				fastestTime = run[i];
			}
	}
	
	// Print fastest run
	int fastestTime_hr = fastestTime / 60 / 60;
	int fastestTime_min = fastestTime / 60 - fastestTime_hr * 60;
	double fastestTime_sec = fastestTime - fastestTime_min * 60 - fastestTime_hr * 60 * 60;
	cout << "Fastest run: "
		<< fastestTime_hr << "h "
		<< fastestTime_min << "m "
		<< fastestTime_sec << "s / #"
		<< fastestRunId;
		
	// Checksum
	if ((((fastestTime_hr*60)+fastestTime_min)*60)+fastestTime_sec != fastestTime)
	cout << endl << "wait... maybe that's not right? time in s is " << fastestTime;
	
	// Close files
	debugOutput.close();
	lss.close();
	
	return 0;
}

