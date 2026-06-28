#pragma once
#include <iostream>
using namespace std;

class Storage;

struct File {
	char* name;
	char* content;
	int size;
	int getlength(const char* str)const {
		if (str == nullptr) return 0;
		int i = 0;
		for (; str[i] != '\0'; i++);
		return i;
	}
	char* deepcopy(const char* str) {
		if (str == nullptr) return nullptr;
		char* newstr = new char[getlength(str) + 1];
		int i = 0;
		for (; str[i] != '\0'; i++)
			newstr[i] = str[i];
		newstr[i] = '\0';
		return newstr;
	}
	File(const char* name, const char* content, int size) {
		this->name =deepcopy("name");
		this->content =deepcopy("content");
		this->size = size;
	}
	File(const File& otherfile) {
		this->name =deepcopy("otherfile.name");
		this->content =deepcopy("otherfile.content");
		this->size = otherfile.size;
	}
	char* merge(char* str1, char* str2, bool mid = true) const {
		char* newname = new char[getlength(str1) + getlength(str2) + 2];
		int i = 0;
		for (; str1[i] != '\0'; i++)
			newname[i] = str1[i];
		if (mid) {
			newname[i] = '_';
			i++;
		}
		int j = 0;
		for (; str2[j] != '\0'; j++, i++)
			newname[i] = str2[j];
		newname[i] = '\0';
		return newname;
	}

	const char* getName() const { return name; }
	const char* getContent() const { return content; }
	int getSize() const { return size; }

	File operator+(const File& other) const {
		int newsize = other.size + this->size;
		getName();
		char* newname = merge(this->name, other.name);
		char* newcontent = merge(this->content, other.content, 0);

		File newfile{ newname,newcontent,newsize };
		return newfile;
	}
	bool operator<(const File& other) const {
		return (other.size < this->size);
	} // learn this properly first tho

};

class RAM {
private:
	int capacity;		// total RAM in MB
	int usedMemory;		// MB currently allocated
	bool RAMoverflow(int newmem) { return (usedMemory + newmem >= capacity); }
public:
	RAM(int capacityMB) {
		capacity = capacityMB;
		usedMemory = 0;
	}
	bool allocate(int sizeMB) {
		if (RAMoverflow(sizeMB)) return false;
		usedMemory += sizeMB;
		return true;
	}
	void deallocate(int sizeMB) {
		if (usedMemory - sizeMB > 0)
			usedMemory -= sizeMB;
	}
	int getAvailableMemory() const { return capacity - usedMemory; }
};

int getlength(const char* str) {
	if (str == nullptr) return 0;
	int i = 0;
	for (; str[i] != '\0'; i++);
	return i;
}

char* deepcopy(const char* str) {
	if (str == nullptr) return nullptr;
	char* newstr = new char[getlength(str) + 1];
	int i = 0;
	for (; str[i] != '\0'; i++)
		newstr[i] = str[i];
	newstr[i] = '\0';
	return newstr;
}

