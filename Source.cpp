
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

vector<string> vectorizeString(string line);
bool isPhysical(vector<string> vec);

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
	//(6) instructors at index 18
	//(7) color

	vector<string> colors{ "#8DC028", "#2AFFE2", "#A1380F", "#3216EB", "#CC673E", "#53E29A", "#384E32", "#95D711", "#4EB3B1", "#3A7ECE"};
	vector<string>::iterator colorIter = colors.begin();

	cout << "Please input the name of a professor:\n>Sean Murthy\n";
	string prof("Sean Murthy");

	ofstream json; //a file that holds json data
	json.open("output.csmo");


	ifstream inFile;
	//inFile.open("Fall2019OpenClose.csv");
	inFile.open("test.csv");

	if (!inFile.is_open()) {
		cout << "Unable to open file";
		exit(1); // terminate with error
	}

	vector<vector<string>> classVec;
	string line;

	if (getline(inFile, line)) {
		vector<string> top;
		while (getline(inFile, line)) {
			vector<string> curr = vectorizeString(line);
			if (!top.empty()) {
				if (curr[0].size() < 1) {
					//curr is a second instance of the same class as top
					if (classVec.empty()) {
						classVec.push_back(top);
						classVec.push_back(curr);
						top.clear();
					}
					else {
						classVec.push_back(curr);
					}
				}
				else if (colorIter != colors.end() && classVec.size() >= 1) {
					//curr is not a second instance of the same class as top
					//json << jsonifyMeetings();
					classVec[0].push_back(*(colorIter++));
					cout << jsonifyMeeting(classVec, prof);
					classVec.clear();
				}
				else if (colorIter != colors.end()) {
					//curr is not a second instance of the same class as top
					//json << jsonifyMeetings();
					classVec.push_back(top);
					classVec[0].push_back(*(colorIter++));
					cout << jsonifyMeeting(classVec, prof);
					classVec.clear();
				}
			}
			else {
				if (!classVec.empty() && colorIter != colors.end()) {
					//json << jsonifyMeetings();
					classVec[0].push_back(*(colorIter++));
					cout << jsonifyMeeting(classVec, prof);
					classVec.clear();
				}
				if (curr[6].find(prof) != string::npos && isPhysical(curr)) {
					top = curr;
					classVec.push_back(top);
				}
				else {
					top.clear();
					curr.clear();
				}
			}
		}
	}
	inFile.close();
	json.close();
}


//returns a vecotr of data from a string
vector<string> vectorizeString(string line){
	vector<size_t> indexes{ 3, 4, 5, 8, 9, 17, 18 };
	vector<string> vec;
	vector<size_t>::iterator curr = indexes.begin();
	size_t last = 0;
	size_t found = line.find(",");
	size_t start_quote = line.find("\"", last);
	bool reached_end = false;
	for (size_t i = 0; found != string::npos || !reached_end; i++) {
		if (found == string::npos && last != string::npos) {
			//handle last cell in a line
			string temp = line.substr(last, line.size() - last);
			start_quote = temp.find("\"");
			if (start_quote == string::npos && line.size() > last) {
				//there no quotes
				vec.push_back(temp);
			}
			reached_end = true;
		}
		else if (start_quote < found) {
			// in a string
			size_t end_quote = line.find("\"", start_quote + 1);
			string temp = line.substr(start_quote + 1, end_quote - start_quote - 1);
			if (curr != indexes.end() && i == *curr) {
				vec.push_back(line.substr(start_quote + 1, end_quote - start_quote - 1));
				curr++;
			}
			last = end_quote + 1;
			start_quote = line.find("\"", end_quote + 1);
			found = line.find(",", last + 1);
		}
		else {
			// not in a string
			string temp = line.substr(last, found - last);
			if (curr != indexes.end() && i == *curr) {
				vec.push_back(line.substr(last, found - last));
				curr++;
			}
			last = found + 1;
			found = line.find(",", last);
		}
	}
	return vec;
}


//returns a bool of if the locaiton string is not "ONLINE" or "HYBRID"
bool isPhysical(vector<string> vec) {
	if (vec[5].find("ONLINE") == string::npos && vec[5].find("HYBRID") == string::npos) {
		return true;
	}
	return false;
}
