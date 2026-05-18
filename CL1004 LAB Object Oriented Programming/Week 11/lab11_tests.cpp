// ============================================================
// GTest File: C++ Operator Overloading Part III  —  Lab 11
// Non-Member Functions, Symmetric Operators, Stream IO
// 3 test cases per question  (30 total)
// ============================================================

#include <gtest/gtest.h>
#include <string>
#include <cmath>
#include <sstream>
#include <iomanip>

// ============================================================
// STUDENT CODE — paste your class implementations below
// ============================================================

// Q1:  class Point2D { ... };
// Q2:  class Angle { ... };
// Q3:  class Weight { ... };
// Q4:  class Time24 { ... };
// Q5:  class Price { ... };
// Q6:  class Velocity3D { ... };
// Q7:  class Ratio { ... };
// Q8:  class Color { ... };
// Q9:  class Duration { ... };
// Q10: class ComplexNum { ... };


// ============================================================
// Q1 – Point2D
// Operators: + (member), * scalar (member), * scalar (non-member), <<
// ============================================================

TEST(Q1_Point2D, AdditionAndScalarMultiplication) {
    Point2D a(3.0, 4.0), b(1.0, 2.0);
    Point2D sum = a + b;
    EXPECT_DOUBLE_EQ(sum.getX(), 4.0);
    EXPECT_DOUBLE_EQ(sum.getY(), 6.0);

    Point2D scaled = a * 2.0;
    EXPECT_DOUBLE_EQ(scaled.getX(), 6.0);
    EXPECT_DOUBLE_EQ(scaled.getY(), 8.0);
}

TEST(Q1_Point2D, NonMemberScalarMultiplication) {
    Point2D p(5.0, 10.0);
    Point2D result = 3.0 * p;    // non-member operator*
    EXPECT_DOUBLE_EQ(result.getX(), 15.0);
    EXPECT_DOUBLE_EQ(result.getY(), 30.0);
}

TEST(Q1_Point2D, StreamOutput) {
    std::ostringstream oss;
    oss << Point2D(3.5, 7.2);
    EXPECT_EQ(oss.str(), "(3.5, 7.2)");
}


// ============================================================
// Q2 – Angle
// Operators: + (non-member), == (non-member), << , >>
// ============================================================

TEST(Q2_Angle, NonMemberAdditionAndEquality) {
    Angle a(45.0), b(30.0);
    Angle sum = a + b;
    EXPECT_DOUBLE_EQ(sum.getDegrees(), 75.0);

    Angle c(75.0);
    EXPECT_TRUE(sum == c);
}

TEST(Q2_Angle, RadiansConversion) {
    Angle a(180.0);
    EXPECT_NEAR(a.toRadians(), M_PI, 0.0001);
    Angle b(90.0);
    EXPECT_NEAR(b.toRadians(), M_PI / 2.0, 0.0001);
}

TEST(Q2_Angle, StreamInputAndOutput) {
    Angle a(0);
    std::istringstream in("60");
    in >> a;
    EXPECT_DOUBLE_EQ(a.getDegrees(), 60.0);

    std::ostringstream out;
    out << a;
    EXPECT_EQ(out.str(), "60°");
}


// ============================================================
// Q3 – Weight (scenario)
// Operators: + (non-member), - (non-member), << , >>
// ============================================================

TEST(Q3_Weight, NonMemberAdditionAndSubtraction) {
    Weight w1(70.5), w2(5.3);
    Weight sum = w1 + w2;
    EXPECT_NEAR(sum.getKilograms(), 75.8, 0.01);

    Weight diff = w1 - w2;
    EXPECT_NEAR(diff.getKilograms(), 65.2, 0.01);
}

TEST(Q3_Weight, PoundsConversion) {
    Weight w(10.0);
    EXPECT_NEAR(w.toPounds(), 22.0462, 0.01);
}

TEST(Q3_Weight, StreamInputAndOutput) {
    Weight w(0);
    std::istringstream in("68.5");
    in >> w;
    EXPECT_NEAR(w.getKilograms(), 68.5, 0.01);

    std::ostringstream out;
    out << w;
    EXPECT_EQ(out.str(), "68.5 kg");
}


// ============================================================
// Q4 – Time24
// Operators: < (non-member), == (non-member), << , >>
// ============================================================

TEST(Q4_Time24, NonMemberComparison) {
    Time24 t1(9, 30), t2(14, 15), t3(9, 30);
    EXPECT_TRUE(t1 < t2);
    EXPECT_FALSE(t2 < t1);
    EXPECT_TRUE(t1 == t3);
    EXPECT_FALSE(t1 == t2);
}

TEST(Q4_Time24, StreamOutput) {
    std::ostringstream oss1, oss2;
    oss1 << Time24(9, 5);
    oss2 << Time24(14, 30);
    EXPECT_EQ(oss1.str(), "09:05");
    EXPECT_EQ(oss2.str(), "14:30");
}

