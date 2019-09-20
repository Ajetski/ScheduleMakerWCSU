#include "Utilities.h"
using std::string;
using std::vector;

string startJson() {
	return string("{\n\"dataCheck\": \"69761aa6-de4c-4013-b455-eb2a91fb2b76\",\n\"saveVersion\" : 4,\n\"schedules\" : [\n{");
}
string endJson() {
	return string("}\n\n],\n\"currentSchedule\": 0\n}");
}
string jsonifyMeeting(vector<string> data) {// to be implemented // need to add arguments based on the parsed data
	return "";
}