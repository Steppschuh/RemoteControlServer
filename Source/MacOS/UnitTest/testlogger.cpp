#include "testlogger.h"

void TestLogger::initTestCase()
{
}

void TestLogger::testAdd(){
    QString message = "Test";
    Logger::Instance()->add(message);
    QVERIFY(Logger::Instance()->logMessages->last().contains(message));
}

//void TestLogger::testAddWithDebugBool(){
//    QString message = "TestWithDebugBool";
//    Logger::Instance()->add(message, false);
//    QVERIFY(Logger::Instance()->logMessages->last().contains(message));
//}

//void TestLogger::testInvalidateLog()
//{

//}

//void TestLogger::testGetLastEntry()
//{
//    //Test a message smaller than 30 chars length
//    QString message = "SmallerThan30";
//    QString expected = message;
//    Logger::Instance()->add(message);
//    QString lastEntry = Logger::Instance()->getLastEntry();
//    QVERIFY(lastEntry == expected);

//    //Test a message higher than 30 chars length without blanks
//    message = "ThisIsAVeryLongTextLongerThan30Chars";
//    expected = "ThisIsAVeryLongTextLongerTh...";
//    Logger::Instance()->add(message);
//    lastEntry = Logger::Instance()->getLastEntry();
//    QVERIFY(lastEntry == expected);

//    //Test a message higher than 30 chars length with blanks
//    message = "This Is A Very Long Text Longer Than 30 Chars";
//    expected = "This Is A Very Long Text...";
//    Logger::Instance()->add(message);
//    lastEntry = Logger::Instance()->getLastEntry();
//    QVERIFY(lastEntry == expected);
//}

//void TestLogger::testTrackLaunchEvent()
//{

//}

//void TestLogger::testTrackEvent()
//{

//}

void TestLogger::cleanupTestCase()
{
}
