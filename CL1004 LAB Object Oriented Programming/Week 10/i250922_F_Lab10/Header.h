#pragma once
#include <iostream>
#include <string>
#include <cmath>
using namespace std;

class TaxCalc {
private: double taxRate;
public:
	TaxCalc(double p): taxRate(p){}
	double operator()(double amount) const {return amount * taxRate / 100.0;}
	double operator()(double amount, double discount) const {
		return taxRate / 100.0 * (amount - (amount * discount / 100.0));
	}
	double operator()(double amount, int quantity) const {
		double total_amount = amount * quantity;
		return (taxRate / 100.0) * total_amount;
	}
	double getTaxRate() const { return taxRate; }
};

void helper(int flag) {
	switch (flag) {
	case 1: cout << "\READ\n"; break;
	case 2: cout << "\nWRITE\n"; break;
	case 4: cout << "\nEXECUTE\n"; break;
	}
}

class Permission {
private: int flags;
public:
	static const int READ = 1, WRITE = 2, EXECUTE = 4;
	Permission(int f): flags(f){}
	int operator|(const Permission& amp) { return flags | amp.getFlags(); }
	int operator&(const Permission& amp) { return flags & amp.getFlags(); }
	int operator^(const Permission& amp) { return flags ^ amp.getFlags(); }
	int operator!() { return flags==0; }
	friend ostream& operator<<(ostream& c, Permission& amp) {
		if (!(amp.getFlags() & 7)) {
			c << "[None]";
			return c;
		}
		c << "[ ";
		if (amp.getFlags() & 1) c << "Read ";
		if (amp.getFlags() & 2) c << "Write ";
		if (amp.getFlags() & 4) c << "Execute ";
		c << "]";
		return c;
	}
	int getFlags() const { return flags; }
	bool has(int flag) { return flags & flag; }
};

class Currency {
private: double amount; string code;
public:
	Currency(double a, string c): amount(a), code(c) {}
	Currency(string c, double a) : amount(a), code(c) {}
	Currency operator-() const {
		Currency c(-amount, code);
		return c;
	}
	Currency operator/(double rate) const {
		Currency c(amount / rate, code);
		return c;
	}
	Currency operator*=(double rate){
		amount*=rate;
		return *this;
	}
	Currency operator/=(double rate) {
		amount /= rate;
		return *this;
	}
	operator double() const{ return amount;}
	double getAmount() const { return amount; }
	string getCode() const { return code; }
	friend ostream& operator<<(ostream& c, Currency& cu) {
		c << cu.getCode() << " " << cu.getAmount();
		return c;
	}
	friend istream& operator>>(istream& c, Currency& cu) {
		string cod; double am;
		c >> cod >> am;
		cu = { am,cod };
		return c;
	}
};

/*
* 
• operator==(const Signal&amp;) const — true if both amplitude and frequency are equal
*/

class Signal {
private: double amplitude, frequency;
public:
	Signal(double a, double f): amplitude(a), frequency(f){}
	Signal operator *=(double gain) { amplitude *= gain; return*this; }
	Signal operator/=(double gain) { amplitude /= gain; return*this; }
	double getAmplitude() const { return amplitude; }
	double getFrequency() const { return frequency; }
	friend ostream& operator<<(ostream& c, const Signal& si) {
		c << "Signal( amp=" << si.getAmplitude() << ", freq=" << si.getFrequency() << ")";
		return c;
	}
	Signal operator-() {
		amplitude *= -1;
		return *this;
	}
	bool operator==(const Signal& other) {
		return (amplitude == other.getAmplitude()) && (frequency == other.getFrequency());
	}
};

class Amplifier {
private: double gainFactor;
public: Amplifier(double f) : gainFactor(f) {}
	  Signal operator()(const Signal& s) const {
		  Signal si(s.getAmplitude() * gainFactor, s.getFrequency());
		  return si;
	  }
};

/*
• operator&lt;&lt; — friend; outputs &quot;(x, y) [valid]&quot; or &quot;(x, y) [invalid]&quot;
• operator&gt;&gt; — friend; reads two doubles into x and y; recalculates valid
*/

class UnitVec {
private: double x, y; bool valid;
public:
	UnitVec(double nx, double ny): x(nx), y (ny), valid (ny*ny + nx* nx>0) {}
	UnitVec() : x(0), y(0), valid(false) {}
	UnitVec operator-() { UnitVec n(-x, -y); return n;}
	UnitVec operator/(double s) {
		if (s == 0) return UnitVec();
		UnitVec n(x / s, y / s);
		return n;
	}
	UnitVec operator*=(double s) {
		if (s == 0) valid = false;
		x*=s, y*=s;
		return *this;
	}
	UnitVec operator/=(double s) {
		if (s == 0)
			x = 0, y = 0, valid = false;
		else x /= s, y /= s;
		return *this;
	}
	bool operator!() { return !valid; }
	operator double() const { return sqrt(x * x + y * y); }
	UnitVec operator()(double a) const{
		UnitVec n(x * cos(a) - y * sin(a), x * sin(a) + y * cos(a));
		return n;
	}
	double getX() const { return x; }		
	double getY() const { return y; }
	bool isValid() const { return valid; }
	bool operator == (const UnitVec & other) {
		if (x != other.getX()) return false;
		if (y != other.getY()) return false;
		if (valid != other.isValid()) return false;
		return true;
	}
	friend ostream& operator<<(ostream& c, const UnitVec& vec) {
		// &quot;(x, y) [valid]&quot;
		c << "(" << vec.getX() << ", " << vec.getY() << ") [" << (vec.isValid() ? "" : "in") << "valid]";
		return c;

	}
	friend istream& operator>>(istream& c, UnitVec& vec) {
		double nx, ny;
		c >> nx >> ny;
		UnitVec n(nx, ny);
		vec = n;
		return c;
	}
};