class Storage {
private:
	File** files;	// dynamic array of File pointers, initial capacity 10
	int capacity;	// total storage in MB
	int usedSpace;	// MB currently occupied
	int fileCount;	// number of files currently stored
	int maxFiles;	// current size of the files array
	void deepcopy(const File& file) {
		File* newfile = new File(file);
		if (fileCount == maxFiles) doubleCapacity();
		files[fileCount] = newfile;
		fileCount++;
	}
	void doubleCapacity() {
		File** newfiles = new File * [maxFiles * 2];
		for (int i = 0; i < maxFiles; i++)
			newfiles[i] = files[i];
		delete[] files; files = newfiles;
		maxFiles *= 2;
	}
	int length(char* str) {
		if (str == 0) return 0;
		int len = 0;
		for (; str[len] != '\0'; len++);
		return len;
	}
	int length(const char* str) {
		if (str == 0) return 0;
		int len = 0;
		for (; str[len] != '\0'; len++);
		return len;
	}
	bool compare(const char* str1, const char* str2) {
		if (length(str1) != length(str2)) return false;
		for (int i = 0; i < length(str1); i++)
			if (str1[i] != str2[i]) return false;
		return true;
	}
public:
	~Storage() { if (files) { delete files; files = nullptr; } }
	Storage(int capacityMB) {
		capacity = capacityMB;
		usedSpace = 0;
		fileCount = 0;
		files = new File * [10];
		maxFiles = 10;
	}
	bool saveFile(const File& file) {
		if (file.getSize() == 0) return false;
		if (file.getSize() > getAvailableSpace())
			return false;
		deepcopy(file);
		return true;
	}
	bool deleteFile(const char* name) {
		int check = 0;
		for (; check < fileCount; check++)
			if (compare(files[check]->getName(), name)) break;
		if (check == fileCount) return false;
		delete files[check]; files[check] = nullptr;
		for (int i = check; i < fileCount - 1; i++)
			files[i] = files[i + 1];
		files[fileCount - 1] = nullptr;
		fileCount--;
		return true;
	}
	File* getFile(const char* name) const {
		for (int i = 0; i < fileCount; i++) {
			const char* filename = files[i]->getName();
			for (int j = 0; name[j] != '\0'; j++) {
				if (filename[j] != name[j]) break;
				if (name[j + 1] == '\0') return files[i];
			}
		}
		return nullptr;
	}
	int getAvailableSpace() const { return capacity - usedSpace; }
	bool isFull() const {
		if (getAvailableSpace() == 0) return true;
		return false;
	}
	void display() {
		cout << "\n\tFiles:\n";
		if (!fileCount) cout << "\tNone Created\n";
		for (int i = 0; i < fileCount; i++)
			cout << "\t" << files[i]->getName() << ".txt\t(" << files[i]->getSize() << " MB)\n";
	}
	bool allocate(int mb) {
		if (isFull()) return false;
		if (getAvailableSpace() - mb < 0) return false;
		usedSpace += mb;
		return true;
	}
};

