#ifndef PARSE_OPTS_H
#define PARSE_OPTS_H

#include <string>


class ValidatedInputArgs {
	enum {
		VALID_FILED_STATES_NO_ONE =     0b000,
		VALID_FILED_STATES_IP =         0b001,
		VALID_FILED_STATES_PORT =       0b010,
		VALID_FILED_STATES_DIR =        0b100,
		VALID_FILED_STATES_ALL_FIELDS = 0b111
	};

public:
	ValidatedInputArgs();
	void invalidate();
	bool setIp(const char * const ipStr);
	bool setPort(const char * const portStr);
	bool setRootDir(const char * const rootDirStr);

	bool isValid() const;
	std::string getIp() const;
	int getPort() const;
	std::string getRootDir() const;

private:
	std::string _ip;
	int         _port;
	std::string _rootDir;

	int _paramsIsValid;
};

void printHelp();
ValidatedInputArgs parseArgs(int argc, char **argv);

#endif //PARSE_OPTS_H
