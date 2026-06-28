/*
Name: Juhayra Khan
ID: 25I-0922
Section: BCS-F
*/

#pragma once
#include <iostream>
#include <fstream>
using namespace std;

// Question 1: Shapeshifter

	void appendatpos(int*& arr, int& size, int next, int pos){
		int* newarr = new int[size + 1];
		for(int i = 0;i < pos;i++)
			*(newarr + i) = *(arr + i);
		*(newarr + pos) = next;
		for(int i = pos;i < size;i++)
			*(newarr + i + 1) = *(arr + i);
		size++;
		delete[] arr;
		arr = newarr;
	}

	int*** CreateViews(int* data, int size, int& count){
		if(data == nullptr) return nullptr;
		count = 2; // 1 and size;
		int factorcount = 2; int* factor = new int[factorcount];
		*factor = 1; *(factor + 1) = size;
		for(int i = 2;i <= size / 2;i++){
			if(size % i == 0){
				count++;
				appendatpos(factor, factorcount, i, factorcount - 1);
			}
		}
		// my logic uses one array for all factors,
		// so if it's a square number, it should come twice so both can be usedindependently
		if(count % 2){
			appendatpos(factor, factorcount, *(factor + (factorcount / 2)), factorcount / 2);
			count++;
		}

		int*** views = new int** [factorcount];

		// 1 2 3 4 6 12
		for(int i = 0;i < factorcount;i++){
			int currentfactor = *(factor + i);
			int** ptr = new int* [currentfactor]; // 2
			for(int j = 0;j < currentfactor;j++){ //
				*(ptr + j) = data + j * *(factor + factorcount - 1 - i);
			}
			*(views + i) = ptr;
		}

		delete[] factor;factor = nullptr;

		return views;
	}

