#include <QCoreApplication>
#include <QTest>

#ifdef LOG_TEST_OUT
#include <stdio.h>
#endif

#include "test_string_utils.h"

int main(int argc, char **argv)
{
#ifdef LOG_TEST_OUT
	freopen("testing.log", "w", stdout);
#endif
	QCoreApplication a(argc, argv);
	QTest::qExec(new Test_string_utils(), argc, argv);
	return 0;
}
