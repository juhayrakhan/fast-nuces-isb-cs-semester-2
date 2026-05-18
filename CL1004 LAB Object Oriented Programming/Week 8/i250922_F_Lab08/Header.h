#pragma once
#include <string>
#include <cmath>
using namespace std;

class EmailBuilder {
private:
	string sender;
	string recipient;
	string subject;
	string body;
	bool hasAttachment;
public:
	EmailBuilder(string s) : sender(s), recipient(""), subject(""), body(""), hasAttachment(false){}
	EmailBuilder to(string r) { recipient = r; return *this; }
	EmailBuilder withSubject(string s) { subject=s; return *this; }
	EmailBuilder withBody(string b) { body=b; return *this; }
	EmailBuilder attach() { hasAttachment=true; return *this; }

	string build() const{
		if (recipient=="" || subject == "" || body == "") return "Error: incomplete email";
		string ret = "From: " + sender + "\nTo: " + recipient + "\nSubject: " + subject + "\nBody: " + body;
		if (hasAttachment) ret += "\n[Attachment]";
		return ret;
	}
};

class InventoryItem {
private:
	string itemName; string supplier;
	double unitPrice; int quantity;
public:
	static int totalItems;
	static int totalQuantity;
	InventoryItem(string itemn, string supp, double up, int q) : itemName(itemn), supplier(supp), unitPrice(up), quantity(q) {
		totalItems++;
		totalQuantity += quantity;
	}
	InventoryItem(const InventoryItem& other) : itemName(other.itemName + " (Clone)"), supplier(other.supplier), unitPrice(other.unitPrice), quantity(other.quantity) {
		totalItems++;
		totalQuantity += quantity;
	}
	string getItemName() const { return itemName; }
	string getSupplier() const { return supplier; }
	double getUnitPrice() const { return unitPrice; }
	int getQuantity() const { return quantity; }
	static int getTotalItems() { return totalItems; }
	static double getTotalQuantity() { return totalQuantity; }

	InventoryItem& restock(int qty) {
		if (qty <= 0) return *this;
		totalQuantity += qty;
		quantity += qty;
		return *this;
	}

	InventoryItem& sell(int qty) {
		if (qty > quantity) return *this;
		totalQuantity -= qty;
		quantity -= qty;
		return* this;
	}

	InventoryItem& applyPriceChange(double percent) {
		unitPrice *= (1.0 + percent / 100.0);
		return*this;

	}

	double totalValue() const {
		return quantity * unitPrice;
	}
	~InventoryItem() {
		totalItems = 0;
		totalQuantity = 0;
	}
};
int InventoryItem::totalItems = 0;
int InventoryItem::totalQuantity = 0;

class SensorLogger {
private:
	const string sensorId, unit;
	double currentValue, minRecorded, maxRecorded;
	int readingCount;

public:
	SensorLogger(string id, double initialValue, string u): sensorId(id), unit(u){
		currentValue = minRecorded = maxRecorded = initialValue;
		readingCount = 1;
	}

	string getSensorId() const { return sensorId; }
	string getUnit() const { return unit; }
	double getCurrentValue() const { return currentValue; }
	double getMin() const { return minRecorded; }
	double getMax() const { return maxRecorded; }
	int getReadingCount() const { return readingCount; }
	SensorLogger& record(double val) {
		currentValue = val;
		if (val < minRecorded) minRecorded = val;
		if (val > maxRecorded) maxRecorded = val;
		readingCount++;
		return *this;
	}
	double getRange() const {
		return maxRecorded - minRecorded;
	}
	string getSummary() const {
		string s=  "[" + sensorId + "] cur:" + to_string((int)currentValue) + " min:" + to_string((int)minRecorded) +" max:" + to_string((int)maxRecorded) + " (" + to_string(readingCount) + " readings)";
		return s;
	}
};

class Product {
private:
	string name;
	double price;
	int stock, soldCount;
public:
	static int totalProducts;
	static double totalRevenue;
	Product(string n, double p, int s): name(n), price(p), stock(s), soldCount(0){	totalProducts++;}
	Product(const Product& other): name(other.name + " (Variant)"),	price(other.price),	stock(other.stock),	soldCount(0){
		totalProducts++;
	}
	~Product(){ totalProducts--;}

	string getName() const { return name; }
	double getPrice() const { return price; }
	int getStock() const { return stock; }
	int getSoldCount() const { return soldCount; }
	static int getTotalProducts() { return totalProducts; }
	static double getTotalRevenue() { return totalRevenue; }
	Product& applyDiscount(double percent) {
		if (percent > 0 && percent < 100)	price *= (1.0 - percent / 100.0);
		return *this;
	}
	Product& restock(int qty) {
		if (qty > 0) stock += qty;
		return *this;
	}
	Product& sell(int qty) {
		if (qty > stock) return *this;
		stock -= qty;
		soldCount += qty;
		totalRevenue += qty * price;
		return *this;
	}
	string getStockStatus() const {
		if (stock == 0) return "Out of Stock";
		else if (stock <= 5) return "Low Stock";
		else return "In Stock";
	}
};
int Product::totalProducts = 0;
double Product::totalRevenue = 0.0;

class Patient {
private:
	string name;
	const int patientId;
	string ward;
	double temperature;
	int wardTransfers;
public:

	static int totalPatients;
	static int totalTransfers;

	Patient(string n, int id, string w, double temp): name(n), patientId(id), ward(w), temperature(temp), wardTransfers(0){
		totalPatients++;
	}
	Patient(const Patient& other): name(other.name + " (Transfer)"),patientId(other.patientId), ward(other.ward),temperature(other.temperature), wardTransfers(0){
		totalPatients++;
	}

	string getName() const { return name; }
	int getId() const { return patientId; }
	string getWard() const { return ward; }
	double getTemperature() const { return temperature; }
	int getWardTransfers() const { return wardTransfers; }
	static int getTotalPatients() { return totalPatients; }
	static int getTotalTransfers() { return totalTransfers; }
	Patient& transfer(string newW) {
		ward = newW;
		wardTransfers++;
		totalTransfers++;
		return *this;
	}
	Patient& recordTemp(double newtemp) {
		temperature = newtemp;
		return *this;
	}
	string getCondition() const {
		if (temperature >= 39.5) return "Critical";
		else if (temperature >= 38) return "Fever";
		else return "Normal";
	}
	string getPatientSummary() const {
		string s = name + " | Ward: " + ward + " | Temp: " + to_string(temperature) + "C | Transfers: " +to_string(wardTransfers);
		return s;
	}
};
int Patient::totalPatients = 0;
int Patient::totalTransfers = 0;