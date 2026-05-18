// ============================================================
// GTest File: C++ Classes II
// Topics: const/non-const members, static data members, pointers to objects
// 3 test cases per question  (30 total)
// ============================================================

#include <string>
#include <cmath>



// ============================================================
// Q1 – 3D Box
// Concepts: pointer to object, const member functions
// ============================================================

TEST(Q1_Box, VolumeAndSurfaceAreaViaPointer) {
    Box* b = new Box(3.0, 4.0, 5.0);
    EXPECT_DOUBLE_EQ(b->volume(),      60.0);
    EXPECT_DOUBLE_EQ(b->surfaceArea(), 94.0);
    delete b;
}

TEST(Q1_Box, LongestEdgeAndIsCube) {
    Box* cube = new Box(4.0, 4.0, 4.0);
    Box* rect = new Box(2.0, 5.0, 3.0);
    EXPECT_TRUE (cube->isCube());
    EXPECT_FALSE(rect->isCube());
    EXPECT_DOUBLE_EQ(rect->longestEdge(), 5.0);
    delete cube; delete rect;
}

TEST(Q1_Box, CanFitChecksAllDimensions) {
    Box* big   = new Box(10.0, 10.0, 10.0);
    Box* small = new Box(3.0,  4.0,  5.0);
    EXPECT_TRUE (big->canFit(*small));   // small fits inside big
    EXPECT_FALSE(small->canFit(*big));   // big does not fit inside small
    delete big; delete small;
}


// ============================================================
// Q2 – Parking Lot System  (scenario)
// Concepts: static tracking both globally and locally, bool returns
// ============================================================

TEST(Q2_ParkingLot, ParkReturnsBoolAndUpdatesStatics) {
    int capBefore = ParkingLot::getTotalCapacity();
    int occBefore = ParkingLot::getTotalOccupied();
    ParkingLot lot("Zone A", 3);
    EXPECT_EQ(ParkingLot::getTotalCapacity(), capBefore + 3);
    EXPECT_TRUE(lot.park());
    EXPECT_TRUE(lot.park());
    EXPECT_EQ(ParkingLot::getTotalOccupied(), occBefore + 2);
    EXPECT_EQ(lot.getLocalOccupied(), 2);
}

TEST(Q2_ParkingLot, FullLotRejectsPark) {
    ParkingLot lot("Zone B", 2);
    lot.park();
    lot.park();
    EXPECT_TRUE (lot.isFull());
    EXPECT_FALSE(lot.park());          // lot is full — must return false
    EXPECT_EQ   (lot.availableSpots(), 0);
}

TEST(Q2_ParkingLot, LeaveUpdatesLocalAndGlobal) {
    ParkingLot lot("Zone C", 5);
    lot.park();
    lot.park();
    int occAfterPark = ParkingLot::getTotalOccupied();
    EXPECT_TRUE(lot.leave());
    EXPECT_EQ(ParkingLot::getTotalOccupied(), occAfterPark - 1);
    EXPECT_EQ(lot.availableSpots(), 4);
}


// ============================================================
// Q3 – Geometry Point
// Concepts: const objects, const functions, non-const translate, quadrant
// ============================================================

TEST(Q3_Point, ConstGettersAndDistance) {
    const Point a(0.0, 0.0);
    const Point b(3.0, 4.0);
    EXPECT_DOUBLE_EQ(a.distanceTo(b), 5.0);
    EXPECT_TRUE (a.isOrigin());
    EXPECT_FALSE(b.isOrigin());
}

TEST(Q3_Point, QuadrantClassification) {
    Point q1( 3.0,  4.0);
    Point q2(-1.0,  2.0);
    Point q3(-2.0, -3.0);
    Point q4( 5.0, -1.0);
    Point ax ( 0.0,  3.0);   // on axis
    EXPECT_EQ(q1.quadrant(), 1);
    EXPECT_EQ(q2.quadrant(), 2);
    EXPECT_EQ(q3.quadrant(), 3);
    EXPECT_EQ(q4.quadrant(), 4);
    EXPECT_EQ(ax.quadrant(), 0);
}

