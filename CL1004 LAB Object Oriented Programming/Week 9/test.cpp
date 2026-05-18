// ============================================================
// GTest File: C++ Operator Overloading  —  Lab 06
// 3 test cases per question  (30 total)
// ============================================================

#include <gtest/gtest.h>
#include <string>
#include <cmath>
#include <sstream>
#include "pch.h"
#include "Header.h"

// ============================================================
// STUDENT CODE — paste your class implementations below
// ============================================================

// Q1:  class Temperature { ... };
// Q2:  class ScoreList { ... };
// Q3:  class TextBuffer { ... };
// Q4:  class Inventory { ... };
// Q5: class Fraction { ... };



// ============================================================
// Q1 – Temperature
// Operators: < ,  > ,  <= ,  >= ,  == ,  + ,  <<
// ============================================================

TEST(Q1_Temperature, ComparisonOperators) {
    Temperature t1(36.6), t2(38.0), t3(36.6);
    EXPECT_TRUE(t1 < t2);
    EXPECT_TRUE(t2 > t1);
    EXPECT_TRUE(t1 <= t3);
    EXPECT_TRUE(t1 >= t3);
    EXPECT_FALSE(t1 > t2);
}

TEST(Q1_Temperature, AdditionAndConversion) {
    Temperature t1(20.0), t2(10.0);
    Temperature sum = t1 + t2;
    EXPECT_DOUBLE_EQ(sum.getCelsius(), 30.0);
    EXPECT_DOUBLE_EQ(sum.toFahrenheit(), 86.0);
}

TEST(Q1_Temperature, StreamOutput) {
    std::ostringstream oss;
    oss << Temperature(36.6);
    EXPECT_EQ(oss.str(), "36.6C");
}


// ============================================================
// Q2 – Fixed Score List  (scenario)
// Operators: [] (non-const & const),  + ,  == ,  <<
// ============================================================

TEST(Q2_ScoreList, SubscriptReadAndWrite) {
    ScoreList sl("Test1");
    sl[0] = 90;
    sl[1] = 85;
    sl[2] = 78;
    sl[3] = 92;
    sl[4] = 88;
    EXPECT_EQ(sl[0], 90);
    EXPECT_EQ(sl[4], 88);
    EXPECT_NEAR(sl.average(), 86.6, 0.01);
}

TEST(Q2_ScoreList, AdditionCombinesElementsAndLabels) {
    ScoreList a("Math"), b("Science");
    a[0] = 10; a[1] = 20; a[2] = 30; a[3] = 40; a[4] = 50;
    b[0] = 5; b[1] = 10; b[2] = 15; b[3] = 20; b[4] = 25;
    ScoreList c = a + b;
    EXPECT_EQ(c[0], 15);
    EXPECT_EQ(c[4], 75);
    EXPECT_EQ(c.getLabel(), "Math+Science");
}

TEST(Q2_ScoreList, EqualityAndStreamOutput) {
    ScoreList a("A"), b("B");
    for (int i = 0; i < 5; i++) { a[i] = i * 10; b[i] = i * 10; }
    EXPECT_TRUE(a == b);
    b[2] = 99;
    EXPECT_FALSE(a == b);
    std::ostringstream oss;
    ScoreList s("Quiz");
    s[0] = 10; s[1] = 20; s[2] = 30; s[3] = 40; s[4] = 50;
    oss << s;
    EXPECT_EQ(oss.str(), "Quiz: [10, 20, 30, 40, 50]");
}

// ============================================================
// Q3 – TextBuffer  (scenario)
// Operators: = (assignment),  += ,  == ,  <<
// ============================================================

TEST(Q3_TextBuffer, AssignmentOperatorWithSelfGuard) {
    TextBuffer a("Hello", 50);
    TextBuffer b("", 50);
    b = a;
    EXPECT_EQ(b.getContent(), "Hello");
    EXPECT_EQ(b.getVersion(), 1);     // version resets on assignment
    b = b;                            // self-assignment — should not crash
    EXPECT_EQ(b.getContent(), "Hello");
}

