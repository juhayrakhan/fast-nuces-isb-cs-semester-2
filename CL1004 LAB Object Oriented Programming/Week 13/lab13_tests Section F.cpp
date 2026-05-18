// ============================================================
// GTest File: Inheritance (REVISED - Higher Difficulty)  —  Lab 13
// Manual implementations, complex hierarchies, helper functions
// 3 test cases per question  (30 total)
// ============================================================

#include <gtest/gtest.h>
#include <cstring>

using namespace std;

// ============================================================
// STUDENT CODE — paste your class implementations below
// ============================================================


// Q1:  class Book { ... };
//      class EBook { ... };
//      class AudioBook { ... };
// Q2:  class Product { ... };
//      class Electronics { ... };
//      class Computer { ... };
// Q3:  class Person { ... };
//      class Student { ... };
//      class GraduateStudent { ... };
// Q4:  class Patient { ... };
//      class InPatient { ... };
//      class ICUPatient { ... };
// Q5: class Item { ... };
//      class Product { ... };
//      class DigitalProduct { ... };
//      class SoftwareLicense { ... };


// ============================================================
// Q1 – Book Catalog with Manual String Comparison
// Tests 3-level hierarchy with string matching
// ============================================================

TEST(Q1_BookCatalog, AuthorMatching) {
    AudioBook book("1984", "George Orwell", 328, 2.5, "MP3", 660, "Simon Prebble");
    
    EXPECT_TRUE(book.matchesAuthor("George Orwell"));
    EXPECT_FALSE(book.matchesAuthor("J.K. Rowling"));
}

TEST(Q1_BookCatalog, FormatMatching) {
    AudioBook book("Dune", "Frank Herbert", 688, 5.2, "AAC", 1260, "Scott Brick");
    
    EXPECT_TRUE(book.matchesFormat("AAC"));
    EXPECT_FALSE(book.matchesFormat("MP3"));
}

TEST(Q1_BookCatalog, NarratorMatching) {
    AudioBook book("Foundation", "Isaac Asimov", 255, 1.8, "MP3", 520, "Scott Brick");
    
    EXPECT_TRUE(book.matchesNarrator("Scott Brick"));
    EXPECT_FALSE(book.matchesNarrator("Stephen Fry"));
}


// ============================================================
// Q2 – Product with Manual Price Sorting
// Tests protected helper and manual bubble sort
// ============================================================

TEST(Q2_ProductSorting, PriceSortingLowest) {
    Computer comp("Gaming PC", 24, "RTX4090, i9", 32);
    comp.addPrice(1500.0);
    comp.addPrice(1200.0);
    comp.addPrice(1800.0);
    comp.addPrice(1100.0);
    
    EXPECT_NEAR(comp.getLowestPrice(), 1100.0, 0.01);
}

TEST(Q2_ProductSorting, PriceSortingHighest) {
    Computer comp("Workstation", 36, "Quadro, Xeon", 64);
    comp.addPrice(2000.0);
    comp.addPrice(2500.0);
    comp.addPrice(1800.0);
    comp.addPrice(2200.0);
    
    EXPECT_NEAR(comp.getHighestPrice(), 2500.0, 0.01);
}

TEST(Q2_ProductSorting, SortedOrder) {
    Electronics elec("Laptop", 12);
    elec.addPrice(999.0);
    elec.addPrice(799.0);
    elec.addPrice(1199.0);
    
    double lowest = elec.getLowestPrice();
    EXPECT_NEAR(lowest, 799.0, 0.01);
    
    // After sorting, prices should be in order
    EXPECT_NEAR(elec.getPrice(0), 799.0, 0.01);
    EXPECT_NEAR(elec.getPrice(1), 999.0, 0.01);
    EXPECT_NEAR(elec.getPrice(2), 1199.0, 0.01);
}


// ============================================================
// Q3 – University with Manual GPA Calculation
// Tests manual averaging from array
// ============================================================

TEST(Q3_University, GPACalculation) {
    GraduateStudent gs("Alice", 25, "alice@uni.edu", "G12345", "ML Thesis", "Dr. Smith", 4.0);
    gs.addGrade(3.7);
    gs.addGrade(3.9);
    gs.addGrade(4.0);
    gs.addGrade(3.8);
    
    double expectedGPA = (3.7 + 3.9 + 4.0 + 3.8) / 4.0;  // 3.85
    EXPECT_NEAR(gs.getGPA(), expectedGPA, 0.01);
}

