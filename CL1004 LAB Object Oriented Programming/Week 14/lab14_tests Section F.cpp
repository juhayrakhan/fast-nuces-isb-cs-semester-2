// ============================================================
// GTest File: Polymorphism (REVISED - Higher Difficulty)  —  Lab 14
// Multiple pure virtual functions, manual array processing
// 3 test cases per question  (30 total)
// ============================================================

#include <gtest/gtest.h>
#include <string>
#include <cmath>

using namespace std;

// ============================================================
// STUDENT CODE — paste your class implementations below
// ============================================================

// Q1:  class Transport { ... };
//      class Bus { ... };
//      class Taxi { ... };
// Q2:  class Notification { ... };
//      class EmailNotification { ... };
//      class SMSNotification { ... };
//      class PushNotification { ... };
//      class SlackNotification { ... };
// Q3:  class Discount { ... };
//      class PercentageDiscount { ... };
//      class FixedDiscount { ... };
//      class BuyOneGetOne { ... };
// Q4:  class Character { ... };
//      class Warrior { ... };
//      class Mage { ... };
//      class Archer { ... };
//      class Healer { ... };
// Q5: class Payment { ... };
//      class CreditCard { ... };
//      class DebitCard { ... };
//      class PayPal { ... };
//      class Cryptocurrency { ... };



// ============================================================
// Q1 – Transportation with Cost Calculation
// Tests distance-based pricing
// ============================================================

TEST(Q1_Transport, BusCostCalculation) {
    Bus bus("City Bus", 50, 0.5);
    Transport* ptr = &bus;
    
    EXPECT_DOUBLE_EQ(ptr->calculateCost(10.0), 5.0);  // 10 * 0.5
    EXPECT_EQ(ptr->getType(), "Bus");
    EXPECT_TRUE(ptr->canCarry(40));
    EXPECT_FALSE(ptr->canCarry(60));
}

TEST(Q1_Transport, TaxiCostCalculation) {
    Taxi taxi("Yellow Cab", 4, 3.0, 1.2);
    Transport* ptr = &taxi;
    
    EXPECT_DOUBLE_EQ(ptr->calculateCost(10.0), 15.0);  // 3.0 + (10 * 1.2)
    EXPECT_EQ(ptr->getType(), "Taxi");
    EXPECT_TRUE(ptr->canCarry(4));
    EXPECT_FALSE(ptr->canCarry(5));
}

TEST(Q1_Transport, PolymorphicFleet) {
    Bus b("Express", 60, 0.8);
    Taxi t("Uber", 3, 2.5, 1.5);
    
    Transport* fleet[2];
    fleet[0] = &b;
    fleet[1] = &t;
    
    EXPECT_DOUBLE_EQ(fleet[0]->calculateCost(20.0), 16.0);  // 20 * 0.8
    EXPECT_DOUBLE_EQ(fleet[1]->calculateCost(20.0), 32.5);  // 2.5 + 30
}


// ============================================================
// Q2 – Notification System with Four Types
// Tests priority system with multiple channels
// ============================================================

TEST(Q2_Notification, EmailWithPriority) {
    EmailNotification email("Meeting at 3pm", 5, "user@example.com");
    Notification* ptr = &email;
    
    EXPECT_EQ(ptr->getChannel(), "Email");
    EXPECT_TRUE(ptr->isUrgent());  // priority 5 >= 4
    EXPECT_EQ(ptr->send(), "Email sent to user@example.com: Meeting at 3pm");
}

TEST(Q2_Notification, SMSNotUrgent) {
    SMSNotification sms("Reminder", 2, "+1234567890");
    Notification* ptr = &sms;
    
    EXPECT_EQ(ptr->getChannel(), "SMS");
    EXPECT_FALSE(ptr->isUrgent());  // priority 2 < 4
}

TEST(Q2_Notification, PolymorphicNotifications) {
    EmailNotification e("Alert!", 4, "admin@site.com");
    SMSNotification s("Code: 123", 3, "+9876543210");
    PushNotification p("Update", 5, "device-abc");
    SlackNotification sl("Deploy complete", 4, "#engineering");
    
    Notification* notifications[4];
    notifications[0] = &e;
    notifications[1] = &s;
    notifications[2] = &p;
    notifications[3] = &sl;
    
    EXPECT_TRUE(notifications[0]->isUrgent());
    EXPECT_FALSE(notifications[1]->isUrgent());
    EXPECT_TRUE(notifications[2]->isUrgent());
    EXPECT_TRUE(notifications[3]->isUrgent());
}


// ============================================================
// Q3 – Discount Strategy with Savings Calculation
// Tests polymorphic savings comparison
// ============================================================

TEST(Q3_Discount, PercentageSavings) {
    PercentageDiscount discount("SAVE20", 20.0);
    Discount* ptr = &discount;
    
    EXPECT_NEAR(ptr->apply(100.0), 80.0, 0.01);
    EXPECT_NEAR(ptr->getSavings(100.0), 20.0, 0.01);
    EXPECT_EQ(ptr->getDescription(), "20% off");
}