// Question 2: Memory Archivist

	char** mutationHistory = new char* [10];	// Stores copies of the tape after each mutation
	int mutationCapacity = 10;					// Current capacity of arrays
	int mutationCount = 0;						// Number of mutations stored

	void doubleCapacity(){
		char** doubled = new char* [mutationCapacity * 2];
		for(int i = 0;i < mutationCount;i++){
			*(doubled+i) = *(mutationHistory+i);
		}
		delete[] mutationHistory;
		mutationHistory = doubled;
		mutationCapacity *= 2;
	}

	int getLength(const char* str){
		int len = 0;
		if(str == "" || str == nullptr) return 0;
		while(*(str + len) != '\0') len++;
		return len;
	}

	char* copyString(const char* str){
		if(str == nullptr) return nullptr;
		int len = getLength(str);
		char* copy = new char[len + 1];
		if(len == 0){
			*copy = '\0';
			return copy;
		}
		for(int i = 0;i < len;i++) *(copy+i) = *(str + i);
		*(copy+len) = '\0';
		return copy;
	}

	void recordMutation(char* newTapeState, const char* operation, const char* inverseOp, const char* params){
		char* mutation = copyString(newTapeState);
		if(mutationCapacity == mutationCount) doubleCapacity();
		*(mutationHistory+mutationCount) = mutation;


		ofstream mutationlog("mutation_log.txt", ios::app);

		mutationlog << mutationCount << '|' << operation << '|' << inverseOp << '|' << params << '\n';

		mutationCount++;
		mutationlog.close();
	}

	char* revertToVersion(int index){

		if(index < 0 || index >= mutationCount) return nullptr;
		char line[256];
		int lineNo = 0;

		ifstream original("mutation_log.txt");
		ofstream temp("temp.txt");

		while(original.getline(line, 256)) {
			if(lineNo > index)
				temp << line << "|REVERTED\n";
			else
				temp << line << "\n";

			lineNo++;
		}

		original.close();
		temp.close();

		remove("mutation_log.txt");
		rename("temp.txt", "mutation_log.txt");

		mutationCount = index + 1;
		return copyString(*(mutationHistory+index));
	}

	bool areEqual(const char* str1, const char* str2){
		if(str1 == nullptr || str2 == nullptr) return false;
		int len1 = getLength(str1), len2 = getLength(str2);
		if(len1 != len2) return false;
		for(int i = 0;i < len1;i++){
			if(*(str1+i) != *(str2+i)) return false;
		}
		return true;
	}

	char* concatenate(const char* str1, const char* str2){
		int len1 = getLength(str1), len2 = getLength(str2);
		int totallen = len1 + len2;
		if(totallen == 0) return nullptr;
		char* newstring = new char[totallen + 1];
		for(int i = 0;i < len1;i++) *(newstring + i) = *(str1+i);
		for(int i = 0;i < len2;i++) *(newstring + i + len1) = *(str2+i);
		*(newstring+totallen) = '\0';
		return newstring;
	}

	void toUpperCase(char* str){
		int len = getLength(str);
		for(int i = 0;i < len;i++)
			if(*(str+i) > 96 && *(str+i) < 123)
				*(str+i) -= 32;
		char* newstring = copyString(str);
		recordMutation(newstring, "UPPERCASE", "LOWERCASE", "");
	}

	void toLowerCase(char* str){
		int len = getLength(str);
		for(int i = 0;i < len;i++)
			if(*(str+i) > 64 && *(str+i) < 91)
				*(str+i) += 32;
		char* newstring = copyString(str);
		recordMutation(newstring, "LOWERCASE", "UPPERCASE", "");
	}

	int findFirstOccurrence(const char* str, char target){
		int len = getLength(str);
		for(int i = 0;i < len;i++)
			if(*(str+i) == target)
				return i;
		return -1;
	}

	int findSubstring(const char* str, char* substring){
		int len = getLength(str), sublen = getLength(substring);
		if(sublen == 0) return 0;
		for(int i = 0;i < len;i++)
			for(int j = 0;j < sublen; j++){
				if(*(str + i + j) != *(substring+j)) break;
				if(j == sublen - 1) return i;
			}
		return -1;
	}

	char* reverseString(char* str){
		int len = getLength(str);
		char* reverse = new char[len + 1];
		for(int i = 0;i < len;i++) *(reverse+len - i - 1) = *(str+i);
		*(reverse+len) = '\0';
		return reverse;
		recordMutation(reverse, "REVERSE", "REVERSE", "");
	}

	char* removeCharacter(char* str, char col){
		int len = getLength(str), newlen = 0;
		for(int i = 0;i < len;i++)
			if(*(str+i) != col) newlen++;
		char* removed = new char[newlen + 1];
		for(int i = 0, j = 0;i < len;i++)
			if(*(str+i) != col){
				*(removed + j) = *(str+i);
				j++;
			}
		*(removed+newlen) = '\0';
		recordMutation(removed, "REMOVECHAR", "", "");
		return removed;
	}

	char* substring(const char* str, int start, int length){
		int len = getLength(str);
		if(len < start) return nullptr;
		char* sub = new char[length + 1];
		for(int i = start, j = 0;j < length;i++, j++)
			*(sub+j) = *(str+i);
		*(sub+length) = '\0';
		return sub;
	}

	char* replaceChar(char* str, char oldChar, char newChar){
		int len = getLength(str);
		char* replaced = new char[len + 1];
		for(int i = 0;i < len;i++)
			if(*(str+i) == oldChar) *(replaced + i) = newChar;
			else *(replaced + i) = *(str+i);
		*(replaced + len) = '\0';
		return replaced;
	}

	char* replaceSubstring(const char* str, const char* target, const char* replacement){

		int len = getLength(str), targetlen = getLength(target), replen = getLength(replacement);
		if(targetlen == 0) return nullptr;

		int count = 0;

		for(int i = 0;i < len;i++){
			int j = 0;
			for(;*(target+j) != '\0' && *(target+j) == *(str+i+j);j++) j++;
			if(*(target+j) == '\0'){
				count++;
				i += targetlen - 1;
			}
		}

		int newlen = len + count * (replen - targetlen);
		char* replaced = new char[newlen + 1];

		int k = 0;

		for(int i = 0;i < len;){
			int j = 0;
			while(*(target+j) != '\0' && *(str+i+j) == *(target+j)) j++;
			if(*(target+j) == '\0'){
				for(int row = 0; row < replen; row++)
					*(replaced+k++) = *(replacement+row);
				i += targetlen;
			}
			else{
				*(replaced+k) = *(str+i);
				k++; i++;
			}
		}

		replaced[newlen] = '\0';

		return replaced;
	}

	char* insertAt(const char* str, const char* toInsert, int position){
		int len = getLength(str); int sublen = getLength(toInsert);
		char* newstring = new char[len + sublen + 1];
		for(int i = 0;i < position;i++){
			*(newstring+i) = *(str+i);
		}
		for(int j = 0;j < sublen;j++){
			*(newstring+j + position) = *(toInsert+j);
		}
		for(int i = position;i < len;i++){
			*(newstring+i + sublen) = *(str+i);
		}
		*(newstring+len + sublen) = '\0';

		return newstring;
	}

	char* deleteRange(const char* str, int start, int length){
		int len = getLength(str);
		char* newstring = new char[len - length + 1];
		int i;
		for(i = 0;i < start;i++){
			*(newstring+i) = *(str+i);
		}
		for(int j = start + length;j < len;j++, i++){
			*(newstring+i) = *(str+j);
		}
		newstring[len - length] = '\0';
		return newstring;
	}

	char* compress(const char* str){
		// aaabbbccc
		int len = getLength(str);
		if(len == 0) return nullptr;
		int unique = 1;
		for(int i = 1;i < len;i++){
			if(*(str+i) != str[i - 1]) unique++;
		}
		char* rle = new char[(unique * 2) + 1];
		rle[unique * 2] = '\0';
		for(int i = 1;i < 2 * unique;i += 2){
			rle[i] = '1';
		}
		rle[0] = str[0];
		for(int i = 1, j = 1;i < len && j < 2 * unique;i++){
			if(rle[j - 1] != *(str+i)){ // j-1 is current character
				rle[j + 1] = *(str+i); // j+1 is next character
				j += 2;
			}
			else (*(rle+j))++; // j is count of current character
		}
		1;

		return rle;
	}

	void append(char*& str, int size, char append){
		char* newstr = new char[size + 2];
		newstr[size + 1] = '\0';
		newstr[size] = append;
		for(int i = 0;i < size;i++)
			newstr[i] = *(str+i);
		delete[]str;
		str = newstr;
	}

	char* decompress(const char* str){
		int len = getLength(str);
		int* countarr = new int[len / 2];
		for(int i = 1, j = 0;i < len;i += 2, j++){
			*(countarr+j) = *(str+i) - 48;
		}
		int count = 0;
		for(int i = 0;i < (len / 2);i++){
			count += *(countarr+i);
		}
		char* rle = new char[count + 1];
		int offset = 0;
		for(int i = 0;i < len / 2;i++){
			for(int j = 0;j < *(countarr+i);j++){
				*(rle+j + offset) = *(str+(2 * i));
			}
			offset += *(countarr+i);
		}
		*(rle+count) = '\0';
		return rle;
	}

	char* rotateString(char* str, int positions){
		if(str == nullptr) return nullptr;
		int len = getLength(str);
		char* reverse = new char[len + 1];
		for(int i = 0;i <= len;i++) *(reverse+i) = *(str+i);
		if(positions == 0) return reverse;

		for(int i = 0;i < positions;i++){
			int temp = *reverse;
			for(int i = 1;i < len;i++){
				*(reverse+i - 1) = *(reverse + i);
			}
			*(reverse+len - 1) = temp;
		}
		return reverse;
	}