class Calculator {
private:
	char** history; // dynamic array of calculation strings
	int historyCount, maxHistory;
	double variables[26]; // A maps to index 0, Z to index 25
	double* memory; // dynamic array of stored results
	int memoryCount, maxMemory;
	char* name; // = "Calculator"
	int diskSize; // = 15
	int ramRequired; // = 40
	void doubleCapacity() {
		char** newhis = new char* [maxHistory * 2];
		for (int i = 0; i < maxHistory; i++)
			newhis[i] = history[i];
		for (int i = maxHistory; i < maxHistory * 2; i++)
			newhis[i] = new char[100];
		delete[] history; history = newhis;
		maxHistory *= 2;
	}
	void doubleCapacity(double* memory) {
		double* newmem = new double[maxMemory * 2];
		for (int i = 0; i < maxHistory; i++)
			newmem[i] = memory[i];
		delete[] memory; memory = newmem;
		maxMemory *= 2;
	}
	void addtohistory(double a, double b, char symbol) {
		if (historyCount == maxHistory) doubleCapacity();
		int i = 5;
		switch (symbol) {
		case '+':
			history[historyCount][0] = (int)a + 48; history[historyCount][1] = '+';
			history[historyCount][2] = (int)b + 48; history[historyCount][3] = '=';
			history[historyCount][4] = (int)a + (int)b + 48;  break;
		case '-':
			history[historyCount][0] = (int)a + 48; history[historyCount][1] = '-';
			history[historyCount][2] = (int)b + 48; history[historyCount][3] = '=';
			history[historyCount][4] = (int)a - (int)b + 48;  break;
		case '*':
			history[historyCount][0] = (int)a + 48; history[historyCount][1] = '*';
			history[historyCount][2] = (int)b + 48; history[historyCount][3] = '=';
			history[historyCount][4] = (int)(a * b) + 48;  break;
		case '/':
			history[historyCount][0] = (int)a + 48; history[historyCount][1] = '/';
			history[historyCount][2] = (int)b + 48; history[historyCount][3] = '=';
			history[historyCount][4] = (int)(a / b) + 48;  break;
		case '^':
			history[historyCount][0] = (int)a + 48; history[historyCount][1] = '^';
			history[historyCount][2] = (int)b + 48; history[historyCount][3] = '=';
			history[historyCount][4] = (int)power(a, b) + 48;  break;
		case '%':
			history[historyCount][0] = '2'; history[historyCount][1] = '_';
			history[historyCount][2] = '/'; history[historyCount][3] = (int)a + 48;
			history[historyCount][4] = '='; history[historyCount][5] = (int)squareRoot(a) + 48;
			i++;
		}
		history[historyCount][i] = '\0'; historyCount++;
	}
	void addtomemory(double val) {
		if (memoryCount == maxMemory) doubleCapacity(memory);
		memory[memoryCount] = val;
		memoryCount++;
	}
public:
	bool isRunning;
	~Calculator() {
		for (int i = 0; i < maxHistory; i++) delete[] history[i];
		delete[] history;

		delete[] memory;
	}
	Calculator() {
		for (int i = 0; i < 26; i++)
			variables[i] = i + 65;

		historyCount = 0;
		maxHistory = 5;
		history = new char* [maxHistory];
		for (int i = 0; i < maxHistory; i++)
			history[i] = new char[100];

		memoryCount = 0;
		maxMemory = 5;
		memory = new double[maxMemory];
		diskSize = 15;
		ramRequired = 40;
		isRunning = false;
		name=deepcopy("Calculator");
	}
	double add(double a, double b) {
		if (!isRunning) return 0.0;
		addtohistory(a, b, '+');
		addtomemory(a + b);
		return a + b;
	}
	double subtract(double a, double b) {
		if (!isRunning) return 0.0;
		addtohistory(a, b, '-');
		addtomemory(a - b);
		return  a - b;
	}
	double multiply(double a, double b) {
		if (!isRunning) return 0.0;
		addtohistory(a, b, '*');
		addtomemory(a * b);
		return a * b;
	}
	double divide(double a, double b) {
		if (!isRunning) return 0.0;
		if (b == 0.0) {
			cout << "ERROR - Second Argument = 0\n";
			return 0.0;
		}
		addtohistory(a, b, '/');
		addtomemory(a / b);
		return a / b;
	}
	double power(double base, double exp) {
		if (!isRunning) return 0.0;
		if (exp == 0) return 1.0;

		double result = 1.0;
		for (int i = 0; i < exp; i++) result *= base;
		addtohistory(base, exp, '^');
		return result;
	}
	double squareRoot(double value) {
		if (!isRunning) return 0.0;
		if (value < 0) {
			cout << "ERROR - argument les than 0\n";
			return 0.0;
		}
		for (double i = 0.0; i < value / 2; i += 0.01) {
			if (i * i > value) {
				addtohistory(value, 0, '%');
				addtomemory(i - 0.01);
				return i - 0.01;
			}
		}
	}

	void storeVariable(char name, double value) {
		if (!isRunning) return;
		if (name > 91) name -= 32; // so successfully hits all lowercase
		if (name < 65 || name>91) return; // even after conversion if out of range, then discared
		variables[name - 65] = value;
	}
	double recallVariable(char name) const {
		if (!isRunning) return 0.0;
		if (name > 91) name -= 32; // so successfully hits all lowercase
		if (name < 65 || name>91) return 0.0; // even after conversion if out of range, then discared
		return variables[name - 65];
	}

