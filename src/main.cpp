
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


using std::ifstream;
using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::ios;
using std::stringstream;
using std::strlen;


vector<string> splitRow(string line);
bool isPhysical(vector<string> vec);
bool validName(string str);

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
	stringstream output;

	for (int argNum = 1; argNum < argc; argNum++) {
		if (strcmp(argv[argNum], "-i") == 0)
			inputPath = argv[++argNum];
		else if (strcmp(argv[argNum], "-p") == 0)
			if(validName(argv[++argNum]))
				prof = argv[++argNum];
		else if (strcmp(argv[argNum], "-o") == 0)
			outputPath = argv[++argNum];
	}

	

	vector<string> colors{ "#8DC028", "#2AFFE2", "#A1380F", "#CC673E", "#53E29A",
		"#95D711", "#4EB3B1", "#103D5B", "#3A7ECE", "#BD4433", "#E16D5C", "#933AB2", "#A78335",
		"#ED5471", "#6DC81E", "#7DC4FE", "#3C7AA0", "#3216EB", "#384E32", "#A78335" };

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
	
	//put the starting chuck fo text into the stringstream
	output << startJson(prof);

	//input file
	ifstream inFile;
	if (inputPath.empty())
		inputPath = "./data/fall2019.csv";


	//sectionTable holds all of the rows that descirbe a given secion. It is then used to jsonify all of the instances of that class
	vector<vector<string>> sectionTable;

	//line holds 1 line of the csv file at a time
	string line;

	//top is used to check and see if there are multiple instances of the same class
	vector<string> top;
	//first will be true until after the first meeting is added to the output stringstream
	bool first = true;

	inFile.open(inputPath);

	//check if file is unopenable
	if (!inFile.is_open()) {
		cout << "Unable to open file";
		return 1; // terminate with error
	}

	while (getline(inFile, line)) {
		vector<string> curr = splitRow(line);

		//for debugging
		/*for (const string& s: curr)
			cout << "'" << s << "' ";
		cout << endl;*/

		if (!top.empty()) {
			if (curr[SUBJECT].empty()) {
				//curr is a second instance of the same class as top
				if (sectionTable.empty()) {
					if (isPhysical(top))
						//if top is a real class, we should add it to sectionTable
						sectionTable.push_back(top);
					if (isPhysical(curr))
						//if curr is a real class, we should add it to calssVec
						sectionTable.push_back(curr);
					//if top is valid, it is now in sectionTable, and else it is not
					//we should now clear top, as it is defined outside of this scope and will persist. (curr is defined in this loop)
					top.clear();
				}
				else
					//sectionTable is empty and curr is a second instance of the class in top
					if (isPhysical(curr))
						sectionTable.push_back(curr);
			}
			else if (!colors.empty() && !sectionTable.empty()) {
				//curr is not a second instance of the same class as top
				sectionTable[0].push_back(colors.back());
				colors.pop_back();
				if (!first)
					output << ",\n";
				first = false;
				output << jsonifyMeeting(sectionTable, prof);
				sectionTable.clear();
				top.clear();
				if (curr[6].find(prof) != string::npos && isPhysical(curr)) {
					top = curr;
					sectionTable.push_back(top);
				}
			}
			else if (!colors.empty() && isPhysical(top)) {
				//curr is not a second instance of the same class as top
				sectionTable.push_back(top);
				sectionTable[0].push_back(colors.back());
				colors.pop_back();
				if (!first)
					output << ",\n";
				first = false;
				output << jsonifyMeeting(sectionTable, prof);
				sectionTable.clear();
				top.clear();
			}
		}
		else {
			if (!sectionTable.empty() && !colors.empty()) {
				sectionTable[0].push_back(colors.back());
				colors.pop_back();
				if (!first)
					output << ",\n";
				first = false;
				output << jsonifyMeeting(sectionTable, prof);
				sectionTable.clear();
				top.clear();
			}
			if (curr[6].find(prof) != string::npos && isPhysical(curr)) {
				top = curr;
				sectionTable.push_back(top);
			}
			else {
				top.clear(); 
				curr.clear();
			}
		}
	}
	output << endJson();
	inFile.close();
	std::ofstream json; //a file that holds json data
	if (outputPath.empty())
		json.open("./output/" + prof + ".csmo");
	else
		json.open(outputPath);
	json << output.rdbuf();
	json.close();
	inFile.close();
}


//returns a vector of data from a string
vector<string> splitRow(string line) {
	vector<CSV_Index> indexes{CSV_SUBJECT, CSV_COURSE, CSV_SECTION, CSV_DAYS, CSV_TIME, CSV_LOCATION, CSV_INSTRUCTOR};
	vector<string> vec;
	string::size_type last{ 0 };
	string::size_type found{ line.find(",") };
	string::size_type start_quote{ line.find("\"", last) };
	bool reached_end = false;
	for (string::size_type i{ 0 }; found != string::npos || !reached_end; i++) {
		if (found == string::npos && last != string::npos) {
			//handle last cell in a line
			string temp(line.substr(last, line.size() - last));
			start_quote = temp.find("\"");
			if (start_quote == string::npos && line.size() > last)
				//there no quotes
				vec.push_back(temp);
			reached_end = true;
		}
		else if (start_quote < found) {
			// in a string
			string::size_type end_quote = line.find("\"", start_quote + 1);
			string temp = line.substr(start_quote + 1, end_quote - start_quote - 1);
			if (!indexes.empty() && i == indexes.front()) {
				vec.push_back(line.substr(start_quote + 1, end_quote - start_quote - 1));
				indexes.erase(indexes.begin());
			}
			last = end_quote + 1;
			start_quote = line.find("\"", end_quote + 1);
			found = line.find(",", last + 1);
		}
		else {
			// not in a string
			string temp = line.substr(last, found - last);
			if (!indexes.empty() && i == indexes.front()) {
				vec.push_back(line.substr(last, found - last));
				indexes.erase(indexes.begin());
			}
			last = found + 1;
			found = line.find(",", last);
		}
	}
	return vec;
}


//returns a bool of if the locaiton string is not "ONLINE" or "HYBRID"
bool isPhysical(vector<string> vec) {
	if (vec[5].find("ONLINE") == string::npos && vec[5].find("HYBRID") == string::npos) 
		return true;
	return false;
}

//returns a bool expression for if a string is a valid name
bool validName(string str) {
	bool validFlag=true;
	if (str.length() == 0)
		validFlag=false;
	for (char chr : str)
		if ((chr < 'A' || chr > 'Z') && (chr < 'a' || chr > 'z') && (chr != '.') && (chr != ' ') && (chr != '-'))
			validFlag = false;
	return validFlag;
}
