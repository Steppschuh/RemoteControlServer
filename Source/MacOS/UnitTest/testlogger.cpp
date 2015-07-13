#include "testlogger.h"

void TestLogger::initTestCase()
{
}

void TestLogger::test1()
{
    QVERIFY(1 + 1 == 2);
}

void TestLogger::test2()
{
    QVERIFY(1 == 0);
}

void TestLogger::cleanupTestCase()
{
}
