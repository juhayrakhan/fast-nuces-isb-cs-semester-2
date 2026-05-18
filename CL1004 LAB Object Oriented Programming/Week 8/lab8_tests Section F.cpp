// ============================================================
// GTest File: C++ Classes III
// Topics: this pointer, copy constructor, member initializer list
// 3 test cases per question  (30 total)
// ============================================================

#include <gtest/gtest.h>
#include <string>
#include <cmath>


// ============================================================
// Q1 – Email Builder
// Concepts: this pointer (*this chaining), member initializer list
// ============================================================

TEST(Q1_EmailBuilder, IncompleteEmailReturnsError) {
    EmailBuilder eb("sender@mail.com");
    EXPECT_EQ(eb.build(), "Error: incomplete email");   // no recipient or subject
    eb.to("recv@mail.com");
    EXPECT_EQ(eb.build(), "Error: incomplete email");   // still no subject
}

TEST(Q1_EmailBuilder, FullEmailWithoutAttachment) {
    EmailBuilder eb("alice@corp.com");
    string result = eb.to("bob@corp.com")
                      .withSubject("Meeting")
                      .withBody("See you at 3pm")
                      .build();
    EXPECT_EQ(result,
        "From: alice@corp.com\nTo: bob@corp.com\nSubject: Meeting\nBody: See you at 3pm");
}

TEST(Q1_EmailBuilder, AttachmentAppendsLine) {
    EmailBuilder eb("x@x.com");
    string result = eb.to("y@y.com")
                      .withSubject("Report")
                      .withBody("See attached")
                      .attach()
                      .build();
    EXPECT_EQ(result,
        "From: x@x.com\nTo: y@y.com\nSubject: Report\nBody: See attached\n[Attachment]");
}


// ============================================================
// Q2 – Inventory Item  (scenario)
// Concepts: copy constructor, *this chaining, initializer list,
//           two static members, price change
// ============================================================

TEST(Q2_InventoryItem, ConstructorUpdatesBothStatics) {
    int itemsBefore = InventoryItem::getTotalItems();
    int qtyBefore   = InventoryItem::getTotalQuantity();
    InventoryItem item("Notebook", "OfficeMax", 50.0, 100);
    EXPECT_EQ(InventoryItem::getTotalItems(),    itemsBefore + 1);
    EXPECT_EQ(InventoryItem::getTotalQuantity(), qtyBefore   + 100);
}

TEST(Q2_InventoryItem, CopyAppendsCloneAndUpdatesStatics) {
    int itemsBefore = InventoryItem::getTotalItems();
    int qtyBefore   = InventoryItem::getTotalQuantity();
    InventoryItem original("Pen", "PaperCo", 10.0, 50);
    InventoryItem clone = original;
    EXPECT_EQ(clone.getItemName(), "Pen (Clone)");
    EXPECT_EQ(InventoryItem::getTotalItems(),    itemsBefore + 2);
    EXPECT_EQ(InventoryItem::getTotalQuantity(), qtyBefore   + 100);
}

TEST(Q2_InventoryItem, RestockSellPriceChainAndTotalValue) {
    InventoryItem item("Eraser", "StatCo", 5.0, 20);
    item.restock(10).sell(5).applyPriceChange(20.0);  // qty 25, sell 5 -> 20, price +20% = 6.0
    EXPECT_EQ(item.getQuantity(), 20);
    EXPECT_DOUBLE_EQ(item.getUnitPrice(), 6.0);
    EXPECT_DOUBLE_EQ(item.totalValue(), 120.0);
}


// ============================================================
// Q3 – Sensor Logger
// Concepts: member initializer list with const members, *this chaining
// ============================================================

TEST(Q3_SensorLogger, ConstMembersAndInitialState) {
    SensorLogger sl("TEMP-01", 25.0, "C");
    EXPECT_EQ(sl.getSensorId(),         "TEMP-01");
    EXPECT_EQ(sl.getUnit(),             "C");
    EXPECT_DOUBLE_EQ(sl.getCurrentValue(), 25.0);
    EXPECT_DOUBLE_EQ(sl.getMin(),          25.0);
    EXPECT_DOUBLE_EQ(sl.getMax(),          25.0);
    EXPECT_EQ(sl.getReadingCount(),     1);
}

TEST(Q3_SensorLogger, RecordChainingUpdatesMinMaxAndRange) {
    SensorLogger sl("HUM-02", 50.0, "%");
    sl.record(30.0).record(70.0).record(45.0);
    EXPECT_DOUBLE_EQ(sl.getCurrentValue(), 45.0);
    EXPECT_DOUBLE_EQ(sl.getMin(), 30.0);
    EXPECT_DOUBLE_EQ(sl.getMax(), 70.0);
    EXPECT_DOUBLE_EQ(sl.getRange(), 40.0);
    EXPECT_EQ(sl.getReadingCount(), 4);  // 1 initial + 3 recorded
}

