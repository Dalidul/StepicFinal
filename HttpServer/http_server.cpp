#include "http_server.h"

#include "message_handler.h"
#include "net_utils.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdexcept>
#include <cassert>
#include <unistd.h>


// static consts ---------------------------------------------------------------

size_t HttpServer::MAX_EVENTS_COUNT = 64;


// public ----------------------------------------------------------------------

HttpServer::HttpServer(const std::string &ip,
                       int port,
                       const std::string &rootDir)
    : _ip(ip)
    , _port(port)
    , _rootDir(rootDir)
    , _masterSocket(-1)
    , _epollFd(-1)
{
	_masterSocket = prepareMasterSocket();
	_epollFd = prepareEpollFd();

	createEpollReadEvent(_masterSocket);
}

HttpServer::~HttpServer()
{
	close(_epollFd);
	shutdown(_masterSocket, SHUT_RDWR);
	close(_masterSocket);
}

void HttpServer::run()
{
	initWorkersPool();

	while(1) {
		struct epoll_event Events[MAX_EVENTS_COUNT];
		int readyFdCount = epoll_wait(_epollFd, Events, MAX_EVENTS_COUNT, -1);
		if (readyFdCount == -1) {
			throw std::runtime_error("epoll_wait worked incorect.");
		}

		for (int i = 0; i < readyFdCount; ++i) {
			if (Events[i].data.fd == _masterSocket) {
				int slaveSocket = accept(_masterSocket, 0, 0);
				if (slaveSocket == -1) {
					throw std::runtime_error(
					            "It's impossible to accept slave socket.");
				}
				setNonblock(slaveSocket);
				createEpollReadEvent(slaveSocket);
			}
			else {
				int readableSlaveSocket = Events[i].data.fd;
				_workersPool->addDataToQueue(readableSlaveSocket);
			}
		}
	}
}


// private ---------------------------------------------------------------------

int HttpServer::prepareMasterSocket() const
{
	int masterSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (masterSocket == -1) {
		throw std::runtime_error("It's impossible to create master socket.");
	}
	int enable = 1;
	if (setsockopt(masterSocket, SOL_SOCKET,
	               SO_REUSEADDR, &enable, sizeof(int)) == -1) {
		throw std::runtime_error(
		            "It's impossible to set master socket reusable.");
	}

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(_port);
	if (inet_pton(AF_INET, _ip.c_str(), &(addr.sin_addr.s_addr)) == 0) {
		throw std::runtime_error("Invalid format ip string.");
	}
	if (bind(masterSocket,
	         reinterpret_cast<sockaddr *>(&addr),
	         sizeof(addr)) == -1) {
		throw std::runtime_error("It's impossible to bind master socket.");
	}

	if (setNonblock(masterSocket) == -1) {
		throw std::runtime_error(
		            "It's impossible to set master socket nonblocked.");
	}

	if (listen(masterSocket, SOMAXCONN)) {
		throw std::runtime_error(
		            "It's impossible to start listen master socket.");
	}

	return masterSocket;
}

int HttpServer::prepareEpollFd() const
{
	int epollFd = epoll_create1(0);
	if (epollFd == -1) {
		throw std::runtime_error("It's impossible to create epoll fd.");
	}
	return epollFd;
}

void HttpServer::createEpollReadEvent(int sock)
{
	assert(_epollFd != -1);
	assert(sock != -1);

	struct epoll_event readEvent;
	readEvent.data.fd = sock;
	readEvent.events = EPOLLIN | EPOLLET;
	epoll_ctl(_epollFd, EPOLL_CTL_ADD, sock, &readEvent);
}

void HttpServer::initWorkersPool()
{
	auto wrapper = [](int sock)->void {
		const size_t BUFF_SIZE = 1024;

		std::string message;

		char buff[BUFF_SIZE];

		int bytesRead = 1;
		while(bytesRead > 0) {
			bytesRead = recv(sock, buff, BUFF_SIZE, MSG_NOSIGNAL);
			if (!bytesRead && (errno != EAGAIN)) {
				shutdown(sock, SHUT_RDWR);
				close(sock);
			}
			else if (bytesRead > 0) message.append(buff, bytesRead);
		}

		if (!message.empty()) {
			//TODO: handle with to MessageHandler
			send(sock, message.c_str(), message.size(), MSG_NOSIGNAL);  //MOCK
		};
	};

	_workersPool.reset(new WorkersPool<int>(wrapper));
}
