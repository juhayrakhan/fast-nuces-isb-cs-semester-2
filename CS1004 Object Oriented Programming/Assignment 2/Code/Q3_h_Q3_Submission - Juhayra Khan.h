#pragma once
#pragma once
#include <iostream>
#include <ctime>
using namespace std;

// www.youtube.com/watch?v=CceepU1vIKo
/* comment on this vid:
D is effectively tracking the cumulative error in the rounding being performed.
When that error becomes greater than half a pixel (when D > 0), then a pixel's worth ofdifference
(along the axis of the loop, so dx) is subtracted. Each step also accumulates the error
(due to the movement perpendicular to the axis of the loop, so dy). It's a very concise way of
avoiding the unnecessary calculations. The reason the delta's are always 2x, is because the value
is being rounded so you need of offset it by half a pixel (what the initial -dx is for).
*/

int loops = 10000;
double speed = .1; // lowest 0.01, fastest .1
char initial = ' ';
char output[8] = "`^-=+e@";

void wait(double seconds) {
	if (!seconds) return;
	clock_t start = clock();
	clock_t ticks = (clock_t)(seconds * CLOCKS_PER_SEC);

	while ((clock() - start) < ticks);
}

const int screensize = 90;
char screen[screensize][screensize];
int span = screensize / 4 - (screensize / 17); // need this because it's clipping outside array otherwise, so segfault
// screensize/2 and /3 is going out of screen

void clear() {
	//system("cls"); // from documentation, rlly rlly slow
	cout << "\033[H" << "\033[?25l"; // ansii codes
	for (int y = 0; y < screensize; y++)
		for (int x = 0; x < screensize; x++) screen[y][x] = initial;
}

class Point3 {
public:
	double x, y, z;

	Point3() : x(0), y(0), z(0) {}
	Point3(double cx, double cy, double cz) : x(cx), y(cy), z(cz) {}

};
class Point2 {
public:
	int x, y;

	Point2(int cx, int cy) : x(cx), y(cy) {}
};
class Line {
public:
	Point3* p1;
	Point3* p2;
	Line(Point3* cp1, Point3* cp2) : p1(cp1), p2(cp2) {}
};

Point2 project(Point3* p3) {

	double d = 3.0;

	double z = p3->z + d;
	if (z < 0.1) z = 0.1;
	double scalefactor = d / z;

	int x = (p3->x * scalefactor * span + (screensize / 2));
	int y = (p3->y * scalefactor * span + (screensize / 2));

	return Point2(x, y);
}

void rotate(Point3* p, float theta) {
	double cost = cos(theta);
	double sint = sin(theta);

	double x = p->x;
	double y = p->y;
	double z = p->z;

	double newX = x * cost - z * sint;
	double newZ = x * sint + z * cost;
	double newY = y * cost - newZ * sint;

	newZ = y * sint + newZ * cost;

	p->x = newX;
	p->y = newY;
	p->z = newZ;
}

class Model {
public:

	Point3** anchors;
	Line** links;

	int anchorCount;
	int linkCount;

	Model(int a, int l) : anchorCount(a), linkCount(l) {
		anchors = new Point3 * [a];
		links = new Line * [l];
	}

	~Model() {
		for (int i = 0; i < anchorCount; i++) delete anchors[i];
		for (int i = 0; i < linkCount; i++)	delete links[i];
		if (anchors) delete[] anchors;
		if (links) delete[] links;
		anchors = nullptr;
		links = nullptr;
	}
};

char checkdepth(double depth) {
	if (depth > 0.6) return output[0];
	else if (depth > 0.4) return output[1];
	else if (depth > 0.2) return output[2];
	else if (depth > 0.0) return output[3];
	else if (depth > -0.2) return output[4];
	else if (depth > -0.4) return output[5];
	else				 return output[6];
}

void drawLine(Point2* p1, Point2* p2, double depth) {

	int dx = (p2->x > p1->x ? p2->x - p1->x : p1->x - p2->x);
	int dy = (p2->y > p1->y ? p2->y - p1->y : p1->y - p2->y);

	int dirx = (p1->x < p2->x) ? 1 : -1;
	int diry = (p1->y < p2->y) ? 1 : -1;

	int initerr = dx - dy;
	char ch = checkdepth(depth);

	while (p1->x != p2->x || p1->y != p2->y) {

		screen[p1->y][p1->x] = ch;

		int cumulerr = initerr << 1;

		if (cumulerr > -dy) {
			initerr -= dy; p1->x += dirx;
		}
		if (cumulerr < dx) {
			initerr += dx; p1->y += diry;
		}
	}
	if (p1->x >= 0 && p1->x < screensize && p1->y >= 0 && p1->y < screensize)
		screen[p1->y][p1->x] = ch;
}

void border(){
	for (int i = 0; i < screensize; i++) {
		screen[i][0] = '|';
		screen[i][screensize - 1] = '|';
		screen[screensize - 1][i] = '-';
		screen[0][i] = '-';
	}
	screen[0][0] = screen[0][screensize - 1] = screen[screensize - 1][0] = screen[screensize - 1][screensize - 1] = '+';
}

void renderScreen() {
	for (int i = 0; i < screensize; i++) {
		for (int j = 0; j < screensize; j++)
			cout << screen[i][j];
		cout << '\n';
	}
}