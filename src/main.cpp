
/*
ScheduleMakerWCSU - Caleb Garrick and Adam Jeniski
This application will take in a file path to a csv file and a name of a professor.
It will then save a json file (saved in the form of .csmo)
which will hold the attributes of that professor's schedule

Edge Cases:

	-Online classes are represented without a meeting day only, time=TBA, location=ONLINE


	-Classes which don't meet on a weekly basis
		Row in csv for each day of meeting
		represeted without a subject, course, or section

	-Classes meeting at different times during different days of the week
		We may get the profname and class time, but without classname we must look up until we find a name to pull
*/


#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <sstream>
#include "Utilities.h"


int main(int argc, char* argv[]) {

	//get csv vals

	//19 columns

	//Index mapping
	//(0) subject at index 3
	//(1) course at index 4
	//(2) section at index 5
	//(3) days at index 8
	//(3) days at index 8
	//(4) time at index 9
	//(5) location at index 17
	//(6) instructors at index 18
	//(7) color


	//important variables that can optionally be taken in via command line args
	string inputPath;
	string prof;
	string outputPath;

	//holds output in the form of json data (which is then read into a file)
	ofstream output;

	for (int argNum = 1; argNum < argc; argNum++) {
		if (strcmp(argv[argNum], "-i") == 0)
			inputPath = argv[++argNum];
		else if (strcmp(argv[argNum], "-p") == 0)
			if(validName(argv[++argNum]))
				prof = argv[argNum];
		else if (strcmp(argv[argNum], "-o") == 0)
			outputPath = argv[++argNum];
	}

	

	//take in name of professor for which we are making a schedule if we dont have it in command line args
	while(prof.empty()) {
		cout << "Please input the name of a professor:\n>";
		string rawInput;
		getline(cin, rawInput);
		if (!cin.good()) {
			cout << "Invalid Professor Name" << endl;
			cin.ignore();
			cin.clear();
		}
		else if (!validName(rawInput))
			cout << "Invalid Professor Name" << endl;
		else
			prof = rawInput;
	}
	

	//input file
	ifstream inFile;
	if (inputPath.empty())
		inputPath = "./data/fall2019.csv";

	inFile.open(inputPath);

	//check if file is unopenable
	if (!inFile.is_open()) {
		cout << "Unable to open file";
		return 1; // terminate with error
	}

	//for debugging/providing more data to user
	cout << "Input: '" << inputPath << "'\n";

	if (outputPath.empty())
		outputPath = "./output/" + prof + ".csmo";

	output.open(outputPath);

	if (!output.is_open()) {
		cout << "Unable to open file";
		return 1; // terminate with error
	}

	//for debugging/providing more data to user
	cout << "Output: '" << outputPath << "'\n";


	//put the starting chuck fo text into the output
	output << startJson(prof);


	parseData(inFile, output, prof);

	output << endJson();
	output.close();
	inFile.close();
}