// Question 3: Alex Horne's Recursion

	// Part 3.1: Diamond (main function is not recursive but every helper function is)
		void spaces(int n){
			if(n <= 0) return;
			cout << " ";
			spaces(n - 1);
		}

		void stars(int o, char c1, char c2){
			if(o <= 0) return;
			if(o % 2) cout << c1;
			else cout << c2;
			stars(o - 1, c1, c2);
		}

		void toprow(int n, int o, char c1, char c2){
			if(n <= 0) return;
			spaces(n - 1);
			stars(2 * (o - n + 1) - 1, c1, c2);
			spaces(n - 1);
			cout << endl;
			toprow(n - 1, o, c1, c2);
		}

		void bottomrow(int n, int o, char c1, char c2){
			if(n <= 1) return;
			bottomrow(n - 1, o, c1, c2);
			spaces(n - 1);
			stars(2 * (o - n + 1) - 1, c1, c2);
			spaces(n - 1);
			cout << endl;
		}

		void printDiamond(int size, char c1, char c2){
			if(size % 2==0){
				char temp = c1; c1 = c2; c2 = temp;
			}
			toprow(size, size, c1, c2);
			bottomrow(size, size, c1, c2);
		}

	// Part 3.2: X
		void nearest3(int& x, int i = 1){
			if(x < 3){
				x = 3; return;
			}
			if(i * 3 > x){
				x = i; return;
			}
			nearest3(x, i * 3);
		}

		bool isX(int row, int col, int size){
			if(size == 1) return true;

			int sub = size / 3;
			int innerrow = row / sub;
			int innercol = col / sub;

			if(innerrow == innercol || innerrow + innercol == 2)
				return isX(row % sub, col % sub, sub);

			return false;
		}

		void printRow(int row, int col, int size){
			if(col == size) return;
			if(isX(row, col, size)) cout << "##";
			else cout << "  ";
			printRow(row, col + 1, size);
		}

		void renderGrid(int row, int size){
			nearest3(size);
			if(row == size) return;
			printRow(row, 0, size);
			cout << endl;
			renderGrid(row + 1, size);
		}

