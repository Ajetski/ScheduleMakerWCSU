#include <string>
#include <fstream>
#include <vector>
#include <iostream>
using std::string;
using std::vector;

string startJson(string prof);
string endJson();
string daysJsonify(string weekdays);
string timeJsonify(string time);
string jsonifyMeeting(vector<vector<string>> data, string prof);

enum CSV_Index { CSV_SUBJECT = 3, CSV_COURSE = 4, CSV_SECTION = 5, CSV_DAYS = 8, CSV_TIME = 9, CSV_LOCATION = 17, CSV_INSTRUCTOR = 18 };
enum Vec_Index { SUBJECT = 0, COURSE = 1, SECTION = 2, DAYS = 3, TIME = 4, LOCATION = 5, INSTRUCTOR = 6, COLOR = 7 };