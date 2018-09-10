// FreqMethod2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <array>

using namespace std;

string const SOLS_FILE_NAME = "ouR.txt";
string const TEMP_FILE_NAME = "tmp.txt";
string const STAT_FILE_NAME = "saR.txt";
const int COLS = 33;
const int ROWS = 9;

struct Method 
{
	
	int stats[ROWS][COLS];       //row: text length, overall texts, char errors
	string name;
};

/* void read_methods(ifstream & fin, vector<Method> & methods)
{
	int n_frags = 0, n_meths = 0;
	string stemp;
	

	fin >> n_frags >> n_meths;
	for (int i = 0; i < n_frags; ++i)
	{
		getline(fin, stemp);
		for (int j = 0; j < (n_meths * 2); ++j)
		{
			getline(fin, stemp);
		}
	}
	
	for (int im = 0; im < n_meths; ++im)
	{
		Method method;
		getline(fin, stemp);
		getline(fin, stemp); //method name is here
		getline(fin, stemp);
		for (int j = 0; j < ROWS + 1; ++j)
		{
			getline(fin, stemp);
		}
		getline(fin, stemp);
		getline(fin, stemp);
		for (int i = 0; i < ROWS; ++i)
		{
			for (int j = 0; j < COLS; ++j)
			{
				fin >> method.stats[i][j];
			}
		}
		methods.push_back(method);
		
	}
	

}
*/
void skipLine (ifstream & fin)
{
	fin.ignore(numeric_limits<streamsize>::max(), '\n');
}
void getNumbers(vector<int> & result, const string & s) { //get numbers from string
	bool found = false;
	int number = 0;

	for (string::size_type i = 0; i < s.length(); i++) {
		const char ch = s[i];
		if (ch >= '0' && ch <= '9') {
			const int digit = ch - '0';
			number = number * 10 + digit;
			found = true;
		}
		else {
			if (found) {
				result.push_back(number);

				number = 0;
				found = false;
			}
		}
	}

	if (found) {
		result.push_back(number);
	}
}
void readMethods(ifstream & fin, vector<Method> & methods, int & nMethods) //reading error statistic table about methods
{
	skipLine(fin);
	for (int iM = 0; iM < nMethods; iM++) {
		Method cMethod;
		getline(fin, cMethod.name);
		for (int i = 0; i < 4 + ROWS; i++)
		{
			skipLine(fin);
		}

		string line;
		vector<int> numbers;
		numbers.reserve(100);
		for (int i = 0; i < ROWS; i++)
		{
			getline(fin, line);
			numbers.clear();
			getNumbers(numbers, line);
			for (int iCol = 0; iCol < COLS; iCol++)
			{
				cMethod.stats[i][iCol] = numbers[iCol];
			}
		}
		skipLine(fin);
		methods.push_back(cMethod);
		
	}
}

void readLengths(ifstream & fin, vector<int> & lengths, int & nMethods) //reading lengths of fragments
{
	int nFrags;
	char c;
	fin >> nFrags >> nMethods >> c;
	string line;
	vector<int> numbers;
	numbers.reserve(2);
	for (int iF = 0; iF < nFrags; iF++)
	{
		getline(fin, line);
		numbers.clear();
		getNumbers(numbers, line);
		lengths.push_back(numbers[1]);
		for (int iM = 0; iM < nMethods*2 + 1; iM++)
		{
			skipLine(fin);
		}

	}
	
	
	

}
int main()
{
	setlocale(LC_ALL, "rus");

	ifstream fin(STAT_FILE_NAME);
	if (!fin.is_open())
		return 1;

	
	vector<int> tLengths;
	tLengths.reserve(2000);
	int nMethods;
	readLengths(fin, tLengths, nMethods);

	vector<Method> methods;
	methods.reserve(20);
	readMethods(fin, methods, nMethods);

	fin.close(); //statistic reading complete




    return 0;
}

