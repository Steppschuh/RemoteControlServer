#ifndef TESTLOGGER_H
#define TESTLOGGER_H

#include "AutoTest.h"

class TestLogger : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void test1();
    void test2();
    void cleanupTestCase();
};

DECLARE_TEST(TestLogger)

#endif // TESTLOGGER_H
