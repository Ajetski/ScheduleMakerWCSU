#include "Utilities.h"


using std::string;
using std::vector;

string startJson() {
	return string("{\n\"dataCheck\": \"69761aa6-de4c-4013-b455-eb2a91fb2b76\",\n\"saveVersion\" : 4,\n\"schedules\" : [\n");
}
string endJson() {
	return string("\n\n],\n\"currentSchedule\": 0\n}");
}
string jsonifyMeeting(vector<string> data) {// to be implemented // need to add arguments based on the parsed data 
											// generates a single meeting block to be added to the schedule
	string title = ("");
	return title;
}

string timeJsonify(string time) {
	//returns the json string from from a given time in format hh:mm am/pm-hh:mm am/pm (ignores preceding characters
	//also converts from 12 hr to 24 hr time
	string meetingTime = "";
	if (time.length()) {
		size_t div = time.find('-');
		string strtTime = time.substr(time.find(':')-2, div);
		string finTime = time.substr(div + 1);
		string strtHr = strtTime.substr(0, strtTime.find(':'));
		string finHr = finTime.substr(0,finTime.find(':'));

		if (strtTime.find("pm")!=string::npos) {
			strtHr = std::to_string(12 + stoi(strtTime.substr(0, 2)));
		}
		if (finTime.find("pm")!=string::npos) {
			finHr = std::to_string(12 + stoi(finTime.substr(0, 2)));
		}

		meetingTime.append("\"startHour\": " + strtHr +
			",\n\"endHour\": " + finHr +
			",\n\"startMinute\": " + strtTime.substr(strtTime.find(':')+1,2) +
			",\n\"endMinute\": " + finTime.substr(finTime.find(':') + 1, 2) +
			",\n");
	}
	

	return meetingTime;
	/*
		\"startHour\": ,\n
		\"endHour\": ,\n
		\"startMinute\": ,\n
		\"endMinute\": ,\n		
	*/
}

string daysJsonify(string days) {
	//returns the json string given up to 6 letters( MTWRFS) representing Monday through Saturday, or will return an empty string if the class does not meet
	//NOTE: could be efficiently used to check if a class does not meet
	vector<char> dayVals{'M','T','W','R','F','S' };
	vector<char>::iterator dayCharsIter = dayVals.begin();
	vector<string> daysFlags;
	string output = "";
	string::iterator daysStringIter = days.begin();
	// 1 for loop that goes through each char in list. if current char is equal to the curr iterator from dayVals,
	// push true, else push false; at end of loop inc iterator
	for (char weekDay : dayVals) {
		if (daysStringIter != days.end() && dayCharsIter != dayVals.end() && *daysStringIter == *dayCharsIter) {
			daysFlags.push_back("true");
			daysStringIter++;
		}
		else {
			daysFlags.push_back("false");
		}
		dayCharsIter++;
	}
	if (daysStringIter != days.begin()) {
		output.append("\"days\": {\n\"monday\": " + daysFlags[0] +
			",\n\"tuesday\": " + daysFlags[1] +
			",\n\"wednesday\": " + daysFlags[2] +
			",\n\"thursday\": " + daysFlags[3] +
			",\n\"friday\": " + daysFlags[4] +
			",\n\"saturday\": " + daysFlags[5] +
			",\n\"sunday\": false\n}\n}\n");
	}

	
	return output;
}

string daysJsonify2(string input) { //adams playground
	string output("");
	vector<string> days{"M", "T", "W", "R", "F", "S"};
	vector<string> format{ "\"days\": {\n\"monday\": ", ",\n\"tuesday\": ", ",\n\"wednesday\": ",
		",\n\"thursday\": ", ",\n\"friday\": ", ",\n\"saturday\": ", ",\n\"sunday\": false\n}\n}\n" };
	size_t last = 0;
	for (vector<string>::iterator daysIter = days.begin(), formatIter = format.begin(); daysIter != days.end() && formatIter != format.end(); daysIter++) {
		size_t found = input.find(*daysIter, last);
		if (found != string::npos) {
			output.append(*formatIter + "true");
			last = found;
		}
		else {
			output.append(*formatIter + "false");
		}
	}

	return output;
}

/*
	Scratch paper

	//input in form [subjectHead,courseNum,sectionNum,days(up to MTWRFS),times(hh:mm am/pm-hh:mm am/pm),[primProf (P), nextProf]]

	this chunk is generated once per class section
	{\n
		\"uid\": \"79c8fe46-035e-4579-b2d8-5f1c2b96f3a0\",\n
		\"type\": \"Course\",\n
		\"title": \"CS 205\",\n
		\"meetingTimes\": [\n



			This chunk is generated for each unique timeslot

			{\n
				\"uid\": \"866e9185-4a29-47ae-a4b0-0423e633c8be\",\n
				\"courseType\": \"\",\n
				\"instructor\": \"\",\n
				\"location\": \"\",\n

				\"startHour\": ,\n
				\"endHour\": ,\n
				\"startMinute\": ,\n
				\"endMinute\": ,\n

				\"days\": {\n
				\"monday\": true,\n
				\"tuesday\": false,\n
				\"wednesday\": false,\n
				\"thursday\": true,\n
				\"friday\": false,\n
				\"saturday\": false,\n
				\"sunday\": false\n

				}\n
			}\n


	This chunk is generated after all meeting times for a course are read
	will be generated in colorJsonify

		],\n
		\"backgroundColor\": \"#FFE37D\"\n
	}\n
}\n
*/