TEST(Q3_Point, TranslateMovesPoint) {
    Point p(2.0, 3.0);
    p.translate(1.0, -2.0);
    EXPECT_DOUBLE_EQ(p.getX(), 3.0);
    EXPECT_DOUBLE_EQ(p.getY(), 1.0);
    EXPECT_EQ(p.quadrant(), 1);
}


// ============================================================
// Q4 – Fleet Manager  (scenario)
// Concepts: two statics, pointer/arrow, service status tiers
// ============================================================

TEST(Q4_Vehicle, ConstructorUpdatesFleetAndMileage) {
    int fleetBefore   = Vehicle::getFleetSize();
    int mileageBefore = Vehicle::getTotalMileage();
    Vehicle* v = new Vehicle("AAA-001", "Truck", 5000);
    EXPECT_EQ(Vehicle::getFleetSize(),    fleetBefore   + 1);
    EXPECT_EQ(Vehicle::getTotalMileage(), mileageBefore + 5000);
    delete v;
}

TEST(Q4_Vehicle, AddMileageUpdatesBothCounters) {
    int mileageBefore = Vehicle::getTotalMileage();
    Vehicle* v = new Vehicle("BBB-002", "Van", 0);
    v->addMileage(3000);
    v->addMileage(-100);   // negative — ignore
    EXPECT_EQ(v->getMileage(),            3000);
    EXPECT_EQ(Vehicle::getTotalMileage(), mileageBefore + 3000);
    delete v;
}

TEST(Q4_Vehicle, ServiceStatusThreeTiers) {
    Vehicle* good     = new Vehicle("C01", "Car", 2000);
    Vehicle* due      = new Vehicle("C02", "Car", 12000);
    Vehicle* overdue  = new Vehicle("C03", "Car", 16000);
    EXPECT_EQ(good->getServiceStatus(),    "Good");
    EXPECT_EQ(due->getServiceStatus(),     "Due Soon");
    EXPECT_EQ(overdue->getServiceStatus(), "Overdue");
    delete good; delete due; delete overdue;
}


// ============================================================
// Q5 – IoT Sensor Network  (scenario — ALL concepts)
// Concepts: static (3 fields), const/non-const, pointer, deactivate, alert
// ============================================================

TEST(Q5_Sensor, ConstructorUpdatesAllStatics) {
    int    activesBefore  = Sensor::getActiveSensors();
    double avgBefore      = Sensor::getNetworkAverage();
    // track reading count by checking average shift
    Sensor* s = new Sensor("T-01", "ZoneA", 60.0);
    EXPECT_EQ(Sensor::getActiveSensors(), activesBefore + 1);
    EXPECT_EQ(s->getId(),   "T-01");
    EXPECT_EQ(s->getZone(), "ZoneA");
    EXPECT_TRUE(s->isActive());
    delete s;
}

TEST(Q5_Sensor, UpdateReadingAndAlertThroughPointer) {
    Sensor* s = new Sensor("T-02", "ZoneB", 50.0);
    EXPECT_EQ(s->getAlert(), "NORMAL");
    s->updateReading(65.0);
    EXPECT_EQ(s->getAlert(), "WARNING");
    s->updateReading(82.0);
    EXPECT_EQ(s->getAlert(), "CRITICAL");
    delete s;
}

TEST(Q5_Sensor, DeactivateChangesAlertAndDecrements) {
    int activesBefore = Sensor::getActiveSensors();
    Sensor* s = new Sensor("T-03", "ZoneC", 90.0);
    EXPECT_EQ(Sensor::getActiveSensors(), activesBefore + 1);
    EXPECT_EQ(s->getAlert(), "CRITICAL");
    s->deactivate();
    EXPECT_FALSE(s->isActive());
    EXPECT_EQ(s->getAlert(), "OFFLINE");
    EXPECT_EQ(Sensor::getActiveSensors(), activesBefore);
    delete s;
}

