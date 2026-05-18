#pragma once
#include <iostream>
using namespace std;

// i250922 Lab 11

class Velocity3D {
private:double vx, vy, vz;
public:
	Velocity3D(double x, double y, double z) : vx(x), vy(y), vz(z) {}

	friend Velocity3D operator+(const Velocity3D& v1, const Velocity3D& v2);
	friend Velocity3D operator*(double d, const Velocity3D& v1);
	friend Velocity3D operator*(const Velocity3D& v1, double d);
	friend istream& operator>>(istream& c, Velocity3D& v2);
	friend ostream& operator<<(ostream& c, const Velocity3D& v2);

	double getVx() const { return vx; }
	double getVy() const { return vy; }
	double getVz() const { return vz; }
	double magnitude() const { return sqrt(vx * vx + vy * vy + vz * vz); }
};

Velocity3D operator+(const Velocity3D& v1, const Velocity3D& v2) {
	Velocity3D temp(v1.vx + v2.vx, v1.vy + v2.vy, v1.vz + v2.vz);
	return temp;
}

Velocity3D operator*(double d, const Velocity3D& v1) {
	Velocity3D temp(v1.vx * d, v1.vy * d, v1.vz * d);
	return temp;
}

 Velocity3D operator*(const Velocity3D& v1, double d) {
	return d * v1;
}

 istream& operator>>(istream& c, Velocity3D& v2) {
	double x, y, z;
	c >> x >> y >> z;
	v2.vx = x, v2.vy = y, v2.vz = z;
	return c;
}

 ostream& operator<<(ostream& c, const Velocity3D& v2) {
	c << '[' << v2.vx << ", " << v2.vy << ", " << v2.vz << ']';
	return c;
}


class Ratio {
private:int part1, part2;
public:
	Ratio(int first, int second): part1(first), part2(second){}

	friend bool operator==(const Ratio lhs, const Ratio rhs);
	friend bool operator!=(const Ratio lhs, const Ratio rhs);

	friend Ratio operator*(const Ratio& lhs, int n);
	friend Ratio operator*(int n, const Ratio& lhs);
	friend istream& operator>>(istream& c, Ratio& r);
	friend ostream& operator<<(ostream& c, const Ratio& r);
	int getPart1() const { return part1; }
	int getPart2() const { return part2; }
	double toDecimal() const {
        if (!part2) return 0.0;
        return ((double)part1) /part2;
    }
};

bool operator==(const Ratio lhs, const Ratio rhs) {
	return lhs.part1 * rhs.part2 == lhs.part2 * rhs.part1;
}

bool operator!=(const Ratio lhs, const Ratio rhs) {
	return !(lhs == rhs);
}

Ratio operator*(const Ratio& lhs, int n) {
	Ratio r(lhs.part1 * n, lhs.part2 * n);
	return r;
}
Ratio operator*(int n, const Ratio& lhs) {
	return lhs * n;
}

istream& operator>>(istream& c, Ratio& v2) {
	double x, y; char rande;
	c >> x >> rande >> y;
	v2.part1 = x, v2.part2 = y;
	return c;
}

ostream& operator<<(ostream& c, const Ratio& v2) {
	c << v2.part1 << ":" << v2.part2;
	return c;
}

int clamp(int val) {
    if (val < 0) return 0;
    if (val > 255) return 255;
    return val;
}

class Color {
private: int r, g, b;
public:
    Color(int r, int g, int b) : r(clamp(r)), g(clamp(g)), b(clamp(b)) {}

    int getR() const { return r; }  int getG() const { return g; }  int getB() const { return b; }

	string toHex() const {
		const char* hex = "0123456789ABCDEF";
		string result = "#";

		result += hex[r / 16];
		result += hex[r % 16];
		result += hex[g / 16];
		result += hex[g % 16];
		result += hex[b / 16];
		result += hex[b % 16];

		return result;
	}

