#include <string>
#include <fstream>
#include <vector>
#include <iostream>
using std::string;
using std::vector;

string startJson();
string endJson();
string daysJsonify(string weekdays);
string timeJsonify(string time);
string jsonifyMeeting(vector<string> data);