TEST(Q3_TextBuffer, AppendOperatorEnforcesCapacityAndVersions) {
    TextBuffer buf("Hi", 10);
    buf += " World";                  // "Hi World" = 8 chars — fits
    EXPECT_EQ(buf.getContent(), "Hi World");
    EXPECT_EQ(buf.getVersion(), 2);
    buf += "!!!TOOLONG";             // would exceed 10 — ignored
    EXPECT_EQ(buf.getContent(), "Hi World");
    EXPECT_EQ(buf.getVersion(), 2);  // version unchanged
}

TEST(Q3_TextBuffer, EqualityAndStreamOutput) {
    TextBuffer a("test", 20), b("test", 50);
    EXPECT_TRUE(a == b);             // content equal; capacity ignored
    TextBuffer c("other", 20);
    EXPECT_FALSE(a == c);
    std::ostringstream oss;
    oss << a;
    EXPECT_EQ(oss.str(), "Buffer(v1): 'test'");
}

// ============================================================
// Q4 – Inventory  (scenario)
// Operators: += ,  -= ,  + ,  == ,  <<
// ============================================================

TEST(Q4_Inventory, CompoundAddAndSubtract) {
    Inventory inv("Pen", 100, 5.0);
    inv += 50;
    EXPECT_EQ(inv.getQuantity(), 150);
    inv -= 30;
    EXPECT_EQ(inv.getQuantity(), 120);
    inv -= 200;              // more than available — ignored
    EXPECT_EQ(inv.getQuantity(), 120);
}

TEST(Q4_Inventory, BinaryAddMergesQuantity) {
    Inventory a("Pen", 60, 5.0);
    Inventory b("Pen", 40, 5.0);
    Inventory merged = a + b;
    EXPECT_EQ(merged.getItemName(), "Pen");
    EXPECT_EQ(merged.getQuantity(), 100);
    EXPECT_DOUBLE_EQ(merged.totalValue(), 500.0);
}

TEST(Q4_Inventory, EqualityAndStreamOutput) {
    Inventory a("Book", 30, 200.0), b("Book", 30, 100.0);
    EXPECT_TRUE(a == b);             // same name and quantity
    Inventory c("Book", 25, 200.0);
    EXPECT_FALSE(a == c);
    std::ostringstream oss;
    oss << Inventory("Pen", 10, 15.0);
    EXPECT_EQ(oss.str(), "Pen: 10 units @ Rs.15");
}


// ============================================================
// Q5 – Fraction  (scenario — ALL operators)
// Operators: + ,  - ,  * ,  += ,  == ,  != ,  < ,  <<
// ============================================================

TEST(Q5_Fraction, AddSubtractAndReduce) {
    Fraction a(1, 2), b(1, 3);
    Fraction sum = a + b;   // 1/2 + 1/3 = 5/6
    Fraction diff = a - b;   // 1/2 - 1/3 = 1/6
    EXPECT_EQ(sum.getNumerator(), 5);
    EXPECT_EQ(sum.getDenominator(), 6);
    EXPECT_EQ(diff.getNumerator(), 1);
    EXPECT_EQ(diff.getDenominator(), 6);
}

TEST(Q5_Fraction, MultiplyAndCompoundAdd) {
    Fraction a(2, 3), b(3, 4);
    Fraction product = a * b;   // 2/3 * 3/4 = 6/12 = 1/2
    EXPECT_EQ(product.getNumerator(), 1);
    EXPECT_EQ(product.getDenominator(), 2);
    Fraction c(1, 4);
    c += Fraction(1, 4);        // 1/4 + 1/4 = 2/4 = 1/2
    EXPECT_EQ(c.getNumerator(), 1);
    EXPECT_EQ(c.getDenominator(), 2);
}

TEST(Q5_Fraction, EqualityInequalityLessThanAndStream) {
    Fraction a(1, 2), b(2, 4), c(1, 3);
    EXPECT_TRUE(a == b);    // 1/2 == 2/4
    EXPECT_FALSE(a == c);
    EXPECT_TRUE(a != c);
    EXPECT_TRUE(c < a);     // 1/3 < 1/2
    EXPECT_FALSE(a < c);
    std::ostringstream oss;
    oss << Fraction(3, 4);
    EXPECT_EQ(oss.str(), "3/4");
}