TEST(Q3_SensorLogger, GetSummaryFormat) {
    SensorLogger sl("PRS-03", 100.0, "kPa");
    sl.record(95.0).record(110.0);
    EXPECT_EQ(sl.getSummary(),
        "[PRS-03] cur:110 min:95 max:110 (3 readings)");
}


// ============================================================
// Q4 – Product Listing  (scenario)
// Concepts: copy constructor, *this chaining, initializer list,
//           revenue tracking static
// ============================================================

TEST(Q4_Product, CopyResetsCounterAndDoesNotAddRevenue) {
    int    prodBefore = Product::getTotalProducts();
    double revBefore  = Product::getTotalRevenue();
    Product original("Notebook", 150.0, 20);
    Product variant = original;
    EXPECT_EQ(variant.getName(),      "Notebook (Variant)");
    EXPECT_EQ(variant.getSoldCount(), 0);   // copy resets sold count
    EXPECT_EQ(Product::getTotalProducts(), prodBefore + 2);
    EXPECT_DOUBLE_EQ(Product::getTotalRevenue(), revBefore);  // no extra revenue from copy
}

TEST(Q4_Product, DiscountRestockAndSellUpdatesRevenue) {
    double revBefore = Product::getTotalRevenue();
    Product p("Pen", 100.0, 10);
    p.applyDiscount(10.0).restock(5).sell(3);  // price=90, stock=12, sell 3 -> 90*3=270
    EXPECT_DOUBLE_EQ(p.getPrice(), 90.0);
    EXPECT_EQ(p.getStock(), 9);
    EXPECT_EQ(p.getSoldCount(), 3);
    EXPECT_DOUBLE_EQ(Product::getTotalRevenue(), revBefore + 270.0);
}

TEST(Q4_Product, SellIgnoredWhenInsufficientStockAndStatusTiers) {
    Product p("Eraser", 20.0, 3);
    EXPECT_EQ(p.getStockStatus(), "Low Stock");
    p.sell(3);
    EXPECT_EQ(p.getStockStatus(), "Out of Stock");
    p.sell(1);                    // no stock — ignored
    EXPECT_EQ(p.getStock(), 0);
    p.restock(10);
    EXPECT_EQ(p.getStockStatus(), "In Stock");
}


// ============================================================
// Q5 – Hospital Patient  (scenario — ALL concepts)
// Concepts: const member + initializer list, copy constructor,
//           *this chaining, two statics, summary string
// ============================================================

TEST(Q5_Patient, ConstIdInitializerListAndStatics) {
    int transfersBefore = Patient::getTotalTransfers();
    int patientsBefore  = Patient::getTotalPatients();
    Patient p("Ahmed", 1001, "General", 37.0);
    EXPECT_EQ(p.getId(),   1001);
    EXPECT_EQ(p.getWard(), "General");
    EXPECT_EQ(Patient::getTotalPatients(),  patientsBefore  + 1);
    EXPECT_EQ(Patient::getTotalTransfers(), transfersBefore);   // no transfer yet
}

TEST(Q5_Patient, CopyResetsTransfersAndMarksTransfer) {
    int patientsBefore = Patient::getTotalPatients();
    Patient original("Mona", 2002, "Cardiology", 38.0);
    original.transfer("ICU").transfer("Recovery");   // 2 transfers on original
    Patient transferred = original;
    EXPECT_EQ(transferred.getName(),         "Mona (Transfer)");
    EXPECT_EQ(transferred.getId(),           2002);
    EXPECT_EQ(transferred.getWardTransfers(), 0);    // copy resets to 0
    EXPECT_EQ(Patient::getTotalPatients(),   patientsBefore + 2);
}

TEST(Q5_Patient, TransferTempChainingConditionAndSummary) {
    Patient p("Yusuf", 3003, "General", 37.0);
    EXPECT_EQ(p.getCondition(), "Normal");
    p.transfer("HDU").recordTemp(38.2).transfer("ICU").recordTemp(39.8);
    EXPECT_EQ(p.getWard(),           "ICU");
    EXPECT_EQ(p.getCondition(),      "Critical");
    EXPECT_EQ(p.getWardTransfers(),  2);
    EXPECT_EQ(p.getPatientSummary(),
        "Yusuf | Ward: ICU | Temp: 39.8C | Transfers: 2");
}


// ============================================================
// Main
// ============================================================

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