// Question 4: Ironclad (INCOMPLETE)

		void extending(char*& arr, int& size){
			char* newarr = new char[size + 1];
			for(int i = 0;i < size;i++) *(newarr+i) = *(arr+i);
			delete[]arr;
			arr = newarr;
			size++;
		}

		unsigned char Key(const char* password, int i = 0) {
			if(i == 8)
				return 0;
			return (((*(password + i) & 1) << i) | Key(password, i + 1));
		}

		unsigned char modifier(const char* password, int i = 0) {
			if(i == 8)
				return 0;
			return ((((*(password + i) & 2) >> 1) << i) | modifier(password, i + 1));
		}

		void firstpartofName(ifstream& file_names, char*& filename, int i = 0){
			if(i == 4) return;
			file_names >> *(filename+i);
			firstpartofName(file_names, filename, i + 1);
		}

		void secondpartofName(ifstream& file_names, char*& filename, int i = 4){
			if(*(filename+i - 3) == 't' && *(filename+i - 2) == 'x' && *(filename+i - 1) == 't'){
				*(filename+i) = '\0';
				return;
			}
			file_names.get(*(filename+i));
			secondpartofName(file_names, filename, i + 1);
		}

		void readfile(ifstream& currentfile, char*& x, int& size, int i = 0){
			if(i + 1 >= size) extending(x, size);
			char check;
			if(!currentfile.get(check)){
				*(x+i) = '\0';
				return;
			}
			else if(check != '\n'){
				*(x+i) = check;
				readfile(currentfile, x, size, i + 1);
			}
			else readfile(currentfile, x, size, i);
		}

		void decrypthelper(char* data, char* decrypted, int& pos, unsigned char key, int currentpos, int totalBlocks) {

			if(totalBlocks <= 0){
				*(decrypted + pos) = '\0';
				return;
			}

			unsigned char nextIndex = *(data + currentpos);
			unsigned char keymod = *(data + currentpos + 1);
			unsigned char encryptedChar = *(data + currentpos + 3);

			*(decrypted + pos) = encryptedChar ^ key;
			pos++;

			if(nextIndex == 255)
				return;

			unsigned char newKey = key ^ keymod;

			decrypthelper(data, decrypted, pos, newKey, nextIndex * 4, totalBlocks - 1);
		}

