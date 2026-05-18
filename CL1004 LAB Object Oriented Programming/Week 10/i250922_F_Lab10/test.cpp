#include "pch.h"
// ============================================================
// GTest File: C++ Operator Overloading Part II  —  Lab 07
// New operators: unary -, /, *=, /=, >>, (), !, type cast, &, |, ^
// 3 test cases per question  (30 total)
// ============================================================

#include <gtest/gtest.h>
#include <string>
#include <cmath>
#include <sstream>
#include "Header.h"



// ============================================================
// Q1 – Tax Calculator Functor
// Operators: () three overloads
// ============================================================

TEST(Q1_TaxCalc, SingleAmountTax) {
    TaxCalc gst(17.0);
    EXPECT_DOUBLE_EQ(gst(100.0), 17.0);
    EXPECT_DOUBLE_EQ(gst(200.0), 34.0);
}

TEST(Q1_TaxCalc, AmountAndQuantityTax) {
    TaxCalc gst(10.0);
    EXPECT_DOUBLE_EQ(gst(50.0, 4), 20.0);   // 50*4 * 10% = 20
}

TEST(Q1_TaxCalc, AmountWithDiscountThenTax) {
    TaxCalc gst(20.0);
    // 1000 with 25% discount = 750, then 20% tax = 150
    EXPECT_DOUBLE_EQ(gst(1000.0, 25.0), 150.0);
}


// ============================================================
// Q2 – Permission Flags  (scenario)
// Operators: | ,  & ,  ^ ,  ! ,  <<
// ============================================================

TEST(Q2_Permission, BitwiseOrGrantsBoth) {
    Permission rw = Permission(Permission::READ) | Permission(Permission::WRITE);
    EXPECT_TRUE(rw.has(Permission::READ));
    EXPECT_TRUE(rw.has(Permission::WRITE));
    EXPECT_FALSE(rw.has(Permission::EXECUTE));
}

TEST(Q2_Permission, BitwiseAndFindsCommon) {
    Permission a(Permission::READ | Permission::WRITE);    // 0b011 = 3
    Permission b(Permission::WRITE | Permission::EXECUTE); // 0b110 = 6
    Permission common = a & b;
    EXPECT_TRUE(common.has(Permission::WRITE));
    EXPECT_FALSE(common.has(Permission::READ));
    EXPECT_FALSE(common.has(Permission::EXECUTE));
}

TEST(Q2_Permission, XorTogglesAndNotDetectsEmpty) {
    Permission rw(Permission::READ | Permission::WRITE);
    Permission w(Permission::WRITE);
    Permission toggled = rw ^ w;             // READ only remains
    EXPECT_TRUE(toggled.has(Permission::READ));
    EXPECT_FALSE(toggled.has(Permission::WRITE));
    Permission none(0);
    EXPECT_TRUE(!none);
    std::ostringstream oss;
    oss << none;
    EXPECT_EQ(oss.str(), "[None]");
}


// ============================================================
// Q3 – Currency
// Operators: unary - ,  / ,  *= ,  /= ,  operator double ,  << ,  >>
// ============================================================

TEST(Q3_Currency, UnaryMinusAndDivision) {
    Currency c(1500.0, "PKR");
    Currency neg = -c;
    EXPECT_DOUBLE_EQ(neg.getAmount(), -1500.0);
    EXPECT_EQ(neg.getCode(), "PKR");

    Currency halved = c / 2.0;
    EXPECT_DOUBLE_EQ(halved.getAmount(), 750.0);
}

TEST(Q3_Currency, CompoundMulDivAndDoubleCast) {
    Currency c(200.0, "USD");
    c *= 3.0;    // 600
    c /= 4.0;    // 150
    EXPECT_DOUBLE_EQ(c.getAmount(), 150.0);
    double raw = static_cast<double>(c);
    EXPECT_DOUBLE_EQ(raw, 150.0);
}

TEST(Q3_Currency, StreamInputAndOutput) {
    Currency c("",0);
    std::istringstream in("EUR 89.99");
    in >> c;
    EXPECT_EQ(c.getCode(), "EUR");
    EXPECT_DOUBLE_EQ(c.getAmount(), 89.99);
    std::ostringstream out;
    out << c;
    EXPECT_EQ(out.str(), "EUR 89.99");
}

// ============================================================
// Q4 – Signal & Amplifier  (scenario)
// Operators: unary - ,  *= ,  /= ,  == ,  <<  on Signal
//            () functor on Amplifier
// ============================================================

TEST(Q4_Signal, UnaryMinusFlipsAmplitude) {
    Signal s(2.0, 440.0);
    Signal flipped = -s;
    EXPECT_DOUBLE_EQ(flipped.getAmplitude(), -2.0);
    EXPECT_DOUBLE_EQ(flipped.getFrequency(), 440.0);
}

TEST(Q4_Signal, CompoundScalingAndEquality) {
    Signal s(4.0, 880.0);
    s *= 2.0;   // amplitude 8
    s /= 4.0;   // amplitude 2
    Signal expected(2.0, 880.0);
    EXPECT_TRUE(s == expected);
}

TEST(Q4_Amplifier, FunctorAppliesGainToSignal) {
    Signal s(1.0, 440.0);
    Amplifier amp(5.0);
    Signal boosted = amp(s);
    EXPECT_DOUBLE_EQ(boosted.getAmplitude(), 5.0);
    EXPECT_DOUBLE_EQ(boosted.getFrequency(), 440.0);
    // original unchanged
    EXPECT_DOUBLE_EQ(s.getAmplitude(), 1.0);
}


// ============================================================
// Q5 – UnitVec  (scenario — ALL Part II operators)
// ============================================================

TEST(Q5_UnitVec, UnaryMinusDivisionAndValidity) {
    UnitVec v(3.0, 4.0);
    EXPECT_TRUE(v.isValid());

    UnitVec neg = -v;
    EXPECT_DOUBLE_EQ(neg.getX(), -3.0);
    EXPECT_DOUBLE_EQ(neg.getY(), -4.0);
    EXPECT_TRUE(neg.isValid());

    UnitVec halved = v / 2.0;
    EXPECT_DOUBLE_EQ(halved.getX(), 1.5);

    // division by zero produces invalid vector
    UnitVec bad = v / 0.0;
    EXPECT_TRUE(!bad);
}

TEST(Q5_UnitVec, TypeCastAndRotation) {
    UnitVec v(3.0, 4.0);
    double mag = static_cast<double>(v);
    EXPECT_NEAR(mag, 5.0, 0.0001);

    // rotate 90 degrees (pi/2): (x,y) -> (-y, x)
    UnitVec rotated = v(3.141592 / 2.0);
    EXPECT_NEAR(rotated.getX(), -4.0, 0.0001);
    EXPECT_NEAR(rotated.getY(), 3.0, 0.0001);
}

TEST(Q5_UnitVec, StreamIOAndCompoundOps) {
    UnitVec v;
    std::istringstream in("3.0 4.0");
    in >> v;
    EXPECT_TRUE(v.isValid());

    v *= 2.0;   // (6, 8)
    v /= 4.0;   // (1.5, 2)

    UnitVec expected(1.5, 2.0);
    EXPECT_TRUE(v == expected);

    std::ostringstream oss;
    oss << expected;
    EXPECT_EQ(oss.str(), "(1.5, 2) [valid]");
}