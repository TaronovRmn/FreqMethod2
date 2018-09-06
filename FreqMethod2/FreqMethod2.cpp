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
string const STAT_FILE_NAME = "stat.txt";
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
void readMethods(ifstream & fin, array<Method, 20> & methods) //reading error statistic table about methods
{
	
}
void readLengths(ifstream & fin, array<int, 2000> & lengths) //reading lengths of fragments
{

}
int main()
{
	setlocale(LC_ALL, "rus");

	ifstream fin(STAT_FILE_NAME);
	if (!fin.is_open())
		return 1;

	array<Method, 20> methods;
	readMethods(fin, methods);

	fin.close();

	fin.open("saR.txt");
	if (!fin.is_open())
		return 1;

	array<int, 2000> tLengths;
	readLengths(fin, tLengths);

	fin.close();


    return 0;
}

