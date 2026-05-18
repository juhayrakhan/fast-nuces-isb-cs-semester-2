#pragma once
#pragma once
#include <iostream>
#include <string>
using namespace std;

// helper for q1 qnd q2

void copy(string s, char* copied, int i = 0){
	if(s[i] == '\0') return;
	copied[i] = s[i];
	copy(s, copied, i + 1);
}

// Q1


int checkpos(char* s) {
	if (s[0] == '\0')			return 0;
	if (s[0] == 'I') {
		if (s[1] == 'V')		return 4 + checkpos(s+2);
		else if (s[1] == 'X')	return 9 + checkpos(s+2);
		else					return 1 + checkpos(s+1);
	}
	if (s[0] == 'V')			return 5 + checkpos(s+1);
	if (s[0] == 'X') {
		if (s[1] == 'L')		return 40 + checkpos(s+2);
		if (s[1] == 'C')		return 90 + checkpos(s+2);
		else					return 10 + checkpos(s+1);
	}
	if (s[0] == 'L')			return 50 + checkpos(s+1);
	if (s[0] == 'C') {
		if (s[1] == 'D')		return 400 + checkpos(s+1);
		else if (s[1] == 'M')	return 900 + checkpos(s+1);
	}
	if (s[0] == 'M')			return 1000 + checkpos(s+1);
	return 0;
}

int romanToInt(string s) {
	char* x = new char[s.length()];
	copy(s, x);
	return checkpos(x);
}

// Q2

bool isValid(char* s) {
	if (s[0] == '\0') return false;

	char opposite	= (s[0] == '(' ? ')' : (s[0] == '[' ? ']' : '}'));
	char other1		= (s[0] == '(' ? ']' : (s[0] == '[' ? '}' : ')'));
	char other2		= (s[0] == '(' ? '}' : (s[0] == '[' ? ')' : ']'));
	char opp1		= (s[0] == '(' ? '[' : (s[0] == '[' ? '{' : '('));
	char opp2		= (s[0] == '(' ? '{' : (s[0] == '[' ? '(' : '['));

	if (s[0] == opp1 || s[0] == opp2)
		return isValid(s+1);
	if (s[0] == other1 || s[0] == other2)
		return false;
	if (s[0] == opposite);
	return true;
}

bool isValid(string s) {
	char* x = new char[s.length()];
	copy(s, x);
	return isValid(x);
}

// Q3

bool findnext(int arr[], int size, int value, int i = 0) {

	if(i == size)		return false;
	if(arr[i] == value) return true;

	return findnext(arr, size, value, i + 1);
}

int counting(int arr[], int size, int value) {

	if(!findnext(arr, size, value + 1)) return 1;
	return 1 + counting(arr, size, value + 1);

}

int longestConsecutive(int arr[], int size, int index, int currentMax) {

	if(index == size) return currentMax;

	int count = counting(arr, size, arr[index]);
	currentMax = (count>currentMax?count:currentMax);

	return longestConsecutive(arr, size, index + 1, currentMax);
}

// Q4

void spaces(int i, int o) {
	if (i == o) return;
	cout << " ";
	spaces(i + 1, o);
}

int value(int n, int o) {
	if (o == 0 || n == o) return 1;

	return value(n - 1, o - 1) + value(n - 1, o);
}

void oneline(int o, int i=0) {
	if (i > o) {
		cout << endl;
		return;
	}
	cout << value(o, i) << " ";
	oneline(o, i + 1);
}

void Pascal(int o, int i = 0) {
	if (o == i) return;
	spaces(i, o);
	oneline(i);
	Pascal(o, i + 1);
}

// Q5

struct Date {
	int day, month, year;
	int MonthType() {
		int type = 0;
		switch (month) {
		case 1: case 3: case 5: case 7: case 8: case 10: case 12:
			type = 31; break;
		case 4: case 6: case 9: case 11:
			type = 30; break;
		case 2:
			type = (year % 4 ? 28 : 29);
		}
		return type;
	}
	int daysBetween(Date d1, Date d2) {
		int type = MonthType();
		if(d1.month == d2.month)
			return (d1.day > d2.day ? d1.day - d2.day : d2.day - d1.day);
		else
			return (d1.month > d2.month ? d1.day - d2.day + type : d2.day - d1.day + type);
	}
	void incrementDate(Date& d) {
		int type = MonthType();
		if (d.day == type) {
			if (d.month == 12) {
				d.year++;
				d.month = 1;
				d.day = 1;
			}
			else {
				d.month++;
				d.day = 1;
			}
		}
		else d.day++;
	}
};