TEST(Q4_Time24, StreamInput) {
    Time24 t(0, 0);
    std::istringstream in("23:45");
    in >> t;
    EXPECT_EQ(t.getHour(), 23);
    EXPECT_EQ(t.getMinute(), 45);
}


// ============================================================
// Q5 – Price (scenario)
// Operators: + (non-member), * int (both directions), << , >>
// ============================================================

TEST(Q5_Price, NonMemberAddition) {
    Price p1(19.99, "USD"), p2(5.00, "USD");
    Price sum = p1 + p2;
    EXPECT_NEAR(sum.getAmount(), 24.99, 0.01);
    EXPECT_EQ(sum.getCurrency(), "USD");
}

TEST(Q5_Price, SymmetricScalarMultiplication) {
    Price p(10.50, "EUR");
    Price r1 = p * 3;
    Price r2 = 3 * p;
    EXPECT_NEAR(r1.getAmount(), 31.50, 0.01);
    EXPECT_NEAR(r2.getAmount(), 31.50, 0.01);
}

TEST(Q5_Price, StreamInputAndOutput) {
    Price p(0.0, "");
    std::istringstream in("GBP 45.75");
    in >> p;
    EXPECT_EQ(p.getCurrency(), "GBP");
    EXPECT_NEAR(p.getAmount(), 45.75, 0.01);

    std::ostringstream out;
    out << p;
    EXPECT_EQ(out.str(), "GBP 45.75");
}


// ============================================================
// Q6 – Velocity3D
// Operators: + (non-member), * scalar (both directions), << , >>
// ============================================================

TEST(Q6_Velocity3D, NonMemberAddition) {
    Velocity3D v1(1.0, 2.0, 3.0), v2(4.0, 5.0, 6.0);
    Velocity3D sum = v1 + v2;
    EXPECT_DOUBLE_EQ(sum.getVx(), 5.0);
    EXPECT_DOUBLE_EQ(sum.getVy(), 7.0);
    EXPECT_DOUBLE_EQ(sum.getVz(), 9.0);
}

TEST(Q6_Velocity3D, SymmetricScaling) {
    Velocity3D v(2.0, 3.0, 4.0);
    Velocity3D r1 = 2.0 * v;
    Velocity3D r2 = v * 2.0;
    EXPECT_DOUBLE_EQ(r1.getVx(), 4.0);
    EXPECT_DOUBLE_EQ(r2.getVy(), 6.0);
    EXPECT_NEAR(r1.magnitude(), r2.magnitude(), 0.0001);
}

TEST(Q6_Velocity3D, StreamInputOutputAndMagnitude) {
    Velocity3D v(0, 0, 0);
    std::istringstream in("3.0 4.0 0.0");
    in >> v;
    EXPECT_DOUBLE_EQ(v.getVx(), 3.0);
    EXPECT_DOUBLE_EQ(v.getVy(), 4.0);
    EXPECT_DOUBLE_EQ(v.magnitude(), 5.0);

    std::ostringstream out;
    out << v;
    EXPECT_EQ(out.str(), "[3, 4, 0]");
}


// ============================================================
// Q7 – Ratio (scenario)
// Operators: == , != , * int (both directions), << , >>
// ============================================================

TEST(Q7_Ratio, NonMemberEqualityAndInequality) {
    Ratio r1(3, 2), r2(6, 4), r3(2, 3);
    EXPECT_TRUE(r1 == r2);   // 3/2 == 6/4 (cross multiply: 3*4 == 2*6)
    EXPECT_FALSE(r1 == r3);
    EXPECT_TRUE(r1 != r3);
}

TEST(Q7_Ratio, SymmetricScaling) {
    Ratio r(2, 3);
    Ratio s1 = r * 5;
    Ratio s2 = 5 * r;
    EXPECT_EQ(s1.getPart1(), 10);
    EXPECT_EQ(s1.getPart2(), 15);
    EXPECT_EQ(s2.getPart1(), 10);
    EXPECT_EQ(s2.getPart2(), 15);
}

TEST(Q7_Ratio, StreamInputOutputAndDecimal) {
    Ratio r(0, 1);
    std::istringstream in("4:5");
    in >> r;
    EXPECT_EQ(r.getPart1(), 4);
    EXPECT_EQ(r.getPart2(), 5);
    EXPECT_NEAR(r.toDecimal(), 0.8, 0.0001);

    std::ostringstream out;
    out << r;
    EXPECT_EQ(out.str(), "4:5");
}


// ============================================================
// Q8 – Color
// Operators: + (non-member with clamping), == , << , >>
// ============================================================

