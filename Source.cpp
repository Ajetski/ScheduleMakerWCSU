#define _CRT_SECURE_NO_WARNINGS
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

	string json = "";

	//string startJson();
	//json += startJson(); //will append the output of startJson to the json string obj

	unsigned int indexes[] = { 3, 4, 5, 8, 9, 18 };

	string line;

	ifstream inFile;
	inFile.open("Fall2019OpenClose.csv");

	if (!inFile.is_open()) {
		cout << "Unable to open file";
		exit(1); // terminate with error
	}

	unsigned int row = 0;

	while (getline(inFile, line)) {
		//line is a string object with the curr line
		//we need to convert line into a list of string objects via tokening
		//we will delimit by ','

		
		unsigned int col = 0;
		unsigned int curr_idx;

		char* str = new char[line.size()];
		strcpy(str, line.c_str());

		char* arr = new char[sizeof(indexes)];

		//delimit line by ',' then add values to vector if index is in indexes
		char* pch;
		pch = strtok(str, ",");

		while (pch != NULL)
		{
			pch = strtok(NULL, ",");

			if (val_in(col, indexes, sizeof(indexes) / sizeof(unsigned int))) {
				strcpy(arr, pch);
				cout << pch << "\n";
			}

			col++;
		}

		//string jsonifyMeeting(char* arr, int length_of_arr);
		//json += jsonifyMeeting(arr, some_int);
	

		delete[] str; delete[] arr;
		row++;
	}
	inFile.close();

	//string endJson();
	//json += endJson(); //will append the output of endJson to the json string obj

	return 0;
}