    friend Color operator+(const Color& c1, const Color& c2) {
        return Color( clamp(c1.r + c2.r), clamp(c1.g + c2.g), clamp(c1.b + c2.b));
    }

    friend bool operator==(const Color& c1, const Color& c2) {
        return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b;
    }

    friend ostream& operator<<(ostream& os, const Color& c) {
        os << "RGB(" << c.r << ", " << c.g << ", " << c.b << ")";
        return os;
    }

    friend istream& operator>>(istream& is, Color& c) {
        int r, g, b;
        is >> r >> g >> b;
        c.r = clamp(r);
        c.g = clamp(g);
        c.b = clamp(b);
        return is;
    }
};


class Duration {
private:
    double hours;

public:
    Duration(double hours) : hours(hours) {}

    double getHours() const { return hours; }

    double toMinutes() const {
        return hours * 60;
    }

    friend Duration operator+(const Duration& d1, const Duration& d2) {
        return Duration(d1.hours + d2.hours);
    }

    friend Duration operator*(const Duration& d, double scalar) {
        return Duration(d.hours * scalar);
    }

    friend Duration operator*(double scalar, const Duration& d) {
        return Duration(d.hours * scalar);
    }

    friend bool operator<(const Duration& d1, const Duration& d2) {
        return d1.hours < d2.hours;
    }

    friend bool operator==(const Duration& d1, const Duration& d2) {
        return d1.hours == d2.hours;
    }

    // Manual 1 decimal formatting
    friend ostream& operator<<(ostream& os, const Duration& d) {
        int whole = (int)d.hours;
        int decimal = (int)((d.hours - whole) * 10 + 0.5); // rounding
        os << whole << "." << decimal << "h";
        return os;
    }

    friend istream& operator>>(istream& is, Duration& d) {
        is >> d.hours;
        return is;
    }
};

class ComplexNum {
private:
    double real;
    double imag;

    // Simple sqrt approximation (Newton's method)
    static double mySqrt(double x) {
        double guess = x;
        for (int i = 0; i < 10; i++)
            guess = (guess + x / guess) / 2;
        return guess;
    }

public:
    ComplexNum(double real = 0, double imag = 0) : real(real), imag(imag) {}

    double getReal() const { return real; }
    double getImag() const { return imag; }

    double magnitude() const {
        return mySqrt(real * real + imag * imag);
    }

    ComplexNum conjugate() const {
        return ComplexNum(real, -imag);
    }

    friend ComplexNum operator+(const ComplexNum& a, const ComplexNum& b) {
        return ComplexNum(a.real + b.real, a.imag + b.imag);
    }

    friend ComplexNum operator-(const ComplexNum& a, const ComplexNum& b) {
        return ComplexNum(a.real - b.real, a.imag - b.imag);
    }

    friend ComplexNum operator*(const ComplexNum& a, const ComplexNum& b) {
        return ComplexNum(
            a.real * b.real - a.imag * b.imag,
            a.real * b.imag + a.imag * b.real
        );
    }

    friend ComplexNum operator*(const ComplexNum& c, double scalar) {
        return ComplexNum(c.real * scalar, c.imag * scalar);
    }

    friend ComplexNum operator*(double scalar, const ComplexNum& c) {
        return ComplexNum(c.real * scalar, c.imag * scalar);
    }

    friend bool operator==(const ComplexNum& a, const ComplexNum& b) {
        return a.real == b.real && a.imag == b.imag;
    }

    friend bool operator!=(const ComplexNum& a, const ComplexNum& b) {
        return !(a == b);
    }

    friend ostream& operator<<(ostream& os, const ComplexNum& c) {
        os << c.real;
        if (c.imag >= 0)
            os << "+" << c.imag << "i";
        else
            os << c.imag << "i";
        return os;
    }

    friend istream& operator>>(istream& is, ComplexNum& c) {
        is >> c.real >> c.imag;
        return is;
    }
};

