#include "test_string_utils.h"

#include <QTest>

#include <string>
#include <vector>

Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(std::vector<std::string>)

void Test_string_utils::simple_testcase()
{
	QCOMPARE(std::string("123"), std::string("123"));
}

void Test_string_utils::string_utils_data()
{
	//NOTE: just for QTest test

	QTest::addColumn<std::string>("line");
	QTest::addColumn<std::string>("delimer");
	QTest::addColumn<std::vector<std::string>>("result");

	QTest::newRow("test case 1")
	        << std::string("123")
	        << std::string(" ")
	        << std::vector<std::string>({"123"}) ;
	QTest::newRow("test case 2")
	        << std::string("1 2  3")
	        << std::string(" ")
	        << std::vector<std::string>({"1", "2", "", "3"}) ;
	QTest::newRow("test case 3")
	        << std::string("12")
	        << std::string(" ")
	        << std::vector<std::string>({"1", "2"}) ;
}

void Test_string_utils::string_utils()
{
	QFETCH(std::string, line);
	QFETCH(std::string, delimer);
	QFETCH(std::vector<std::string>, result);
	QCOMPARE(split<std::vector<std::string> >(line, delimer), result);
}
