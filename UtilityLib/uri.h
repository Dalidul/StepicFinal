#ifndef URI_H
#define URI_H

#include <string>

// TODO: replace to http://cpp-netlib.github.com

class URI_MOCK
{
public:
	URI_MOCK(const std::string &uriStr);
	std::string getFile() const;

private:
	std::string _file;
};

#endif //URI_H
