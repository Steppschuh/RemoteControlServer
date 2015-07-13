#ifndef TESTLOGGER_H
#define TESTLOGGER_H

#include "AutoTest.h"
#include "logger.h"

class TestLogger : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testAdd();
    void testAddWithDebugBool();
    void testInvalidateLog();
    void testGetLastEntry();
    void testTrackLaunchEvent();
    void testTrackEvent();
    void cleanupTestCase();
};

DECLARE_TEST(TestLogger)

#endif // TESTLOGGER_H
