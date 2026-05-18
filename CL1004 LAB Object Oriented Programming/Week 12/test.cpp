// ============================================================
// GTest File: Composition and Aggregation  —  Lab 12
// Object Relationships, Lifetime Management, Has-A patterns
// 3 test cases per question  (30 total)
// ============================================================
#include "pch.h"
#include <gtest/gtest.h>
#include <string>
#include <cmath>
#include "Header.h"


using namespace std;

// ============================================================
// STUDENT CODE — paste your class implementations below
// ============================================================

// Q1:  class CPU { ... };
//      class RAM { ... };
//      class Computer { ... };
// Q2:  class Song { ... };
//      class Playlist { ... };
// Q3:  class Transaction { ... };
//      class BankAccount { ... };
// Q4:  class University { ... };
// Q5: class Device { ... };
//      class Room { ... };
//      class SmartHome { ... };




// ============================================================
// Q1 – Computer and Components (Composition)
// Computer is COMPOSED of CPU and RAM objects
// ============================================================

TEST(Q1_Computer, CompositionCreatesCPUAndRAM) {
    Computer comp("ThinkPad", "Intel", 3.2, 16);
    EXPECT_EQ(comp.getModel(), "ThinkPad");
    EXPECT_EQ(comp.getCPUBrand(), "Intel");
    EXPECT_DOUBLE_EQ(comp.getCPUSpeed(), 3.2);
    EXPECT_EQ(comp.getRAMSize(), 16);
}

TEST(Q1_Computer, DelegationToComponents) {
    Computer comp("MacBook", "Apple M1", 3.5, 32);
    EXPECT_EQ(comp.getCPUBrand(), "Apple M1");
    EXPECT_DOUBLE_EQ(comp.getCPUSpeed(), 3.5);
    EXPECT_EQ(comp.getRAMSize(), 32);
}

TEST(Q1_Computer, MultipleComputersSeparateComponents) {
    Computer comp1("Desktop", "AMD", 4.0, 64);
    Computer comp2("Laptop", "Intel", 2.8, 8);

    EXPECT_NE(comp1.getCPUBrand(), comp2.getCPUBrand());
    EXPECT_NE(comp1.getRAMSize(), comp2.getRAMSize());
}


// ============================================================
// Q2 – Playlist and Songs (Aggregation)
// Playlist AGGREGATES Song pointers
// ============================================================

TEST(Q2_Playlist, AddAndRetrieveSongs) {
    Song song1("Bohemian Rhapsody", "Queen", 354);
    Song song2("Hotel California", "Eagles", 391);
    Song song3("Imagine", "John Lennon", 183);

    Playlist playlist("Rock Classics");
    playlist.addSong(&song1);
    playlist.addSong(&song2);
    playlist.addSong(&song3);

    EXPECT_EQ(playlist.getSongCount(), 3);
    EXPECT_EQ(playlist.getSong(0)->getTitle(), "Bohemian Rhapsody");
    EXPECT_EQ(playlist.getSong(1)->getArtist(), "Eagles");
}

TEST(Q2_Playlist, CalculateTotalDuration) {
    Song song1("Song A", "Artist A", 200);
    Song song2("Song B", "Artist B", 180);
    Song song3("Song C", "Artist C", 220);

    Playlist playlist("My Mix");
    playlist.addSong(&song1);
    playlist.addSong(&song2);
    playlist.addSong(&song3);

    EXPECT_EQ(playlist.getTotalDuration(), 600);
}

TEST(Q2_Playlist, GetSongReturnsNullptrForInvalidIndex) {
    Playlist playlist("Empty");
    EXPECT_EQ(playlist.getSong(0), nullptr);
    EXPECT_EQ(playlist.getSong(-1), nullptr);
}


// ============================================================
// Q3 – BankAccount with Transaction History (Composition)
// BankAccount is COMPOSED of Transaction objects array
// ============================================================

TEST(Q3_BankAccount, DepositCreatesTransaction) {
    BankAccount account("ACC123", 1000.0);
    account.deposit(500.0);

    EXPECT_DOUBLE_EQ(account.getBalance(), 1500.0);
    EXPECT_EQ(account.getTransactionCount(), 1);

    Transaction t = account.getTransaction(0);
    EXPECT_EQ(t.getType(), "deposit");
    EXPECT_DOUBLE_EQ(t.getAmount(), 500.0);
}

TEST(Q3_BankAccount, WithdrawCreatesTransaction) {
    BankAccount account("ACC456", 2000.0);
    account.withdraw(300.0);

    EXPECT_DOUBLE_EQ(account.getBalance(), 1700.0);
    EXPECT_EQ(account.getTransactionCount(), 1);

    Transaction t = account.getTransaction(0);
    EXPECT_EQ(t.getType(), "withdraw");
    EXPECT_DOUBLE_EQ(t.getAmount(), 300.0);
}

