
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

	//vector<unsigned int> indexes{ 3, 4, 5, 8, 9, 17, 18 };
	vector<size_t> indexes{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	

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
		vector<size_t>::iterator curr = indexes.begin();
		size_t last = 0;
		size_t found = line.find(",");
		size_t start_quote = line.find("\"", last);
		

		//still need to solve issue of values potentially being strings with "," inside
		for (size_t i = 0; found != string::npos; i++) {
			if (start_quote < found) {
				size_t end_quote = line.find("\"", start_quote + 1);
				if (curr != indexes.end() && i == *curr) {
					vec.push_back(line.substr(start_quote, end_quote));
					curr++;
				}
				last = end_quote + 1;
				start_quote = line.find("\"", end_quote + 1);
				found = line.find(",", last);
			}			
			else {
				// not in a string
				if (curr != indexes.end() && i == *curr) {
					vec.push_back(line.substr(last, found - last));
					curr++;
				}
				last = found + 1;
				found = line.find(",", last);
			}
		}

		cout << vec.size() << "\n";

		//print out tokens from this row
		for (vector<string>::iterator i = vec.begin(); i != vec.end(); i++) {
			cout << *i << "\n";
		}
		cout << endl;
	}
	inFile.close();

	json << endJson();
	json.close();

	return 0;
}

