#pragma once


/*
Create a class Box with private double width, height, depth.
• Constructor takes three doubles
• volume() const — returns width * height * depth
• surfaceArea() const — returns 2 * (w*h + h*d + w*d)
• longestEdge() const — returns the largest of width, height, depth
• isCube() const — returns true if all three dimensions are equal
• canFit(const Box&amp; other) const — returns true if all three dimensions of other are strictly less
than this box&#39;s dimensions
Tests will create Box objects on the heap using new, call all methods via -&gt;, then delete them.
*/

class Box {
private:
	double width;
	double height;
	double depth;

public:

	Box(double w, double h, double d): width(w), height(h) {
		width = w;
		height = h;
		depth = d;
	}

	double volume() const {
	   int vol= width * height * depth;
	   return vol;
	}

	double surfaceArea() const {
		int area=2 * (width * height + height * depth + width * depth);
		return area;
	}

	double longestEdge() const {
		if (width >= height && width >= depth)
			return width;
		if (depth >= height && depth >= width) 
			return depth;
		if (height >= depth && height >= width) 
			return height;
	}

	bool isCube() const {
		if ((width == height) && (height == depth) && (width == depth)) return true;
		return false;
	}

	bool canFit(const Box& other) const {
		if ((other.width <= width) && (other.height <= height) && (other.depth <= depth)) return true;
		return false;
	}
};

/*
Scenario: A city manages multiple parking lots from a central system.
Create a class ParkingLot with private: char* lotName, int capacity, int localOccupied. Static: int
totalOccupied, int totalCapacity.
• Constructor takes lotName and capacity; adds capacity to totalCapacity; localOccupied starts at 0
• park() — non-const; increments both localOccupied and totalOccupied if localOccupied &lt;
capacity; returns true on success, false if full
• leave() — non-const; decrements both if localOccupied &gt; 0; returns true on success
• getCapacity() const,
• getLocalOccupied() const — local getters
• static getTotalOccupied(),
• static getTotalCapacity() — system-wide getters
• isFull() const — returns true if localOccupied &gt;= capacity
• availableSpots() const — returns capacity - localOccupied
*/

class ParkingLot {
private:
	char* lotName;
	int capacity;
	int localOccupied;
	static int totalOccupied;
	static int totalCapacity;

public:
	
	ParkingLot(char* name, int cap) {
		lotName = name;
		capacity = cap;
		localOccupied = 0;
		totalCapacity += capacity;
	}

	bool park() {
		if (localOccupied < capacity) {
			localOccupied++;
			totalOccupied++;
			return true;
		}
		return false;
	}

	bool leave() {
		if (localOccupied > 0) {
			localOccupied--;
			totalOccupied--;
			return true;
		}
		return false;
	}

	int getCapacity() const { return capacity; }

	int getLocalOccupied() const {return localOccupied; }

	static int getTotalOccupied() {return totalOccupied; }

	static int getTotalCapacity() { return totalCapacity; }

	bool isFull() const { 
		if(localOccupied >= capacity) return true;
		return false;
	}

	int availableSpots() const {
		int spots = capacity - localOccupied;
		return spots;
	}
};


/*
Create a class Point with private double x and y.
• Constructor takes two doubles
• getX() const and
• getY() const — read-only getters
• distanceTo(const Point&amp; other) const — Euclidean distance: sqrt((x2-x1)^2 + (y2-y1)^2)
• isOrigin() const — true if x == 0.0 &amp;&amp; y == 0.0
• translate(double dx, double dy) — non-const; shifts x by dx and y by dy
• quadrant() const — returns 1, 2, 3, or 4 based on sign of x and y; returns 0 if on an axis
All getter/calculation functions must be const so they work on const Point references.
*/

class Point {
private:
	double x;
	double y;

public:

	Point(double xVal, double yVal) {
		x = xVal;
		y = yVal;
	}


	double getX() const { return x; }
	double getY() const { return y;	}

	double distanceTo(const Point& other) const {
		double dx = other.x - x;
		double dy = other.y - y;
		return sqrt(dx * dx + dy * dy);
	}

	bool isOrigin() const {
		if (!x && !y)
			return true;
		return false;
	}

	void translate(double dx, double dy) {
		x += dx;
		y += dy;
	}

	int quadrant() const {
		if (!x||!y)
			return 0;
		if (x>0&&y>0)
			return 1;
		if (x<0&&y>0)
			return 2;
		if (x<0&&y<0)
			return 3;
		if (x>0&&y<0)
			return 4;
		return 0;
	}
};