TEST(Q3_BankAccount, MultipleTransactionsTracked) {
    BankAccount account("ACC789", 1000.0);
    account.deposit(200.0);    // 1200
    account.withdraw(100.0);   // 1100
    account.deposit(300.0);    // 1400

    EXPECT_DOUBLE_EQ(account.getBalance(), 1400.0);
    EXPECT_EQ(account.getTransactionCount(), 3);
    EXPECT_EQ(account.getTransaction(0).getType(), "deposit");
    EXPECT_EQ(account.getTransaction(1).getType(), "withdraw");
    EXPECT_EQ(account.getTransaction(2).getType(), "deposit");
}


// ============================================================
// Q4 – University and Departments (Aggregation)
// University AGGREGATES Department pointers
// ============================================================

TEST(Q4_University, AddAndRetrieveDepartments) {
    Employee e1("Alice", 1, 50000);
    Employee e2("Bob", 2, 60000);

    Department dept1("CS");
    Department dept2("Math");
    dept1.addEmployee(&e1);
    dept2.addEmployee(&e2);

    University uni("MIT");
    uni.addDepartment(&dept1);
    uni.addDepartment(&dept2);

    EXPECT_EQ(uni.getDeptCount(), 2);
    EXPECT_EQ(uni.getDepartment(0)->getDeptName(), "CS");
    EXPECT_EQ(uni.getDepartment(1)->getDeptName(), "Math");
}

TEST(Q4_University, CalculateTotalEmployees) {
    Employee e1("Alice", 1, 50000), e2("Bob", 2, 60000);
    Employee e3("Charlie", 3, 55000), e4("Diana", 4, 65000);

    Department dept1("Engineering");
    Department dept2("Business");
    dept1.addEmployee(&e1);
    dept1.addEmployee(&e2);
    dept2.addEmployee(&e3);
    dept2.addEmployee(&e4);

    University uni("Harvard");
    uni.addDepartment(&dept1);
    uni.addDepartment(&dept2);

    EXPECT_EQ(uni.getTotalEmployees(), 4);
}

TEST(Q4_University, GetDepartmentReturnsNullptrForInvalidIndex) {
    University uni("Stanford");
    EXPECT_EQ(uni.getDepartment(0), nullptr);
    EXPECT_EQ(uni.getDepartment(-1), nullptr);
}


// ============================================================
// Q5 – SmartHome System (Mixed Composition & Aggregation)
// SmartHome COMPOSED of Rooms, Rooms AGGREGATE Devices
// ============================================================

TEST(Q5_SmartHome, CompositionCreatesRooms) {
    SmartHome home("My House");
    EXPECT_EQ(home.getHomeName(), "My House");
    EXPECT_EQ(home.getLivingRoom().getRoomName(), "Living Room");
    EXPECT_EQ(home.getBedroom().getRoomName(), "Bedroom");
}

TEST(Q5_SmartHome, AddDevicesToRoomsAndCalculatePower) {
    Device tv("TV", 100);
    Device lamp("Lamp", 60);
    Device ac("AC", 1500);

    SmartHome home("Smart Villa");
    home.getLivingRoom().addDevice(&tv);
    home.getLivingRoom().addDevice(&lamp);
    home.getBedroom().addDevice(&ac);

    EXPECT_EQ(home.getLivingRoom().getDeviceCount(), 2);
    EXPECT_EQ(home.getBedroom().getDeviceCount(), 1);

    // Turn on devices
    tv.turnOn();
    lamp.turnOn();
    ac.turnOn();

    EXPECT_EQ(home.getLivingRoom().getTotalPower(), 160);
    EXPECT_EQ(home.getBedroom().getTotalPower(), 1500);
    EXPECT_EQ(home.getTotalHomePower(), 1660);
}

TEST(Q5_SmartHome, OnlyOnDevicesCountTowardPower) {
    Device light1("Light1", 40);
    Device light2("Light2", 40);
    Device fan("Fan", 75);

    SmartHome home("Test Home");
    home.getLivingRoom().addDevice(&light1);
    home.getLivingRoom().addDevice(&light2);
    home.getBedroom().addDevice(&fan);

    // Only turn on some devices
    light1.turnOn();
    // light2 stays off
    fan.turnOn();

    EXPECT_EQ(home.getLivingRoom().getTotalPower(), 40);   // Only light1
    EXPECT_EQ(home.getBedroom().getTotalPower(), 75);       // Fan
    EXPECT_EQ(home.getTotalHomePower(), 115);
}