TEST(Q3_University, FinalGPAWithResearch) {
    GraduateStudent gs("Bob", 26, "bob@uni.edu", "G67890", "AI Research", "Dr. Johnson", 4.0);
    gs.addGrade(3.5);
    gs.addGrade(3.7);
    
    double courseGPA = (3.5 + 3.7) / 2.0;  // 3.6
    double finalGPA = (courseGPA * 0.7) + (4.0 * 0.3);  // 2.52 + 1.2 = 3.72
    EXPECT_NEAR(gs.getFinalGPA(), finalGPA, 0.01);
}

TEST(Q3_University, MultipleGrades) {
    GraduateStudent gs("Charlie", 24, "c@uni.edu", "G111", "Thesis", "Dr. Lee", 3.8);
    gs.addGrade(4.0);
    gs.addGrade(3.9);
    gs.addGrade(3.8);
    gs.addGrade(3.7);
    gs.addGrade(4.0);
    
    double avgGrades = (4.0 + 3.9 + 3.8 + 3.7 + 4.0) / 5.0;
    double final = (avgGrades * 0.7) + (3.8 * 0.3);
    EXPECT_NEAR(gs.getFinalGPA(), final, 0.01);
}


// ============================================================
// Q4 – Hospital with Visit Tracking
// Tests manual array searching
// ============================================================

TEST(Q4_Hospital, VisitTracking) {
    ICUPatient patient("John Doe", 55, "P12345", "ICU-101", 20250101, "Critical");
    patient.addVisit(20250101);
    patient.addVisit(20250115);
    patient.addVisit(20250210);
    
    EXPECT_EQ(patient.getVisitCount(), 3);
}

TEST(Q4_Hospital, DaysStayedCalculation) {
    ICUPatient patient("Jane Smith", 62, "P67890", "ICU-202", 20250101, "Stable");
    patient.updateStay(20250110);
    
    EXPECT_EQ(patient.getDaysStayed(), 9);  // 20250110 - 20250101
}

TEST(Q4_Hospital, CriticalityScore) {
    ICUPatient patient("Bob Wilson", 48, "P111", "ICU-303", 20250101, "Severe");
    patient.addVitalSign(80);
    patient.addVitalSign(90);
    patient.addVitalSign(85);
    patient.addVitalSign(75);
    
    patient.calculateCriticality();
    double avg = (80 + 90 + 85 + 75) / 4.0;  // 82.5
    EXPECT_NEAR(patient.getCriticalityScore(), avg / 10.0, 0.01);  // 8.25
}


// ============================================================
// Q5 – E-Commerce Platform (Comprehensive 4-Level)
// Tests complex hierarchy with pricing logic
// ============================================================

TEST(Q5_ECommerce, ItemBasicFunctionality) {
    SoftwareLicense license("SW001", "Office Suite", 299.99, "Software", 50.0, "ZIP", 5, 
                           "XXXX-YYYY-ZZZZ", 365, "Professional");
    
    EXPECT_STREQ(license.getItemId(), "SW001");
    EXPECT_STREQ(license.getName(), "Office Suite");
    EXPECT_NEAR(license.getBasePrice(), 299.99, 0.01);
}

TEST(Q5_ECommerce, DigitalProductNoShipping) {
    SoftwareLicense license("SW002", "Antivirus", 49.99, "Software", 25.0, "EXE", 10,
                           "AAAA-BBBB-CCCC", 365, "Personal");
    
    // Digital products should not include shipping in total price
    double expected = 49.99 * 1.05;  // Only tax, no shipping
    EXPECT_NEAR(license.getTotalPrice(), expected, 0.01);
}

TEST(Q5_ECommerce, EnterpriseLicensePremium) {
    SoftwareLicense enterprise("SW003", "Enterprise Suite", 999.99, "Software", 100.0, "ISO", 3,
                               "EEEE-FFFF-GGGG", 730, "Enterprise");
    SoftwareLicense personal("SW004", "Personal Suite", 999.99, "Software", 100.0, "ISO", 3,
                            "HHHH-IIII-JJJJ", 730, "Personal");
    
    enterprise.activateLicense();
    personal.activateLicense();
    
    // Enterprise should be 1.5x the total price
    double baseTaxed = 999.99 * 1.05;
    EXPECT_NEAR(enterprise.getFinalPrice(), baseTaxed * 1.5, 0.01);
    EXPECT_NEAR(personal.getFinalPrice(), baseTaxed, 0.01);
}