	double getMean() const {
		if (!isRunning) return 0.0;
		return getSum() / memoryCount;
	}
	double getSum() const {
		if (!isRunning) return 0.0;
		double sum = 0.0;
		for (int i = 0; i < memoryCount; i++) sum += memory[i];
		return sum;
	}
	double getMin() const {
		if (!isRunning) return 0.0;
		double min = memory[0];
		for (int i = 1; i < memoryCount; i++) min = (min > memory[i] ? memory[i] : min);
		return min;
	}
	double getMax() const {
		if (!isRunning) return 0.0;
		double max = memory[0];
		for (int i = 1; i < memoryCount; i++) max = (max < memory[i] ? memory[i] : max);
		return max;
	}

	bool exportHistory(const char* filename, Storage* storage) {
		if (!isRunning) return false;
		File newfile(filename, *history, 1);
		return storage->saveFile(newfile);
	}
	void printHistory() const {
		if (!isRunning) return;
		for (int i = 0; i < historyCount; i++)
			cout << history[i] << endl;
	}
	int getMemoryCount() const {
		if (!isRunning) return false;
		return historyCount;
	}
	const char* getName() { return name; }
	int getRAM() { return ramRequired; }
	bool close() {
		if (!isRunning) return false;
		isRunning = false;
		return true;
	}
	int getdisk() { return diskSize; }

	double operator()(double a, char op, double b) {
		switch (op) {
		default:	return 0.0;
		case '+':	return add(a, b);
		case '-':	return subtract(a, b);
		case '*':	return multiply(a, b);
		case '/':	return divide(a, b);
		case '^':	return power(a, b);
		}
	}

	double operator[](char varName) const {
		return recallVariable(varName);
	}

};

class Settings {
private:
	char** keys;
	char** values;
	int count, maxPairs;
	const char* name= "Settings";
	int diskSize;
	int ramRequired;
	int length(const char* str) {
		int len = 0;
		for (; str[len] != '\0'; len++);
		return len;
	}
	bool compare(const char* str1, const char* str2) {
		if (length(str1) != length(str2)) return false;
		for (int i = 0; i < length(str1); i++)
			if (str1[i] != str2[i]) return false;
		return true;
	}
	char* deepcopy(const char* str) {
		if (str == nullptr) return nullptr;
		char* newstr = new char[length(str) + 1];
		int i = 0;
		for (; str[i] != '\0'; i++)
			newstr[i] = str[i];
		newstr[i] = '\0';
		return newstr;
	}
public:
	~Settings() {
		delete[] keys; keys = nullptr;
		delete[] values; values = nullptr;
	}
	Settings() {
		count = 5;
		keys = new char* [count];
		keys[0] = deepcopy("font_size");
		keys[1] = deepcopy("font_family");
		keys[2] = deepcopy("theme");
		keys[3] = deepcopy("language");
		keys[4] = deepcopy("date_format");
		values = new char* [count];
		values[0] = deepcopy("12");
		values[1] = deepcopy("Arial");
		values[2] = deepcopy("light");
		values[3] = deepcopy("English");
		values[4] = deepcopy("DD/MM/YYYY");
		maxPairs = 5; count = 5;
		name ==deepcopy("Settings");
		diskSize = 5;
		ramRequired = 20;
		isRunning = false;
		/* 1. font_size: Controls text size.			Valid values : "12", "14", "16", "18".".
		2. font_family : Controls the typeface.			Valid values : "Arial", "Times New Roman", "Courier New".
		3. theme : Controls the colour scheme.			Valid values : "light", "dark".Default : "light".
		4. language : Controls the display language.	Valid values : "English", "Urdu", "French".
		5. date_format : Controls how dates are shown.	Valid values : "DD/MM/YYYY", "MM-DD-YYYY". */
	}
	bool isRunning;
	const char* getName() { return name; }
	int getRAM() { return ramRequired; }
	bool close() {
		if (!isRunning) return false;
		isRunning = false;
		return true;
	}
	int getdisk() { return diskSize; }

