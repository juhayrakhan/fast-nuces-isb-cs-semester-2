#pragma once
#include <iostream>
#include <string>
using namespace std;

class Temperature {
private:
	double celsius;
public:
	Temperature(double c) : celsius(c) {}
	double getCelsius() const { return celsius; }
	double toFahrenheit() const { return (celsius * 9.0 / 5.0) + 32; }

	Temperature operator+(const Temperature& amp) const {
		Temperature temp(this->celsius+amp.getCelsius());
		return temp;
	}
	bool operator==(const Temperature& amp) const {
		if (this->celsius == amp.getCelsius()) return true;
		return false;
	}
	bool operator>(const Temperature& amp) const {
		if (this->celsius > amp.getCelsius()) return true;
		return false;
	}
	bool operator<(const Temperature& amp) const {
		if (this->celsius < amp.getCelsius()) return true;
		return false;
	}
	bool operator<=(const Temperature& amp) const {
		if (this->celsius <= amp.getCelsius()) return true;
		return false;
	}
	bool operator>=(const Temperature& amp) const {
		if (this->celsius >= amp.getCelsius()) return true;
		return false;
	}
	friend ostream& operator<<(ostream& amp, const Temperature& temp) {
		amp << temp.celsius << 'C';
		return amp;
	}
};

/*
• operator&lt;&lt; — friend; outputs: &quot;&lt;label&gt;: [s0, s1, s2, s3, s4]&quot;
• getLabel() const — const getter
• average() const — returns sum of all 5 scores divided by 5.0*/

class ScoreList {
private:
	int scores[5];
	string label;
	int count;
public:
	ScoreList(string l) : label(l), count(0) {
		for (int i = 0; i < 5; i++) scores[i] = 0;
	}
	string getLabel() const { return label; }
	double average() const {
		double av = 0;
		for (int i = 0; i < 5; i++)
			av += scores[i];
		av /= 5; return av;
	}
	int& operator[] (int i) {
		return scores[i];
	}
	int operator[](int i) const {
		return scores[i];
	}
	ScoreList operator+(const ScoreList& amp) {
		ScoreList newlist(this->label + '+' + amp.getLabel());
		for (int i = 0; i < 5; i++)
			newlist[i] = scores[i] + amp[i];
		return newlist;
	}
	bool operator==(const ScoreList& amp) const {
		for(int i=0;i<5;i++)
			if (scores[i] != amp[i]) return false;
		return true;
	}
	friend ostream& operator<<(ostream& amp, const ScoreList& temp) {
		amp << temp.getLabel() << ": [";
		for (int i = 0; i < 4; i++)
			amp << temp[i] << ", ";
		amp << temp[4] << "]";
		return amp;
	}
};


class TextBuffer {
private:
	string content;
	int maxCapacity, version;
public:
	TextBuffer(string c, int maxcap): content(c), maxCapacity(maxcap), version(1) {}
	string getContent() const { return content; }
	int getMaxCapacity() const { return maxCapacity; }
	int getVersion() const { return version; }
	TextBuffer operator=(const TextBuffer& rhs) {
		content = rhs.getContent();
		maxCapacity = rhs.getMaxCapacity();
		version = 1;
		return *this;
	}
	friend ostream& operator<<(ostream& amp, const TextBuffer& temp) {
		amp << "Buffer(v" << temp.getVersion()<< "): '" << temp.getContent()<< '\'';
		return amp;
	}
	bool operator==(const TextBuffer& amp) const {
		if (content == amp.getContent()) return true;
		return false;
	}
	TextBuffer operator+=(const string& rhs) {
		if (rhs.length() + content.length() > maxCapacity) return *this;
		content += rhs;
		version++;
		return *this;
	}
};

class Inventory {
private:
	string itemName;
	int quantity;
	double unitPrice;
public:
	Inventory(string itemName, int quantity, double unitPrice): itemName(itemName), quantity(quantity), unitPrice(unitPrice) {}
	string getItemName() const { return itemName; }
	int getQuantity() const { return quantity; }
	double getUnitPrice() const { return unitPrice; }

	Inventory& operator+=(int qty) {
		if (qty > 0) quantity += qty;
		return *this;
	}
	Inventory& operator-=(int qty) {
		if (qty > 0 && qty <= quantity) quantity -= qty;
		return *this;
	}
	Inventory operator+(const Inventory& rhs) const {
		return Inventory(itemName,quantity + rhs.getQuantity(), unitPrice);
	}
	bool operator==(const Inventory& other) const {
		if (itemName == other.getItemName() && quantity == other.getQuantity()) return true;
		return false;
	}
	friend ostream& operator<<(ostream& out, const Inventory& inv) {
		out << inv.getItemName() << ": " << inv.getQuantity() << " units @ Rs." << inv.getUnitPrice();
		return out;
	}
	double totalValue() const {
		double val= quantity * unitPrice;
		return val;
	}
};

class Fraction {
private:
	int numerator;
	int denominator;
	static int gcd(int a, int b) {
		if (b == 0) return a;
		return gcd(b, a % b);
	}
	void reduce() {
		int g = gcd(abs(numerator), abs(denominator));
		numerator /= g;
		denominator /= g;
		if (denominator < 0) {
			numerator *= -1; denominator *= -1;
		}
	}

public:
	Fraction(int n, int d) : numerator(n), denominator(d == 0 ? 1 : d) { reduce();}
	int getNumerator() const { return numerator; }
	int getDenominator() const { return denominator; }

	Fraction operator+(const Fraction& f) const {
		int n = numerator * f.denominator + f.numerator * denominator;
		int d = denominator * f.denominator;
		return Fraction(n, d);
	}
	Fraction operator-(const Fraction& f) const {
		int n = numerator * f.denominator - f.numerator * denominator;
		int d = denominator * f.denominator;
		return Fraction(n, d);
	}
	Fraction operator*(const Fraction& f) const {
		return Fraction(numerator * f.numerator,
			denominator * f.denominator);
	}
	Fraction& operator+=(const Fraction& f) {
		numerator = numerator * f.denominator + f.numerator * denominator;
		denominator = denominator * f.denominator;
		reduce();
		return *this;
	}
	bool operator==(const Fraction& f) const {
		if(numerator * f.denominator == denominator * f.numerator) return true;
		return false;
	}
	bool operator!=(const Fraction& f) const {
		if (numerator * f.denominator == denominator * f.numerator) return false;
		return true;
	}
	bool operator<(const Fraction& f) const {
		if (numerator* f.denominator < denominator * f.numerator) return true;
		return false;
	}
	friend ostream& operator<<(ostream& out, const Fraction& f) {
		out << f.numerator << "/" << f.denominator;
		return out;
	}
};