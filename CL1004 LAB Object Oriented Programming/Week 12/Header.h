#pragma once
#include <string>
using namespace std;

class CPU {
private: string brand; double speedGHz;
public:
	CPU(string brand, double speedGHz) : brand(brand), speedGHz(speedGHz){}
	string getBrand() const { return brand; }
	double getSpeed() const { return speedGHz; }
};

class RAM {
private: int sizeGB;
public:
	RAM(int sizeGB) : sizeGB(sizeGB){}
	int getSize() const { return sizeGB; }
};

class Computer {
private: string model;  CPU processor; RAM memory;
public:
	Computer(string model, string brand, double speed, int ramSize) :
		model(model), memory(ramSize), processor(brand, speed){}
	string getModel() const { return model; }
	string getCPUBrand() const { return processor.getBrand(); }
	double getCPUSpeed() const { return processor.getSpeed(); }
	int getRAMSize() const { return memory.getSize(); }
};

class Song {
private: string title, artist; int durationSeconds;
public:
	Song(string title, string artist, int durationSeconds) :
		title(title), artist(artist), durationSeconds(durationSeconds){}
	string getTitle() const { return title; }
	string getArtist() const { return artist; }
	int getDuration() const { return durationSeconds; }
};
class Playlist {
private: string playlistName; Song* songs[100]; int songCount;
public:
	Playlist(string playlistname): playlistName(playlistname), songCount(0){}
	void addSong(Song* s) { if (songCount < 100) songs[songCount] = s; songCount++; }
	Song* getSong(int index) { if (index<0||index >= songCount) return nullptr; return songs[index]; }
	int getSongCount() const { return songCount; }
	int getTotalDuration() const {
		int sum = 0;
		for (int i = 0; i < songCount; i++)
			sum += songs[i]->getDuration();	
		return sum;
	}
	string getPlaylistName() const { return playlistName; }
};

class Transaction {
private: string type; double amount;

public:
	Transaction(string type, double amount): type(type), amount(amount) {}
	string getType() const {return type;}
	double getAmount() const {	return amount;}
};

class BankAccount {
private:
	string accountNumber;
	double balance;
	Transaction* transactions[100];
	int transactionCount;

public:
	BankAccount(string accountNumber, double initialBalance):
		accountNumber(accountNumber), balance(initialBalance),transactionCount(0) {	}
	void deposit(double amount) {
		balance += amount;
		if (transactionCount < 100) {
			transactions[transactionCount++] = new Transaction("deposit", amount);
		}
	}
	void withdraw(double amount) {
		if (amount <= balance) {
			balance -= amount;
			if (transactionCount < 100)
				transactions[transactionCount++] = new Transaction("withdraw", amount);
		}
	}
	double getBalance() const {	return balance;}
	int getTransactionCount() const {return transactionCount;}
	Transaction getTransaction(int index) const {return *transactions[index];}
	string getAccountNumber() const {return accountNumber;}
	~BankAccount() {
		for (int i = 0; i < transactionCount; i++) delete transactions[i];
	}
};

class Employee {
private: string name;int id; double salary;
public:
	Employee(string name, int id, double salary)
		: name(name), id(id), salary(salary) {}
	string getName() const { return name; }
	int getId() const { return id; }
	double getSalary() const { return salary; }
};

class Department {
private: string deptName; Employee* employees[50];int employeeCount;
public:
	Department(string deptName)	: deptName(deptName), employeeCount(0) {}
	void addEmployee(Employee* e) {	if (employeeCount < 50)	employees[employeeCount++] = e; }

	int getEmployeeCount() const {	return employeeCount;	}
	string getDeptName() const {return deptName;}
};

class University {
private:
	string universityName;
	Department* departments[20];
	int deptCount;

public:
	University(string universityName): universityName(universityName), deptCount(0) {}
	void addDepartment(Department* d) {	if (deptCount < 20)	departments[deptCount++] = d;}
	Department* getDepartment(int index) const {
		if (index < 0 || index >= deptCount) return nullptr;
		return departments[index];
	}
	int getDeptCount() const {	return deptCount;}
	string getUniversityName() const {return universityName;}
	int getTotalEmployees() const {
		int total = 0;
		for (int i = 0; i < deptCount; i++)
			total += departments[i]->getEmployeeCount();
		return total;
	}
};

class Device {
private:string name;bool isOn;	int powerConsumption;
public:
	Device(string name, int powerConsumption): name(name), isOn(false), powerConsumption(powerConsumption) {}
	void turnOn() {isOn = true;}
	void turnOff() {isOn = false;}
	string getName() const {return name;}
	bool getIsOn() const {return isOn;}
	int getPowerConsumption() const {return powerConsumption;}
};

class Room {
private: string roomName;Device* devices[10];int deviceCount;
public:
	Room(string roomName): roomName(roomName), deviceCount(0) {}
	void addDevice(Device* d) {	if (deviceCount < 10) devices[deviceCount++] = d;	}
	int getDeviceCount() const {return deviceCount;}
	string getRoomName() const { return roomName; }
	int getTotalPower() const {
		int total = 0;
		for (int i = 0; i < deviceCount; i++) if (devices[i]->getIsOn())
			total += devices[i]->getPowerConsumption();
		return total;
	}
};

class SmartHome {
private:string homeName; Room livingRoom, bedroom;
public:
	SmartHome(string homeName) : homeName(homeName), livingRoom("Living Room"), bedroom("Bedroom") {}
	Room& getLivingRoom() {	return livingRoom;	}
	Room& getBedroom() {return bedroom;}
	int getTotalHomePower() const {	return livingRoom.getTotalPower() + bedroom.getTotalPower();}
	string getHomeName() const {return homeName;}
};