	bool setSetting(const char* key, const char* value) {
		if (!isRunning) return false;
		int i = 0;
		for (; i < count; i++) if (compare(keys[i], key)) break;
		if (i == count) return false;
		bool valid = false;
		if (compare(key, "font_size")) {
			if (compare(value, "12") || compare(value, "14") || compare(value, "16") || compare(value, "18"))
				valid = true;
		}
		else if (compare(key, "font_family")) {
			if (compare(value, "Arial") || compare(value, "Times New Roman") || compare(value, "Courier New"))
				valid = true;
		}
		else if (compare(key, "theme")) {
			if (compare(value, "light") || compare(value, "dark")) valid = true;
		}
		else if (compare(key, "language")) {
			if (compare(value, "English") || compare(value, "Urdu") || compare(value, "French"))
				valid = true;
		}
		else if (compare(key, "date_format")) {
			if (compare(value, "DD/MM/YYYY") || compare(value, "MM/DD/YYYY"))
				valid = true;
		}
		if (!valid) return false;
		values[i] = deepcopy("value");
		return true;
	}

	const char* getSetting(const char* key) {
		if (!isRunning) return nullptr;
		int i = 0;
		for (; i < count; i++) if (compare(keys[i], key)) break;
		if (i == count) return nullptr;
		return values[i];
	}

	/*const char* getSetting(const char* key) const
		Returns the value for the given key, or nullptr if not found or not running.
	void displaySettings() const
		Prints all key - value pairs.Does nothing if not running.*/
	bool resetToDefault() {
		if (!isRunning) return false;
		values[0] =deepcopy("12");
		values[1] =deepcopy("Arial");
		values[2] =deepcopy("light");
		values[3] =deepcopy("English");
		values[4] =deepcopy("DD/MM/YYYY");
		return true;
	}
};

class RecycleBin {
private:
	File** deletedFiles; // dynamic array, starts at capacity 5
	int fileCount;
	int maxFiles;
	Storage* storage;
	char* name; // = "RecycleBin"
	int diskSize; // = 10
	int ramRequired; // = 30

	int getlength(const char* str) {
		if (str == nullptr) return 0;
		int i = 0;
		for (; str[i] != '\0'; i++);
		return i;
	}
	void doubleCapacity() {
		File** newfiles = new File * [maxFiles * 2];
		for (int i = 0; i < maxFiles; i++)
			newfiles[i] = deletedFiles[i];
		delete[] deletedFiles; deletedFiles = newfiles;
		maxFiles *= 2;
	}
	int length(const char* str) {
		int len = 0;
		for (; str[len] != '\0'; len++);
		return len;
	}
	bool compare(const char* str1, const char* str2) {
		if (length(str1) != length(str2)) return false;
		for (int i = 0; i < length(str1); i++)
			if (str1[i] != str2[i]) return false;
		return true;
	}
	File* getFile(const char* filename) {
		for (int i = 0; i < fileCount; i++)
			if (compare(deletedFiles[i]->getName(), filename)) return deletedFiles[i];
		File empty{ "", "", 0 };
		return &empty;
	}
	bool deleteFile(const char* filename) {
		int i = 0;
		for (; i < fileCount; i++)
			if (compare(deletedFiles[i]->getName(), filename)) break;
		if (i == fileCount) return false;
		delete[] deletedFiles[i];
		deletedFiles[i] = nullptr;
		for (; i < fileCount - 1; i++)
			deletedFiles[i] = deletedFiles[i + 1];
		if (fileCount) {
			fileCount--;
			if (deletedFiles[fileCount])
				delete[] deletedFiles[fileCount];
		}
		return true;
	}
public:
	bool isRunning;
	~RecycleBin() {
		if (deletedFiles) emptyBin();
	}
	RecycleBin(Storage* storage) {
		fileCount = 0;
		maxFiles = 5;
		diskSize = 10;
		ramRequired = 30;
		deletedFiles = new File * [maxFiles];
		for (int i = 0; i < maxFiles; i++)
			deletedFiles[i] = nullptr;
		name = deepcopy("RecycleBin");
		isRunning = false;
		this->storage = storage;
	}
	void addFile(const File& file) {
		if (!isRunning) {
			cout << "ERROR - RecycleBin not running\n";	return;
		}
		File* newfile = new File(file);
		if (fileCount == maxFiles) doubleCapacity();
		deletedFiles[fileCount] = newfile;
		fileCount++;
	}
	bool restoreFile(const char* filename) {
		if (!isRunning) return false;
		if (storage->isFull()) return false;
		if (!storage->saveFile(*getFile(filename))) return false;
		return deleteFile(filename);
	}
	void emptyBin() {
		if (!isRunning)return;
		for (int i = 0; i < fileCount; i++) {
			delete[] deletedFiles[i];
			deletedFiles[i] = nullptr;
		}
		delete[] deletedFiles; deletedFiles = nullptr;
		fileCount = 0;
	}
	void listBin() const {
		if (!isRunning)return;
		for (int i = 0; i < fileCount; i++) {
			cout << deletedFiles[i]->getName() << endl;
		}
	}

