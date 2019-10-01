#include "Utilities.h"

string startJson(string prof) {
	string title = ("\"Schedule for " + prof +  "\"");
	return string("{\n\"dataCheck\": \"69761aa6-de4c-4013-b455-eb2a91fb2b76\",\n\"saveVersion\" : 4,\n\"schedules\" : [{\n\"title\": " + title + ",\n\"items\" : [");
}

string endJson() {
	return string("\n]\n}\n],\n\"currentSchedule\": 0\n}\n");
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
	if (data[0][4].find("TBA") != string::npos)
		return string("");
	if (stoi(data[0][2]) < 10) {
		data[0][2] = "0" + data[0][2];
	}
	string output("{\n\"uid\": \"79c8fe46-035e-4579-b2d8-5f1c2b96f3a0\",\n\"type\": \"Course\",\n\"title\": \"" + data[0][0] + " " + data[0][1] + "-" + data[0][2] + string("\",\n\"meetingTimes\": [\n"));

	if (data.size() > 1) {
		for (size_t iter = 0; iter < data.size(); iter++) {
			if (data[iter][4].find("TBA") == string::npos) {
				output.append("{\n\"uid\": \"866e9185-4a29-47ae-a4b0-0423e633c8be\",\n\"courseType\": \"\",\n\"instructor\": \"\",\n\"location\": \"" + data[iter][5] + ("\",\n"));
				output.append(timeJsonify(data[iter][4]));
				output.append(daysJsonify(data[iter][3]));
				if (iter < data.size() - 1) {
					output.append("\n},\n");
				}
			}
		}
	}
	else {
		output.append("{\n\"uid\": \"866e9185-4a29-47ae-a4b0-0423e633c8be\",\n\"courseType\": \"\",\n\"instructor\": \"\",\n\"location\": \"" + data[0][5]  + string("\",\n"));
		output.append(timeJsonify(data[0][4]));
		output.append(daysJsonify(data[0][3]));
	}

	output += "}],\n\"backgroundColor\": \"" + data[0][7] + "\"\n}";
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
		if (strtTime.find("am") != string::npos && strtTime.substr(0, 2).compare("12") == 0) {
			strtHr = std::to_string(12 - stoi(strtTime.substr(0, 2)));
		}
		if (finTime.find("am") != string::npos && finTime.substr(0, 2).compare("12") == 0) {
			finHr = std::to_string(12 - stoi(finTime.substr(0, 2)));
		}

		strtHr = std::to_string(stoi(strtHr));
		finHr = std::to_string(stoi(finHr));
		string strtMin = std::to_string(stoi(strtTime.substr(strtTime.find(':') + 1, 2)));
		string finMin = std::to_string(stoi(finTime.substr(finTime.find(':') + 1, 2)));
		//casting to make 00 into 0

		meetingTime.append("\"startHour\": " + strtHr +
			",\n\"endHour\": " + finHr +
			",\n\"startMinute\": " + strtMin +
			",\n\"endMinute\": " + finMin +
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



void parseData(ifstream& inFile, ofstream& output, string& prof) {
	vector<string> colors{ "#8DC028", "#2AFFE2", "#A1380F", "#CC673E", "#53E29A",
		"#95D711", "#4EB3B1", "#103D5B", "#3A7ECE", "#BD4433", "#E16D5C", "#933AB2", "#A78335",
		"#ED5471", "#6DC81E", "#7DC4FE", "#3C7AA0", "#3216EB", "#384E32", "#A78335" };

	//sectionTable holds all of the rows that descirbe a given secion. It is then used to jsonify all of the instances of that class
	vector<vector<string>> sectionTable;

	//line holds 1 line of the csv file at a time
	string line;

	//top is used to check and see if there are multiple instances of the same class
	vector<string> top;
	//first will be true until after the first meeting is added to the output stringstream
	bool first = true;


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
}


//returns a vector of data from a string
vector<string> splitRow(string line) {
	vector<CSV_Index> indexes{ CSV_SUBJECT, CSV_COURSE, CSV_SECTION, CSV_DAYS, CSV_TIME, CSV_LOCATION, CSV_INSTRUCTOR };
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
	bool validFlag = true;
	if (str.length() == 0)
		validFlag = false;
	for (char chr : str)
		if ((chr < 'A' || chr > 'Z') && (chr < 'a' || chr > 'z') && (chr != '.') && (chr != ' ') && (chr != '-'))
			validFlag = false;
	return validFlag;
}