// Question 5: Mystery of Severance

	void extend(int*& arr, int size){
		int* newarr = new int[size + 1]{0};
		for(int i = 0;i < size;i++) *(newarr+i) = *(arr+i);
		delete[]arr;
		arr = newarr;
	}

	void append(bool*& arr, int& size, bool val, int loop = 1){
		for(int k = 0;k < loop;k++){
			bool* newarr = new bool[size + 1];
			for(int i = 0;i < size;i++) *(newarr+i) = *(arr+i);
			*(newarr+size) = val;
			delete[]arr;
			arr = newarr;
			size++;
		}
	}

	void add(bool*& arr, char* ch, int& size){
		for(int i = 0;*(ch+i) != '\0';i++){
			if(i == 0){
				*arr = true;
			}
			switch(*(ch+i)){
			default: return;
			case ' ':
				append(arr, size, 0, 2);	break;
			case 'A':case 'a':
				if(i != 0)
				append(arr, size, 1);		append(arr, size, 0);
				append(arr, size, 1, 3);	break;
			case 'B':case 'b':
				if(i != 0)
				append(arr, size, 1, 1);	append(arr, size, 1, 2);	append(arr, size, 0);
				append(arr, size, 1);		append(arr, size, 0);
				append(arr, size, 1);		append(arr, size, 0);
				append(arr, size, 1);		break;
			case 'C':case 'c':
				if(i != 0)
				append(arr, size, 1, 1);	append(arr, size, 1, 2);	append(arr, size, 0);
				append(arr, size, 1);		append(arr, size, 0);
				append(arr, size, 1, 3);	append(arr, size, 0);
				append(arr, size, 1);		break;
			case 'D':case 'd':
				if(i != 0)
				append(arr, size, 1, 1);	append(arr, size, 1, 2);	append(arr, size, 0);
				append(arr, size, 1);		append(arr, size, 0);
				append(arr, size, 1);		break;
			case 'E':case 'e':
				if(i != 0)
				append(arr, size, 1);		break;
			case 'F':case 'f':
				if(i != 0)
				append(arr, size, 1);		append(arr, size, 0);
				append(arr, size, 1);		append(arr, size, 0);
				append(arr, size, 1, 3);	append(arr, size, 0);
				append(arr, size, 1);		break;
			case 'G':case 'g':
				if(i != 0)
				append(arr, size, 1, 1);	append(arr, size, 1, 2);	append(arr, size, 0);
				append(arr, size, 1, 3);	append(arr, size, 0);
				append(arr, size, 1);		break;
			case 'H':case 'h':
				if(i != 0)
				append(arr, size, 1);		append(arr, size, 0);
				append(arr, size, 1);		append(arr, size, 0);
				append(arr, size, 1);		append(arr, size, 0);
				append(arr, size, 1);		break;
			case 'I':case 'i':
				if(i != 0)
				append(arr, size, 1);		append(arr, size, 0);
				append(arr, size, 1);		break;
			case 'J':case 'j':
				if(i != 0)
				append(arr, size, 1);		append(arr, size, 0);
				append(arr, size, 1, 3);	append(arr, size, 0);
				append(arr, size, 1, 3);	append(arr, size, 0);
				append(arr, size, 1, 3);	break;
			case 'K':case 'k':
				if(i != 0)
				append(arr, size, 1, 1);	append(arr, size, 1, 2);	append(arr, size, 0);
				append(arr, size, 1);		append(arr, size, 0);
				append(arr, size, 1, 3);	break;
			case 'L':case 'l':
				if(i != 0)
				append(arr, size, 1);		append(arr, size, 0);
				append(arr, size, 1, 3);	append(arr, size, 0);
				append(arr, size, 1);		append(arr, size, 0);
				append(arr, size, 1);		break;
			case 'M':case 'm':
				if(i != 0)
				append(arr, size, 1, 1);	append(arr, size, 1, 2);	append(arr, size, 0);
				append(arr, size, 1, 3);	break;
			case 'N':case 'n':
				if(i != 0)
				append(arr, size, 1, 1);	append(arr, size, 1, 2);	append(arr, size, 0);
				append(arr, size, 1);		break;
			case 'O':case 'o':
				if(i != 0)
				append(arr, size, 1, 1);	append(arr, size, 1, 2);	append(arr, size, 0);
				append(arr, size, 1, 3);	append(arr, size, 0);
				append(arr, size, 1, 3);	break;
			case 'P':case 'p':
				if(i != 0)
				append(arr, size, 1);		append(arr, size, 0);
				append(arr, size, 1, 3);	append(arr, size, 0);
				append(arr, size, 1, 3);	append(arr, size, 0);
				append(arr, size, 1);		break;
			case 'Q':case 'q':
				if(i != 0)
				append(arr, size, 1, 1);	append(arr, size, 1, 2);	append(arr, size, 0);
				append(arr, size, 1, 3);	append(arr, size, 0);
				append(arr, size, 1);		append(arr, size, 0);
				append(arr, size, 1, 3);	break;
			case 'R':case 'r':
				if(i != 0)
				append(arr, size, 1);		append(arr, size, 0);
				append(arr, size, 1, 3);	append(arr, size, 0);
				append(arr, size, 1);		break;
			case 'S':case 's':
				if(i != 0)
				append(arr, size, 1);		append(arr, size, 0);
				append(arr, size, 1);		append(arr, size, 0);
				append(arr, size, 1);		break;
			case 'T':case 't':
				if(i != 0)
				append(arr, size, 1, 1);	append(arr, size, 1, 2);	break;
			case 'U':case 'u':
				if(i != 0)
				append(arr, size, 1);		append(arr, size, 0);
				append(arr, size, 1);		append(arr, size, 0);
				append(arr, size, 1, 3);	break;
			case 'V':case 'v':
				if(i != 0)
				append(arr, size, 1);		append(arr, size, 0);
				append(arr, size, 1);		append(arr, size, 0);
				append(arr, size, 1);		append(arr, size, 0);
				append(arr, size, 1, 3);	break;
			case 'W':case 'w':
				if(i != 0)
				append(arr, size, 1);		append(arr, size, 0);
				append(arr, size, 1, 3);	append(arr, size, 0);
				append(arr, size, 1, 3);	break;
			case 'X':case 'x':
				if(i != 0)
				append(arr, size, 1, 1);	append(arr, size, 1, 2);	append(arr, size, 0);
				append(arr, size, 1);		append(arr, size, 0);
				append(arr, size, 1);		append(arr, size, 0);
				append(arr, size, 1, 3);	break;
			case 'Y':case 'y':
				if(i != 0)
				append(arr, size, 1, 1);	append(arr, size, 1, 2);	append(arr, size, 0);
				append(arr, size, 1);		append(arr, size, 0);
				append(arr, size, 1, 3);	append(arr, size, 0);
				append(arr, size, 1, 3);	break;
			case 'Z':case 'z':
				if(i != 0)
				append(arr, size, 1, 1);	append(arr, size, 1, 2);	append(arr, size, 0);
				append(arr, size, 1, 3);	append(arr, size, 0);
				append(arr, size, 1);		append(arr, size, 0);
				append(arr, size, 1);
			}
			if(ch[i + 1] != '\0') append(arr, size, 0, 2);
		}
	}

	bool** refineSignal(const char* inputDirective, int& outWordCount, int*& outWordSizes){
		if(*inputDirective == '\0'){
			outWordCount = 0;
			return nullptr;
		}
		char** words = new char* [10];
		for(int i = 0;i < 5;i++) *(words+i) = new char[20];
		outWordCount = 0;int j = 0;
		outWordSizes = new int[1]{0};
		for(int i = 0;*(inputDirective+i) != '\0';i++){
			if(*(inputDirective+i) == ' '){
				*(*(words + outWordCount)+j) = '\0';
				j = 0; outWordCount++;
				extend(outWordSizes, outWordCount);
				continue;
			}
			*(*(words + outWordCount)+j) = *(inputDirective+i);
			(*(outWordSizes+outWordCount))++;
			j++;
		}
		*(*(words + outWordCount)+j) = '\0';
		outWordCount++;
		bool** parsed = new bool* [outWordCount];
		for(int i = 0;i < outWordCount;i++){
			int size = 1;
			*(parsed+i) = new bool[size];
			add(*(parsed+i), *(words+i), size);
			if(size % 2) append(*(parsed+i), size, 0);
			*(outWordSizes+i) = size / 2;
		}

		for(int i = 0;i < 5;i++){
			delete[] *(words+i);
			*(words+i) = nullptr;
		}
		delete[] words; words = nullptr;

		return parsed;
	}

	long long computeChecksum(bool** cluster, int* sizes, int wordCount, int index){
		if(index == wordCount) return 0;
		unsigned short* ptr = (unsigned short*)*(cluster + index);
		unsigned short* endptr = ptr + *(sizes + index);
		long long currentVal = 0;
		while(ptr != endptr){
			if(*ptr)	currentVal = (currentVal >> 1) ^ (*ptr);
			else		currentVal = (currentVal << 1) + (*ptr);
			ptr++;
		}
		if(index % 3 == 0)	return currentVal + computeChecksum(cluster, sizes, wordCount, index + 1);
		else				return currentVal - computeChecksum(cluster, sizes, wordCount, index + 1);
	}

	void destroyCluster(bool** cluster, int* sizes, int wordCount){
		for(int i = 0;i < wordCount;i++){
			delete[] *(cluster+i); *(cluster+i) = nullptr;
		}
		delete[] cluster; cluster = nullptr;
		delete[] sizes; sizes = nullptr;
	}