/*
Scenario: A logistics company tracks its vehicle fleet and total distance driven.
Create a class Vehicle with private: char* plateNumber, char* type, int mileage. Static: int fleetSize, int
totalMileage.
• Constructor takes plateNumber, type, and initial mileage; increments fleetSize; adds mileage to
totalMileage
• getPlate() const,
• getType() const,
• getMileage() const — getters
• addMileage(int km) — non-const; adds km to both mileage and totalMileage; ignore if km &lt;= 0
• static getFleetSize() and
• static getTotalMileage() — static getters
• needsService() const — returns true if mileage &gt;= 10000
• getServiceStatus() const — returns &quot;Overdue&quot; (&gt;= 15000), &quot;Due Soon&quot; (&gt;= 10000), or &quot;Good&quot;
Tests create Vehicle objects via pointer (new/delete) and call through the arrow operator.
*/

class Vehicle {
private:
	const char* plateNumber;
	const char* type;
	int mileage;

	static int fleetSize;
	static int totalMileage;

public:

	Vehicle(const char* plate, const char* t, int initialMileage) {
		plateNumber = plate;
		type = t;
		mileage = initialMileage;

		fleetSize++;
		totalMileage += mileage;
	}

	const char* getPlate() const {	return plateNumber;	}

	const char* getType() const {	return type;  }

	int getMileage() const {return mileage;	}

	void addMileage(int km) {
		if (km < 0) return;
		mileage += km;
		totalMileage += km;
	}

	static int getFleetSize() {return fleetSize;}

	static int getTotalMileage() { return totalMileage;}

	bool needsService() const {
		if(mileage >= 10000) return true;
		return false;
	}

	const char* getServiceStatus() const {
		if (mileage >= 15000) return "Overdue";
		if (mileage >= 10000) return "Due Soon";
		return "Good";
	}
};

/*
centrally.
Create a class Sensor with private: const char* sensorId, const char* zone, double reading, bool active. Static: int
activeSensors, double totalReadings, int readingCount.
• Constructor takes sensorId, zone, and initial reading; sets active = true; increments
activeSensors; adds reading to totalReadings and increments readingCount
• getId() const,
• getZone() const,
• getReading() const,
• isActive() const — const getters
• updateReading(double) — non-const; updates reading; adds to totalReadings; increments
readingCount
• deactivate() — non-const; sets active = false; decrements activeSensors
• static getActiveSensors(),
• static getNetworkAverage() — returns totalReadings / readingCount (0.0 if no readings)
• getAlert() const — returns:
◦ &quot;OFFLINE&quot; if not active
◦ &quot;CRITICAL&quot; if reading &gt;= 80
◦ &quot;WARNING&quot; if reading &gt;= 60
◦ &quot;NORMAL&quot; if reading &lt; 60
Tests will create Sensor objects on the heap using new, call all methods via -&gt;, then delete them.
*/

class Sensor {
private:
	const char* sensorId;
	const char* zone;
	double reading;
	bool active;

	static int activeSensors;
	static double totalReadings;
	static int readingCount;

public:
	
	Sensor(const char* id, const char* z, double initialReading) {
		sensorId = id;
		zone = z;
		reading = initialReading;
		active = true;

		activeSensors++;
		totalReadings += reading;
		readingCount++;
	}

	const char* getId() const {	return sensorId;	}

	const char* getZone() const { return zone; }

	double getReading() const {	return reading;	}

	bool isActive() const {	return active;	}

	void updateReading(double newReading) {
		reading = newReading;
		totalReadings += newReading;
		readingCount++;
	}

	void deactivate() {
		if (!active) return;
		active = false;
		activeSensors--;
	}

	static int getActiveSensors() {	return activeSensors;	}

	static double getNetworkAverage() {
		if (readingCount == 0) return 0.0;
		double av = totalReadings / readingCount;
		return av;
	}

	const char* getAlert() const {
		if (!active) return "OFFLINE";
		if (reading >= 80) return "CRITICAL";
		if (reading >= 60) return "WARNING";
		return "NORMAL";
	}
};

// q5 sttic
int Sensor::activeSensors = 0;
double Sensor::totalReadings = 0.0;
int Sensor::readingCount = 0;


//q2 staticc
int ParkingLot::totalOccupied = 0;
int ParkingLot::totalCapacity = 0;

// tis is for q4
int Vehicle::fleetSize = 0;
int Vehicle::totalMileage = 0;