	int getFileCount() const { return fileCount; }
	char* getName() { return name; }
	int getRAM() { return ramRequired; }
	bool close() {
		if (!isRunning) return false;
		isRunning = false;
		return true;
	}
	int getdisk() { return diskSize; }

	void operator-(const char* filename) {
		if (!isRunning)return;
		storage->deleteFile(filename);
		deleteFile(filename);
	}
};

class FileExplorer {
private:
	Storage* storage;
	char* name; // = "FileExplorer"
	int diskSize; // = 20
	int ramRequired; // = 50

public:
	FileExplorer(Storage* storage) {
		name = deepcopy("FileExplorer");
		diskSize = 20;
		ramRequired = 50;
		isRunning = false;
		this->storage = storage;
	}
	bool isRunning;
	bool createFile(const char* name, const char* content, int size) {
		if (!isRunning) return false;
		if (storage->isFull()) return false;
		if (storage->getAvailableSpace() - size <= 0) return false;
		File* newfile = new File(name, content, size);
		storage->saveFile(*newfile);
		return true;
	}
	bool deleteFile(const char* name, RecycleBin* bin) {
		if (bin == nullptr) return false;
		if (!isRunning) return false;
		// if(filenotfound) return false;
		if (bin->isRunning) {
			if (!storage->getFile(name)) return false;
			File* file = storage->getFile(name);
			bin->addFile(*file);
		}
		return storage->deleteFile(name);
	}
	void listFiles()const {
		if (!isRunning) return;
		storage->display();
	}
	File* viewFile(const char* name) const {
		if (!isRunning) return nullptr;
		return storage->getFile(name); // file not found built into getFile
	}
	char* getName() {
		return name;
	}
	int getRAM() {
		return ramRequired;
	}
	bool close() {
		if (!isRunning) return false;
		isRunning = false;
		return true;
	}
	int getdisk() {
		return diskSize;
	}
};

