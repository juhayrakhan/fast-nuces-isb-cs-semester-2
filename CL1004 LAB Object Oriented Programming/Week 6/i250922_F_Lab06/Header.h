#pragma once
#include <string>
#include <cstring>
using namespace std;

// Q1

class ScoreTracker {
private:
	int score;
	int highScore;
public:
	ScoreTracker() {
		score = highScore = 0;
	}
	ScoreTracker(int startscore, int starthighscore) {
		score = startscore;
		highScore = starthighscore;
	}
	void addPoints(int n) {
		score += n;
		if (score > highScore) highScore = score;
	}
	int getScore() { return score; }
	int getHighScore() { return highScore; }
};

// Q2

class CoffeeOrder {
private:
	string type;
	int shots;
	bool hasMilk;
	int pricePerShot;
public:
	CoffeeOrder() {
		type = "Espresso";
		shots = 1;
		hasMilk= false;
		pricePerShot= 150;
	}
	CoffeeOrder(string newtype, int newshots, bool ifmilk, int newprice) {
		type = newtype;
		shots = newshots;
		hasMilk = ifmilk;
		pricePerShot = newprice*shots;
	}
	string getReceipt() { return type+" x"+to_string(shots)+" |"+(hasMilk ? " With milk" : " No milk") + " | Total: Rs." + to_string(pricePerShot); }
};


//Q3

class TemperatureConverter {
private: double celsius;
public:
	TemperatureConverter(double c) {
		celsius = c;
	}
	double toFahrenheit() { return (celsius * 9.0 / 5.0) + 32; }
	double toKelvin() { return celsius + 273.15; }
	double getCelsius() { return celsius; }
	string scaleLabel() {
		if (celsius < 15) return "Cold";
		if (celsius > 15&&celsius<35) return "Warm";
		if (celsius >35) return "Hot";
	}
};

//Q4

class Product {
private:
	string itemName;
	int quantity;
	double price;
public:
	Product() {
		itemName = "Unknown";
		quantity = 0;
		price = 0.0;
	}
	Product(string newname, int newq, double newp) {
		itemName = newname;
		quantity = newq;
		price = newp;
	}
	void restock(int val) {
		quantity += val;
	}
	void sell(int val) {
		if ((quantity-val) > 0) quantity -= val;
	}
	int totalValue() {
		return quantity * price;
	}
	int getQuantity() {
		return quantity;
	}
	string getStatus() {
		if (!quantity) return "Out of Stock";
		if (quantity<=5) return "Low Stock";
		else return "High Stock";
	}
};

//Q5


class Circle {
private: double radius;
public:
	Circle() { radius = 1.0; }
	Circle(double r) { radius=r; }
	double area() { return  3.14159 * radius * radius; }
	double circumference() { 
		return radius= 2 * 3.14159 * radius;}
	Circle scale(double factor) {
		double newr = radius * factor;
		return Circle(newr);
	}
	bool isLargerThan(Circle other) {
		if (other.radius < radius) return true; 
		else return false; }
	bool fitsInside(Circle other) { 
		if (other.radius >= radius) 
			return true; 
		else return false; }
};