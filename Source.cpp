
/*
ScheduleMakerWCSU - Caleb Garrick and Adam Jeniski
This application will take in a file path to a csv file and a name of a professor.
It will then save a json file (saved in the form of .csmo)
which will hold the attributes of that professor's schedule
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
	//time is in format hh:mm am/pm-hh:mm am/pm

	//(0) subject at index 3
	//(1) course at index 4
	//(2) section at index 5
	//(3) days at index 8
	//(4) time at index 9
	//(5) instructor at index 18

	//make ofstream that holds the json data

	//string startJson();
	//writeresult of startJson() to ofstream that holds json data

	unsigned int indexes[] = { 3, 4, 5, 8, 9, 18 };

	string line;

	ifstream inFile;
	inFile.open("Fall2019OpenClose.csv");

	if (!inFile.is_open()) {
		cout << "Unable to open file";
		exit(1); // terminate with error
	}

	

	while (getline(inFile, line)) {

		vector<string> vec;

		// token string into vec
		size_t found = line.find(",");
		size_t last = 0;
		while (found != string::npos) {
			vec.push_back(line.substr(last, found - last));;
			last = found + 1;
			found = line.find(",", last);
		}

		//print out tokens from this row
		for (string s : vec) {
			//here instead of printing, we should parse some data then pass the data into jsonifyMeeting()
			cout << s << "\n";
		}
	}
	inFile.close();

	//string endJson();
	//write result of endJson() to file

	return 0;
}

