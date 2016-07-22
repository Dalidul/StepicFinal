#include "uri.h"

URI_MOCK::URI_MOCK(const std::string &uriStr)
{
	//TODO: parse URI
	_file = uriStr.substr(0, uriStr.find_first_of('?'));  //MOCK
}

std::string URI_MOCK::getFile() const
{
	return _file;
}