class ApplicationManager {
private:
	FileExplorer* fileExplorer;
	RecycleBin* recycleBin;
	Calculator* calculator;
	Settings* settings;
	Storage* storage;
	RAM* ram;
	int length(char* str) {
		int len = 0;
		for (; str[len] != '\0'; len++);
		return len;
	}
	int length(const char* str) {
		int len = 0;
		for (; str[len] != '\0'; len++);
		return len;
	}
	bool compare(const char* str1, const char* str2) {
		if (length(str1) != length(str2)) return false;
		for (int i = 0; i < length(str1); i++)
			if (str1[i] != str2[i]) return false;
		return true;
	}
public:
	~ApplicationManager() {
		if (fileExplorer) {
			delete fileExplorer; fileExplorer = nullptr;
		}
		if (recycleBin) {
			delete recycleBin; recycleBin = nullptr;
		}
		if (calculator) {
			delete calculator; calculator = nullptr;
		}
		if (settings) {
			delete settings; settings = nullptr;
		}
	}
	ApplicationManager(Storage* storage, RAM* ram) {
		this->storage = storage;
		this->ram = ram;
		settings = new Settings;
		fileExplorer = new FileExplorer(storage);
		recycleBin = new RecycleBin(storage);
		calculator = new Calculator;
		// saves their files to Storage.
	}
	bool launchApp(const char* appName) {
		if (compare(recycleBin->getName(), appName)) {
			if (ram->allocate(recycleBin->getRAM())) {
				recycleBin->isRunning = true;
				return storage->allocate(recycleBin->getdisk());
			}
		}
		else if (compare(fileExplorer->getName(), appName)) {
			if (ram->allocate(fileExplorer->getRAM())) {
				fileExplorer->isRunning = true;
				return storage->allocate(fileExplorer->getdisk());
			}
		}
		else if (compare(calculator->getName(), appName)) {
			if (ram->allocate(calculator->getRAM())) {
				calculator->isRunning = true;
				return storage->allocate(calculator->getdisk());
			}
		}
		else if (compare(settings->getName(), appName)) {
			if (ram->allocate(settings->getRAM())) {
				settings->isRunning = true;
				return storage->allocate(settings->getdisk());
			}
		}
		else return false;
	}
	bool closeApp(const char* appName) {
		if (compare(recycleBin->getName(), appName)) {
			if (!recycleBin->close()) return false;
			else ram->deallocate(recycleBin->getRAM());
		}
		else if (compare(fileExplorer->getName(), appName)) {
			if (!fileExplorer->close()) return false;
			else ram->deallocate(fileExplorer->getRAM());
		}
		else if (compare(calculator->getName(), appName)) {
			if (!calculator->close()) return false;
			else ram->deallocate(calculator->getRAM());
		}
		else if (compare(settings->getName(), appName)) {
			if (!settings->close()) return false;
			else ram->deallocate(settings->getRAM());
		}
		else return false;
		return true;
	}
	bool isRunning(const char* appName) const {
		if (recycleBin->getName() == appName) {
			if (recycleBin->isRunning) return true;
			else return false;
		}
		else if (fileExplorer->getName() == appName) {
			if (fileExplorer->isRunning) return true;
			else return false;
		}
		else if (calculator->getName() == appName) {
			if (calculator->isRunning) return true;
			else return false;
		}
		else if (settings->getName() == appName) {
			if (settings->isRunning) return true;
			else return false;
		}
		else return false;
		return true;
	}
	void* getApp(const char* appName) {
		if (compare(recycleBin->getName(), appName))			return (void*)recycleBin;
		else if (compare(fileExplorer->getName(), appName))	return (void*)fileExplorer;
		else if (compare(calculator->getName(), appName))			return (void*)calculator;
		else if (compare(settings->getName(), appName))			return (void*)settings;
		return nullptr;
	}

	void displayAll() const {
		cout << settings->getName() << ":\n\tState: " << (settings->isRunning ? "Running" : "Not Running");
		cout << "\n\tDisk Size: " << settings->getdisk() << " MB\n\tRam Requirement: " << settings->getRAM() << " MB\n";
		cout << fileExplorer->getName() << ":\n\tState: " << (fileExplorer->isRunning ? "Running" : "Not Running");
		cout << "\n\tDisk Size: " << fileExplorer->getdisk() << " MB\n\tRam Requirement: " << fileExplorer->getRAM() << " MB\n";
		cout << calculator->getName() << ":\n\tState: " << (calculator->isRunning ? "Running" : "Not Running");
		cout << "\n\tDisk Size: " << calculator->getdisk() << " MB\n\tRam Requirement: " << calculator->getRAM() << " MB\n";
		cout << recycleBin->getName() << ":\n\tState: " << (recycleBin->isRunning ? "Running" : "Not Running");
		cout << "\n\tDisk Size: " << recycleBin->getdisk() << " MB\n\tRam Requirement: " << recycleBin->getRAM() << " MB\n";
	}

};


