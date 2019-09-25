#include "Utilities.h"


using std::string;
using std::vector;

string startJson(string prof) {
	string title = ("'" + prof + "'s Schedule" + "'");
	return string("{\n\"dataCheck\": \"69761aa6-de4c-4013-b455-eb2a91fb2b76\",\n\"saveVersion\" : 4,\n\"schedules\" : [\n\"title\": " + title + ",\"items\" : [");
	/*
	{
	  "title": "",
	  "items": [
	  */
}

string endJson() {
	return string("\n\n],\n\"currentSchedule\": 0\n}");
}

//Index mapping
	//(0) subject at index 3
	//(1) course at index 4
	//(2) section at index 5
	//(3) days at index 8
	//(4) time at index 9
	//(5) location at index 17
	//(6) instructor at index 18
	//(7) color

string jsonifyMeeting(vector<vector<string>> data, string prof) {// generates a single meeting block to be added to the schedule
	//unfinished.
	//Plan:
	//goto source.cpp and add conditionals chekcing if my curr vector is worth parsing. (is it the correct prof)?
	if (data[0][4].find("TBA") != string::npos) { return string(""); }

	string output("{\n\"uid\": \"79c8fe46-035e-4579-b2d8-5f1c2b96f3a0\",\n\"type\": \"Course\",\n\"title\": " + data[0][0] + " " + data[0][1] + "-" + data[0][2] + string("\",\n\"meetingTimes\": [\n"));

	if (data.size() > 1) {
		for (int iter = 0; iter < data.size(); iter++) {
			output.append("{\n\"uid\": \"866e9185-4a29-47ae-a4b0-0423e633c8be\",\n\"courseType\": \"\",\n\"instructor\": \"\",\n\"location\": \"\",\n");
			output.append(timeJsonify(data[iter][4]));
			output.append(daysJsonify(data[iter][3]));
			if (iter < data.size() - 1) {
				output.append("\n},\n");
			}
		}
	}
	else {
		output.append("{\n\"uid\": \"866e9185-4a29-47ae-a4b0-0423e633c8be\",\n\"courseType\": \"\",\n\"instructor\": \"\",\n\"location\": \"\",\n");
		output.append(timeJsonify(data[0][4]));
		output.append(daysJsonify(data[0][3]));
	}

	output += "],\n\"backgroundColor\": \"" + data[0][7] + "\"\n}\n}\n";
	return output;
}

string timeJsonify(string time) {
	//returns the json string from from a given time in format hh:mm am/pm-hh:mm am/pm (ignores preceding characters
	//also converts from 12 hr to 24 hr time
	string meetingTime = "";
	if (time.length()) {
		size_t div = time.find('-');
		string strtTime = time.substr(time.find(':') - 2, div);
		string finTime = time.substr(div + 1);
		string strtHr = strtTime.substr(0, strtTime.find(':'));
		string finHr = finTime.substr(0, finTime.find(':'));

		if (strtTime.find("pm") != string::npos&& strtTime.substr(0, 2).compare("12")!=0) {
			strtHr = std::to_string(12 + stoi(strtTime.substr(0, 2)));
		}
		if (finTime.find("pm") != string::npos && finTime.substr(0, 2).compare("12") != 0) {
			finHr = std::to_string(12 + stoi(finTime.substr(0, 2)));
		}
		if (strtTime.find("am") != string::npos && strtTime.substr(0, 2).compare("12") != 0) {
			strtHr = std::to_string(12 + stoi(strtTime.substr(0, 2)));
		}
		if (finTime.find("am") != string::npos && finTime.substr(0, 2).compare("12") != 0) {
			finHr = std::to_string(12 + stoi(finTime.substr(0, 2)));
		}

		meetingTime.append("\"startHour\": " + strtHr +
			",\n\"endHour\": " + finHr +
			",\n\"startMinute\": " + strtTime.substr(strtTime.find(':') + 1, 2) +
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


string daysJsonify(string input) {
	string output("");
	vector<string> days{ "M", "T", "W", "R", "F", "S" };
	vector<string> format{ "\"days\": {\n\"monday\": ", ",\n\"tuesday\": ", ",\n\"wednesday\": ",
		",\n\"thursday\": ", ",\n\"friday\": ", ",\n\"saturday\": ", ",\n\"sunday\": false\n}\n" };
	size_t last = 0;
	vector<string>::iterator daysIter = days.begin(), formatIter = format.begin();
	for (; daysIter != days.end() && formatIter != format.end(); daysIter++, formatIter++) {
		size_t found = input.find(*daysIter, last);
		if (found != string::npos) {
			output.append(*formatIter + "true");
			last = found;
		}
		else {
			output.append(*formatIter + "false");
		}
	}
	if (formatIter != format.end()) {
		output.append(*formatIter);
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
		\"title\": \"CS 205\",\n
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
