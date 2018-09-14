// FreqMethod2.cpp: определяет точку входа для консольного приложения.
// Программа для реализации частотного метода объединения решений, с использованием таблицы ошибок
// Program for implementing a frequency method for combining solutions, using an error table
// input files: ouR.txt, saR.txt, enQ.txt

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
string const SCALE_FILE_NAME = "enQ.txt";
const int COLS = 33;
const int ROWS = 9;
string scaleLetters;

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
	int getSizeIndex(int & size)
	{
		for (int i = 0; i < ROWS; i++)
		{
			if (size <= table_[i][0]) return i;
		}
		return ROWS - 1;
	}
	vector<double> getRow(int index)
	{
		vector<double> row;
		row.reserve(COLS - 2);
		for (int i = 0; i < COLS - 2; i++)
		{
			row.push_back(static_cast<double>(table_[index][i + 2])/table_[index][1]); 
		}
		return row;
		
	}
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
	int getSize() const { return letters_.length(); }
	string getLetters() const { return letters_; }
	void print(ofstream & fout) const
	{
		fout << letters_ << '\t' << index_ << '\t' << treePosition_ << '\t' << name_ << endl;
	}
private:
	string letters_;
	int index_ = 0;
	int treePosition_ = 0;
	string name_;
};

class LettersSet
{
public:
	bool addLetter(char & c)
	{
		if (letters_.find(c) == string::npos) { letters_ += c; return true; }
		return false;
	}
	char getLetter()
	{
		char c;
		if (letters_.length() != 0) { c = letters_[0]; letters_.erase(letters_.begin());}
		else c = '#';
		return c;
	}
	bool removeLetter(char & c)
	{
		auto const pos = letters_.find(c);
		if (pos != string::npos)
		{
			letters_.erase(letters_.begin() + pos); return true;
		}
		return false;
	}
private:
	string letters_;
};

struct Letter
{
	Letter(char & c, double & d) : letter(c), errorProbability(d) {}
	char letter;
	double errorProbability;
};

class LetterRating
{
public:
	LetterRating(int length)
	{
		letters_.reserve(length);
	}
	void addLetter(char & c, double & d)
	{
		auto it = find_if(letters_.begin(), letters_.end(), 
			[c](const Letter & current) {return current.letter == c; });
		if (it != letters_.end())
		{
			it->errorProbability *= d;
		}
		else
		{
			letters_.push_back(Letter(c, d));
		}
		
	}
	char findMin()
	{
		const auto it = min_element(letters_.begin(), letters_.end(),
			[](const Letter & a, const Letter & b) {return a.errorProbability < b.errorProbability; });
		return it->letter;
	}
	int getSize() const
	{
		return letters_.size();
	}
private:
	vector<Letter> letters_;
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

char findBestLetter(string & letters, vector<vector<double>> & coeffs, string & usedLetters, LettersSet & candidates)
{	
	
	char best = '#';
	double minRate = 1;
	LetterRating lr(static_cast<int>(letters.length()));

	for (auto iC = 0; iC < letters.length(); iC++)
	{
		if (usedLetters.find(letters[iC]) == string::npos) {
			candidates.addLetter(letters[iC]);
			double rate = coeffs[iC][scaleLetters.find(letters[iC])];
			lr.addLetter(letters[iC], rate);
		}
		/*
		if (rate < minRate)
		{
			best = letters[iC];
			minRate = rate;
		}
		*/
	}
	if (lr.getSize() == 0) return candidates.getLetter();
	best = lr.findMin();
	candidates.removeLetter(best);
	return best;
	
}

Solution findBestSolution(vector<Solution> & solutions, int & nMethods, int & tLength, vector<Method> & methods)
{
	Solution bestSol;
	int const nLetters = solutions[0].getSize();
	LettersSet candidates;
	string bsol;
	bsol.reserve(COLS - 2);
	for (int iL = 0; iL < nLetters; iL++)
	{
		string column;
		column.reserve(nMethods);
		for (int iS = 0; iS < nMethods; iS++)
		{
			column += solutions[iS].getLetters()[iL];
			
		}

		vector<vector<double>> coeffs;
		coeffs.reserve(nMethods);

		int const row = methods[0].getSizeIndex(tLength);
		for (int iS = 0; iS < nMethods; iS++)
		{
			coeffs.push_back(methods[iS].getRow(row));
		}

		bsol += findBestLetter(column, coeffs, bsol, candidates);
		
	}
	


	bestSol.setLetters(bsol);
	int i = nMethods + 1;
	bestSol.setIndex(i);
	i = 0;
	bestSol.setTreePosition(i);
	string s(" [FR2]");
	bestSol.setName(s);
	return bestSol;
}

void doResolve(ifstream & fin, ofstream & fout, int & nM, vector<int> & tLengths, vector<Method> & methods)
{
	int nFrags, nMethods;
	vector<string> fragNumbers;

	fin >> nFrags >> nMethods;
	fout << nFrags << " " << nMethods + 1 << endl;
	fragNumbers.reserve(nFrags);
	if (nM != nMethods) { cout << "Method numbers does not match! (in" + SOLS_FILE_NAME + " & " + STAT_FILE_NAME + " files"; return ; }
	for (int iF = 0; iF < nFrags; iF++)
	{
		vector<Solution> solutions;
		solutions.reserve(nMethods);
		readSolutions(fin, nMethods, solutions, fragNumbers);
		Solution bestSolution = findBestSolution(solutions, nMethods, tLengths[iF], methods);

		fout << fragNumbers[iF] << endl;
		for (auto & i : solutions) {
			i.print(fout);
		}
		bestSolution.print(fout);

	}
}

string readScaleLetters(ifstream & fin)
{
	int dig;
	string s;
	fin >> dig >> dig >> dig >> dig;
	s.reserve(dig);
	for (int i = 0; i < dig; i++)
	{
		char c;
		fin >> c;
		s += c;
		
	}
	return s;
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

	fin.open(SCALE_FILE_NAME, ios_base::in);
	if (!fin.is_open())
		return 1;

	scaleLetters = readScaleLetters(fin);

	fin.close();

	fin.open(SOLS_FILE_NAME, ios_base::in);
	if (!fin.is_open())
		return 1;

	fout.open(TEMP_FILE_NAME, ios_base::out);

	doResolve(fin, fout, nMethods, tLengths, methods);
	fin.close();
	fout.close();




    return 0;
}