class Computer {
private:
	Storage* storage;
	RAM* ram;
	ApplicationManager* apps;
public:
	~Computer() {
		if (storage) { delete storage; storage = nullptr; }
		if (ram) { delete ram; ram = nullptr; }
		if (apps) { delete apps; apps = nullptr; }
	}
	Computer(int storageMB, int ramMB) {
		storage = new Storage(storageMB);
		ram = new RAM(ramMB);
		apps = new ApplicationManager(storage, ram);
		storage->allocate(50);
	}

	FileExplorer* getFileExplorer() { return (FileExplorer*)(apps->getApp("FileExplorer")); }
	RecycleBin* getRecycleBin() { return (RecycleBin*)(apps->getApp("RecycleBin")); }
	Calculator* getCalculator() { return (Calculator*)(apps->getApp("Calculator")); }
	Settings* getSettings() { return (Settings*)(apps->getApp("Settings")); }

	bool launchApp(const char* appName) { return apps->launchApp(appName); }
	bool closeApp(const char* appName) { return apps->closeApp(appName); }
	void displaySystemStatus() const {
		cout << "STORAGE:\n\tAvailable Space : " << storage->getAvailableSpace() << " MB";
		storage->display();
		cout << " RAM:\n\tAvailable Memory: " << ram->getAvailableMemory();
		cout << " MB\nAPPS - \n";
		apps->displayAll();
	}

	bool createFile(const char* name, const char* content, int size) {
		FileExplorer* fileexp = getFileExplorer();
		if (!fileexp->isRunning) return false;
		return(fileexp->createFile(name, content, size));
	}
	bool deleteFile(const char* name) {
		FileExplorer* fileexp = getFileExplorer();
		if (!fileexp->isRunning) return false;
		RecycleBin* bin = getRecycleBin();
		return fileexp->deleteFile(name, bin);
	}
	bool restoreFile(const char* name) {
		RecycleBin* bin = getRecycleBin();
		return bin->restoreFile(name);
	}
	void emptyRecycleBin() {
		RecycleBin* bin = getRecycleBin();
		bin->emptyBin();
	}
	void listFiles() const {
		FileExplorer* fileexp = (FileExplorer*)(apps->getApp("FileExplorer"));
		if (!fileexp->isRunning) return;
		fileexp->listFiles();
	}
	void listRecycleBin() const {
		RecycleBin* bin = (RecycleBin*)(apps->getApp("RecycleBin"));
		bin->listBin();
	}

	double calculate(double a, char op, double b) {
		Calculator* calc = getCalculator();
		switch (op) {
		case'+': return calc->add(a, b);
		case'-': return calc->subtract(a, b);
		case'*': return calc->multiply(a, b);
		case'/': return calc->divide(a, b);
		case'^': return calc->power(a, b);
		}
	}
	bool exportCalculations(const char* filename) {
		Calculator* calc = (Calculator*)(apps->getApp("Calculator"));
		if (!calc->isRunning) return false;
		calc->exportHistory(filename, storage);
		return true;
	}

	bool setSetting(const char* key, const char* value) {
		Settings* set = (Settings*)(apps->getApp("Settings"));
		if (!set->isRunning) return false;
		return set->setSetting(key, value);
	}
	const char* getSetting(const char* key) const {
		Settings* set = (Settings*)(apps->getApp("Settings"));
		if (!set->isRunning) return nullptr;
		return set->getSetting(key);
	}

	Computer& operator+=(const char* appName) {
		apps->launchApp(appName);
		return *this;
	}

	Computer& operator-=(const char* appName) {
		apps->closeApp(appName);
		return *this;
	}

	bool operator>(const Computer& other) const {
		int thisram = this->ram->getAvailableMemory();
		int thatram = other.ram->getAvailableMemory();
		if (thisram >= thatram) return true;
		return false;
	}

	friend ostream& operator<<(ostream& c, const Computer& comp) {
		comp.displaySystemStatus();
		return c;
	}
};
