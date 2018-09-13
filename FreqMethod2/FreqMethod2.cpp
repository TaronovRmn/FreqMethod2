// FreqMethod2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <array>

using namespace std;

string const SOLS_FILE_NAME = "ouR.txt";
string const TEMP_FILE_NAME = "tmp.txt";
string const STAT_FILE_NAME = "saR.txt";
const int COLS = 33;
const int ROWS = 9;

class Method //method statistic with name
{
public:
	bool setCell (int & i, int & j, int & dig)
	{
		if (i >= 0 && i < ROWS && j >= 0 && j < COLS) {
			table_[i][j] = dig; return true;
		}
		return false;
	}
	void setName(string & s) { name_ = s; }
	string getName() const { return name_; }
	int getCell(int & i, int & j) const { return table_[i][j]; }
private:
	int table_[ROWS][COLS];       //row: text length, overall texts, char errors
	string name_;
};

class Solution //solution with: letters, name, index
{
public:
	void setLetters(string & s)
	{
		letters_ = s;
	}

	void setName(string & name)
	{
		if (name.length() == 6) name_ = name;
		else if (name.length() == 5) name_ = name.insert(0, " ");
	}
	void setIndex(int & i) { index_ = i; }
	void setTreePosition(int & i) { treePosition_ = i; }
private:
	string letters_;
	int index_ = 0;
	int treePosition_ = 0;
	string name_;
};

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
		string line;
		vector<int> numbers;
		getline(fin, line);
		cMethod.setName(line);
		for (int i = 0; i < 4 + ROWS; i++)
		{
			skipLine(fin);
		}

		
		numbers.reserve(100);
		for (int i = 0; i < ROWS; i++)
		{
			getline(fin, line);
			numbers.clear();
			getNumbers(numbers, line);
			for (int iCol = 0; iCol < COLS; iCol++)
			{
				cMethod.setCell(i,iCol,numbers[iCol]);
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

void readSolutions(ifstream & fin, int & nMethods, vector<Solution> & solutions, vector<string> & fragNumbers)
{
	
	int i;
	string s;
	fin >> s;
	fragNumbers.push_back(s);
	for (int iS = 0; iS < nMethods; iS++)
	{
		Solution sol;
		fin >> s;
		sol.setLetters(s);
		fin >> i;
		sol.setIndex(i);
		fin >> i;
		sol.setTreePosition(i);
		fin >> s;
		sol.setName(s);
		solutions.push_back(sol);
	}

	 
}

void doResolve(ifstream & fin, ofstream & fout, int & nM, vector<int> & tLengths, vector<Method> & methods)
{
	int nFrags, nMethods;
	vector<string> fragNumbers;

	fin >> nFrags >> nMethods;
	fragNumbers.reserve(nFrags);
	if (nM != nMethods) { cout << "Method numbers does not match! (in" + SOLS_FILE_NAME + " & " + STAT_FILE_NAME + " files"; return ; }
	for (int iF = 0; iF < nFrags; iF++)
	{
		vector<Solution> solutions;
		solutions.reserve(nMethods);
		readSolutions(fin, nMethods, solutions, fragNumbers);
	}
}

int main()
{
	setlocale(LC_ALL, "rus");

	ifstream fin;
	ofstream fout;
	fin.open(STAT_FILE_NAME, ios_base::in);
	if (!fin.is_open())
		return 1;

	
	vector<int> tLengths;
	tLengths.reserve(2000);
	int nMethods;
	readLengths(fin, tLengths, nMethods);

	vector<Method> methods;
	methods.reserve(nMethods);
	readMethods(fin, methods, nMethods);

	fin.close(); //statistic reading complete

	fin.open(SOLS_FILE_NAME, ios_base::in);
	if (!fin.is_open())
		return 1;

	fout.open(TEMP_FILE_NAME, ios_base::out);

	doResolve(fin, fout, nMethods, tLengths, methods);





    return 0;
}