TEST(Q8_Color, NonMemberAdditionWithClamping) {
    Color c1(200, 150, 100), c2(100, 150, 200);
    Color sum = c1 + c2;
    EXPECT_EQ(sum.getR(), 255);  // clamped
    EXPECT_EQ(sum.getG(), 255);  // clamped
    EXPECT_EQ(sum.getB(), 255);  // clamped
}

TEST(Q8_Color, NonMemberEquality) {
    Color c1(128, 64, 32), c2(128, 64, 32), c3(0, 0, 0);
    EXPECT_TRUE(c1 == c2);
    EXPECT_FALSE(c1 == c3);
}

TEST(Q8_Color, StreamInputOutputAndHex) {
    Color c(0, 0, 0);
    std::istringstream in("255 128 0");
    in >> c;
    EXPECT_EQ(c.getR(), 255);
    EXPECT_EQ(c.getG(), 128);
    EXPECT_EQ(c.getB(), 0);

    std::ostringstream out;
    out << c;
    EXPECT_EQ(out.str(), "RGB(255, 128, 0)");
    EXPECT_EQ(c.toHex(), "#FF8000");
}


// ============================================================
// Q9 – Duration (scenario)
// Operators: + , * double (both directions), < , == , << , >>
// ============================================================

TEST(Q9_Duration, NonMemberAdditionAndComparison) {
    Duration d1(3.5), d2(2.0), d3(3.5);
    Duration sum = d1 + d2;
    EXPECT_NEAR(sum.getHours(), 5.5, 0.01);

    EXPECT_TRUE(d2 < d1);
    EXPECT_FALSE(d1 < d2);
    EXPECT_TRUE(d1 == d3);
}

TEST(Q9_Duration, SymmetricScaling) {
    Duration d(4.0);
    Duration r1 = d * 1.5;
    Duration r2 = 1.5 * d;
    EXPECT_NEAR(r1.getHours(), 6.0, 0.01);
    EXPECT_NEAR(r2.getHours(), 6.0, 0.01);
}

TEST(Q9_Duration, StreamInputOutputAndMinutes) {
    Duration d(0);
    std::istringstream in("2.5");
    in >> d;
    EXPECT_NEAR(d.getHours(), 2.5, 0.01);
    EXPECT_NEAR(d.toMinutes(), 150.0, 0.1);

    std::ostringstream out;
    out << d;
    EXPECT_EQ(out.str(), "2.5h");
}


// ============================================================
// Q10 – ComplexNum (scenario — ALL Part III concepts)
// Operators: + , - , * (complex), * double (both), == , != , << , >>
// Plus: magnitude, conjugate
// ============================================================

TEST(Q10_ComplexNum, NonMemberArithmetic) {
    ComplexNum a(3.0, 4.0), b(1.0, 2.0);
    ComplexNum sum = a + b;
    EXPECT_DOUBLE_EQ(sum.getReal(), 4.0);
    EXPECT_DOUBLE_EQ(sum.getImag(), 6.0);

    ComplexNum diff = a - b;
    EXPECT_DOUBLE_EQ(diff.getReal(), 2.0);
    EXPECT_DOUBLE_EQ(diff.getImag(), 2.0);

    // (3+4i) * (1+2i) = 3 + 6i + 4i + 8i^2 = 3 + 10i - 8 = -5 + 10i
    ComplexNum product = a * b;
    EXPECT_DOUBLE_EQ(product.getReal(), -5.0);
    EXPECT_DOUBLE_EQ(product.getImag(), 10.0);
}

TEST(Q10_ComplexNum, SymmetricScalarMultiplicationAndEquality) {
    ComplexNum c(2.0, 3.0);
    ComplexNum r1 = c * 5.0;
    ComplexNum r2 = 5.0 * c;
    EXPECT_DOUBLE_EQ(r1.getReal(), 10.0);
    EXPECT_DOUBLE_EQ(r1.getImag(), 15.0);
    EXPECT_TRUE(r1 == r2);
    EXPECT_FALSE(r1 != r2);
}

TEST(Q10_ComplexNum, StreamIOMagnitudeConjugate) {
    ComplexNum c(0, 0);
    std::istringstream in("3.0 4.0");
    in >> c;
    EXPECT_DOUBLE_EQ(c.getReal(), 3.0);
    EXPECT_DOUBLE_EQ(c.getImag(), 4.0);
    EXPECT_DOUBLE_EQ(c.magnitude(), 5.0);

    ComplexNum conj = c.conjugate();
    EXPECT_DOUBLE_EQ(conj.getReal(), 3.0);
    EXPECT_DOUBLE_EQ(conj.getImag(), -4.0);

    std::ostringstream pos, neg;
    pos << ComplexNum(3.0, 4.0);
    neg << ComplexNum(3.0, -4.0);
    EXPECT_EQ(pos.str(), "3+4i");
    EXPECT_EQ(neg.str(), "3-4i");
}


// ============================================================
// Main
// ============================================================

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