// Question 6: Void Walkers

	float calculateNecroticPower(const unsigned char* data, int numUnits, int targetID, int i = 0) {
		if(i >= numUnits * 12) return 0;
		int start = i;
		int unitID = *(data + i) | (*(data + i + 1) << 8) | (*(data + i + 2) << 16) | (*(data + i + 3) << 24);
		i += 4;
		float power = *((float*)(data + i));
		i += 4;
		int masterID = *(data + i) | (*(data + i + 1) << 8) | (*(data + i + 2) << 16) | (*(data + i + 3) << 24);
		float sum = 0.0f;
		if(unitID == targetID) sum += power;
		if(masterID == targetID) sum += calculateNecroticPower(data, numUnits, unitID);
		sum += calculateNecroticPower(data, numUnits, targetID, start + 12);
		return sum;
	}

	bool solveMaze(char* maze, int n, int row, int col){

		if(row >= n || col >= n) return false;
		if(row < 0 || col < 0) return false;
		if(*(maze + row * n + col) == '*') return false;
		if(*(maze + row * n + col) == '#') return false;
		if(*(maze + row * n + col) == 'E') return true;
		if(*(maze + row * n + col) != 'S')
			*(maze + row * n + col) = '*';
		bool currcheck = false;
		currcheck |= solveMaze(maze, n, row + 1, col); // down
		currcheck |= solveMaze(maze, n, row, col + 1); // right
		currcheck |= solveMaze(maze, n, row - 1, col); // up
		currcheck |= solveMaze(maze, n, row, col - 1); // left
		return currcheck;
	}

	int getSqrt(int n, int i = 1){
		if(i * i > n) return 0;
		if(i * i == n) return i;
		return getSqrt(n, i + 1);
	}

	void processStream(unsigned char* stream) {

		int packettype = *stream;
		int size = *(stream + 1) | (*(stream + 2) << 8) | (*(stream + 3) << 16) | (*(stream + 4) << 24);
		int payloadsize = size - 5;
		switch(packettype){
		case 1:{
			unsigned char* chain = stream + 5;
			int targetID = *(stream + size - 4) | (*(stream + size - 3) << 8) | (*(stream + size - 2) << 16) | (*(stream + size - 1) << 24);
			int numUnits = (payloadsize - 4) / 12;
			float power = calculateNecroticPower(chain, numUnits, targetID);
			cout << "Necrotic Power: " << power; 
			break;
		}
		case 2:{
			unsigned char* maze = stream + 5;
			int n = getSqrt(payloadsize);
			if(!n){
				cout << "Frostbite Labyrinth: UNSOLVABLE";
				return;
			}
			bool s = solveMaze((char*)maze, n, 0, 0);
			1;
			if(s) cout << "Frostbite Labyrinth: SOLVABLE"; 
			else cout << "Frostbite Labyrinth: UNSOLVABLE";
			break;
		}
		case 3:{
			cout << "The Scream: Audio Processed"; // note to TA: my testcases kept failing because y'all never specified we were supposed to output this??? wasted a whole day onn this fr
			unsigned char* scream = stream + 5;
			int samplerate = (*scream) | (*(scream + 1) << 8) | (*(scream + 2) << 16) | (*(scream + 3) << 24);
			scream += 4;
			int channelcount = (*scream) | (*(scream + 1) << 8) | (*(scream + 2) << 16) | (*(scream + 3) << 24);
			scream += 4;

			float* samples = (float*)(scream);
			int count = (payloadsize - 8) / 4;
			if(count == 1) return;

			float* newsamples = new float[count];

			for(int i = 0;i < count;i++){
				if(i - channelcount < 0)
					*(newsamples + i) = (*(samples + i) + *(samples + i + channelcount)) / 2;
				else if(i + channelcount >= count)
					*(newsamples + i) = (*(samples + i) + *(samples + i - channelcount)) / 2;
				else
					*(newsamples + i) = (*(samples + i) + *(samples + i - channelcount) + *(samples + i + channelcount)) / 3;
			}

			int i = 0;
			while(i < count){
				*(samples+i) = *(newsamples+i);
				i++;
			}
		}
		}
	}