TEST(Q3_Discount, FixedSavings) {
    FixedDiscount discount("OFF15", 15.0);
    Discount* ptr = &discount;
    
    EXPECT_DOUBLE_EQ(ptr->apply(100.0), 85.0);
    EXPECT_DOUBLE_EQ(ptr->getSavings(100.0), 15.0);
    EXPECT_DOUBLE_EQ(ptr->getSavings(10.0), 10.0);  // can't save more than price
}

TEST(Q3_Discount, PolymorphicDiscounts) {
    PercentageDiscount p("PERCENT25", 25.0);
    FixedDiscount f("FIXED10", 10.0);
    BuyOneGetOne bogo("BOGO");
    
    Discount* discounts[3];
    discounts[0] = &p;
    discounts[1] = &f;
    discounts[2] = &bogo;
    
    EXPECT_NEAR(discounts[0]->getSavings(200.0), 50.0, 0.01);  // 25% of 200
    EXPECT_DOUBLE_EQ(discounts[1]->getSavings(200.0), 10.0);
    EXPECT_NEAR(discounts[2]->getSavings(200.0), 100.0, 0.01);  // 50% of 200
}


// ============================================================
// Q4 – Game Characters with Combat Stats
// Tests four character types with different stat calculations
// ============================================================

TEST(Q4_Character, WarriorStats) {
    Warrior warrior("Conan", 100, 8);
    Character* ptr = &warrior;
    
    EXPECT_EQ(ptr->attack(), 40);  // 8 * 5
    EXPECT_EQ(ptr->defend(), 24);  // 8 * 3
    EXPECT_EQ(ptr->getClass(), "Warrior");
    EXPECT_FALSE(ptr->isElite());  // level 8 < 10
}

TEST(Q4_Character, MageElite) {
    Mage mage("Gandalf", 80, 12);
    Character* ptr = &mage;
    
    EXPECT_EQ(ptr->attack(), 84);  // 12 * 7
    EXPECT_EQ(ptr->defend(), 12);  // 12 * 1
    EXPECT_TRUE(ptr->isElite());  // level 12 >= 10
}

TEST(Q4_Character, PolymorphicParty) {
    Warrior w("Aragorn", 120, 10);
    Mage m("Merlin", 70, 15);
    Archer a("Legolas", 90, 9);
    Healer h("Mercy", 85, 11);
    
    Character* party[4];
    party[0] = &w;
    party[1] = &m;
    party[2] = &a;
    party[3] = &h;
    
    EXPECT_EQ(party[0]->attack(), 50);   // 10 * 5
    EXPECT_EQ(party[1]->attack(), 105);  // 15 * 7
    EXPECT_EQ(party[2]->attack(), 54);   // 9 * 6
    EXPECT_EQ(party[3]->attack(), 22);   // 11 * 2
    
    EXPECT_TRUE(party[0]->isElite());
    EXPECT_TRUE(party[1]->isElite());
    EXPECT_FALSE(party[2]->isElite());
    EXPECT_TRUE(party[3]->isElite());
}


// ============================================================
// Q5 – Payment Processing with Complex Fee Structure
// Tests international fees and total cost calculation
// ============================================================

TEST(Q5_Payment, CreditCardDomestic) {
    CreditCard cc(100.0, "TXN001", "4532-1234", false);
    Payment* ptr = &cc;
    
    double fee = 100.0 * 0.029;  // 2.9
    EXPECT_NEAR(ptr->getFee(), fee, 0.01);
    EXPECT_DOUBLE_EQ(ptr->getConversionFee(), 0.0);
    EXPECT_NEAR(ptr->getTotalCost(), 100.0 + fee, 0.01);
}

TEST(Q5_Payment, PayPalInternational) {
    PayPal pp(200.0, "TXN002", "user@paypal.com", true);
    Payment* ptr = &pp;
    
    double fee = 200.0 * 0.034;  // 6.8
    double conversionFee = 2.5;
    EXPECT_NEAR(ptr->getFee(), fee, 0.01);
    EXPECT_DOUBLE_EQ(ptr->getConversionFee(), conversionFee);
    EXPECT_NEAR(ptr->getTotalCost(), 200.0 + fee + conversionFee, 0.01);
}

TEST(Q5_Payment, PolymorphicPaymentProcessing) {
    CreditCard cc(150.0, "T1", "1234-5678", true);
    DebitCard dc(150.0, "T2", "9876-5432", false);
    PayPal pp(150.0, "T3", "pay@pal.com", true);
    Cryptocurrency crypto(150.0, "T4", "0x1234abcd", false);
    
    Payment* payments[4];
    payments[0] = &cc;
    payments[1] = &dc;
    payments[2] = &pp;
    payments[3] = &crypto;
    
    // CreditCard: 150 * 0.029 + 150 * 0.03 = 4.35 + 4.5 = 8.85
    EXPECT_NEAR(payments[0]->getTotalCost(), 150.0 + 8.85, 0.1);
    
    // DebitCard: 1.5 + 0 = 1.5
    EXPECT_NEAR(payments[1]->getTotalCost(), 150.0 + 1.5, 0.1);
    
    // PayPal: 150 * 0.034 + 2.5 = 5.1 + 2.5 = 7.6
    EXPECT_NEAR(payments[2]->getTotalCost(), 150.0 + 7.6, 0.1);
    
    // Crypto: 1.0 + 0 = 1.0
    EXPECT_NEAR(payments[3]->getTotalCost(), 150.0 + 1.0, 0.1);
}

