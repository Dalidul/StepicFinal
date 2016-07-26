#ifndef TEST_STRING_UTILS_H
#define TEST_STRING_UTILS_H

#include <QObject>

#include "string_utils.h"


class Test_string_utils: public QObject
{
	Q_OBJECT

public:
	explicit Test_string_utils(QObject *parent = 0)
		: QObject(parent)
	{}

private slots:
	void simple_testcase();
	void string_utils_data();
	void string_utils();
};

#endif // TEST_STRING_UTILS_H
