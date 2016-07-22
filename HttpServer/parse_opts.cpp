#include "parse_opts.h"

#include <unistd.h>
#include <limits>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <stdexcept>

#include "string_utils.h"
#include "net_utils.h"


ValidatedInputArgs::ValidatedInputArgs()
    : _port(0)
    , _paramsIsValid(VALID_FILED_STATES_NO_ONE)
{}

void ValidatedInputArgs::invalidate()
{
	_paramsIsValid = VALID_FILED_STATES_NO_ONE;
}

bool ValidatedInputArgs::setIp(const char * const ipString)
{
	if (ipStrIsValid(ipString)) {
		_ip = ipString;
		_paramsIsValid = _paramsIsValid | VALID_FILED_STATES_IP;
		return true;
	}
	else {
		_paramsIsValid = _paramsIsValid & ~VALID_FILED_STATES_IP;
		return false;
	}
}

bool ValidatedInputArgs::setPort(const char * const portString)
{
	char truncatedPortStr[strlen(portString)+1];
	truncateStr(portString, truncatedPortStr);
	if (isNumber(truncatedPortStr)) {
		long res = strtol(truncatedPortStr, NULL, 10);
		if ((res <= std::numeric_limits<int16_t>::max()) && (res >= 0)
		    && (errno != EINVAL)) {
			_port = res;
			_paramsIsValid = _paramsIsValid | VALID_FILED_STATES_PORT;
			return true;
		}
	}
	_paramsIsValid = _paramsIsValid & ~VALID_FILED_STATES_PORT;
	return false;
}

bool ValidatedInputArgs::setRootDir(const char * const rootDirString)
{
	_rootDir = rootDirString;
	_paramsIsValid = _paramsIsValid | VALID_FILED_STATES_DIR;
	return true;
}

bool ValidatedInputArgs::isValid() const
{
	return _paramsIsValid == VALID_FILED_STATES_ALL_FIELDS;
}

std::string ValidatedInputArgs::getIp() const
{
	return _ip;
}

int ValidatedInputArgs::getPort() const
{
	return _port;
}

std::string ValidatedInputArgs::getRootDir() const
{
	return _rootDir;
}


// =============================================================================
void printHelp()
{
	std::cout << "usage: httpServerExample -h ip -p port -d directory"
	          << std::endl;
}

ValidatedInputArgs parseArgs(int argc, char **argv)
{
	ValidatedInputArgs args;
	int opt = -2;
	while ((opt = getopt(argc, argv, "h:p:d:")) != -1) {
		switch (opt) {
			case 'h':
				if (!args.setIp(optarg)) {
					std::cout << "Error: Ip value is invalid." << std::endl;
					return args;
				}
				break;
			case 'p':
				if (!args.setPort(optarg)) {
					std::cout << "Error: Port value is invalid." << std::endl;
					return args;
				}
				break;
			case 'd':
				if (!args.setRootDir(optarg)) {
					std::cout << "Error: Directory value is invalid."
					          << std::endl;
					return args;
				}
				break;
			default: /* '?' */
				args.invalidate();
				return args;
		}
	}
	return args;
}

