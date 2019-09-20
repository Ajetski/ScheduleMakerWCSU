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

//Assumption: we don't need to worry about whether class is full semester or not

string timeJsonify() {//returns the json string from the 
	return string("\"startHour\": ,\n\"endHour\": ,\n\"startMinute\": ,\n\"endMinute\": ,\n");
}

string daysJsonify(string days) {//returns the json string given up to 6 letters( MTWRFS) representing Monday through Saturday
	if(days.length())
	char* cstrDays = new char[days.size() - 1];
	strcpy(cstrDays, days.c_str());
	vector<bool> daysFlags;
	string output = "";
	output << "\"days\": {\n\"monday\":";
	int pos = 0;
	for (char a in cstrDays) {
		while (a != '\0') {
			if (a == 'M') {
				daysFlags.push_back(true);
				pos++;
				break;
			}
			else if (a == 'T') {
				for (i = pos; i < 1; i++) {
					daysFlag.push_back(false);
				}
				daysFlags.push_back(true);
				pos++;
				break;
			}
			else if (a == 'W') {
				for (i = pos; i < 2; i++) {
					daysFlag.push_back(false);
				}
				daysFlags.push_back(true);
				pos++;
				break;
			}
			else if (a == 'R') {
				for (i = pos; i < 3; i++) {
					daysFlag.push_back(false);
				}
				daysFlags.push_back(true);
				pos++;
				break;
			}
			else if (a == 'F') {
				for (i = pos; i < 4; i++) {
					daysFlag.push_back(false);
				}
				daysFlags.push_back(true);
				pos++;
				break;
			}
			else if (a == 'S') {
				for (i = pos; i < 5; i++) {
					daysFlag.push_back(false);
				}
				daysFlags.push_back(true);
				pos++;
				break;
			}
		}
	}
	
	return "";

	/*
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
	*/
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

		],\n
		\"backgroundColor\": \"#FFE37D\"\n
	}\n
}\n
*/



