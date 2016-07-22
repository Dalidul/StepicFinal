#include <unistd.h>
#include <iostream>

#include "parse_opts.h"
#include "http_server.h"
#include <vector>

int main(int argc, char **argv) {
	if (argc == 1) {
		printHelp();
		return 0;
	}

	ValidatedInputArgs args = parseArgs(argc, argv);
	if (!args.isValid()) {
		printHelp();
		return 1;
	}

//	int detachedSuccessfully = !daemon(1, 0);
//	if (detachedSuccessfully) {
		HttpServer server(args.getIp(), args.getPort(), args.getRootDir());
		server.run();
		return 0;
//	}
//	else return 2;
}
