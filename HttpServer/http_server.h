#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "workers_pool.h"

#include <string>
#include <memory>

class HttpServer
{
	static size_t MAX_EVENTS_COUNT;

public:
	HttpServer(const std::string & ip, int port, const std::string &rootDir);
	~HttpServer();
	void run();

private:
	int  prepareMasterSocket() const;
	int  prepareEpollFd() const;
	void createEpollReadEvent(int sock);
	void initWorkersPool();

private:
	std::string _ip;
	int         _port;
	std::string _rootDir;

	int _masterSocket;
	int _epollFd;

	std::shared_ptr<WorkersPool<int> > _workersPool;

};

#endif //HTTP_SERVER_H
