
/*
ScheduleMakerWCSU - Caleb Garrick and Adam Jeniski
This application will take in a file path to a csv file and a name of a professor.
It will then save a json file (saved in the form of .csmo)
which will hold the attributes of that professor's schedule

Edge Cases:
	-Classes which don't meet on a weekly basis
		Row in csv for each day of meeting

	-Classes meeting at different times during different days of the week
		We may get the profname and class time, but without classname we must look up until we find a name to pull
*/


#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "Utilities.h"



/*
using std::ifstream;
using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::ios;*/

using namespace std;

int main() {

	//get csv vals
	
	//19 columns

	//Index mapping
	//(0) subject at index 3
	//(1) course at index 4
	//(2) section at index 5
	//(3) days at index 8
	//(4) time at index 9
	//(5) location at index 17
	//(6) instructor at index 18

	ofstream json; //a file that holds json data
	json.open("output.csmo");
	json << "Writing this to a file.\n";
	
	
	//json << startJson(json)";

	unsigned int indexes[] = { 3, 4, 5, 8, 9, 17, 18 };

	string line;

	ifstream inFile;
	//inFile.open("Fall2019OpenClose.csv");
	inFile.open("test.csv");

	if (!inFile.is_open()) {
		cout << "Unable to open file";
		exit(1); // terminate with error
	}

	

	while (getline(inFile, line)) {

		vector<string> vec;

		// token string into vec
		size_t last = 0;
		size_t found = line.find(",");
		size_t found_string_open = line.find("\"", 0);
		size_t found_string_close= line.find("\"",found_string_open+1);
		

		//still need to solve issue of values potentially being strings with "," inside
		for (unsigned int i = 0; found != string::npos; i++) {
			if (found_string_open < found && found < found_string_close) {
				if (i == 3 || i == 4 || i == 5 || i == 8 || i == 9 || i == 17 || i == 18) {
					vec.push_back(line.substr(found_string_open+1,(found_string_close-found_string_open)-1));
				}
				last = found_string_close + 1;
				found = line.find(",", last);
			}
			else {
				// not in a string
				if (i == 3 || i == 4 || i == 5 || i == 8 || i == 9 || i==17 || i == 18) {
					vec.push_back(line.substr(last, found - last));
				}
				last = found + 1;
				found = line.find(",", last);
			}
			
		}

		cout << vec.size() << "\n";

		//print out tokens from this row
		for (size_t i = 0; i < vec.size(); i++) {
			cout << vec[i] << endl;
		}
	}
	inFile.close();

	json << endJson();
	json.close();

	return 0;
}

