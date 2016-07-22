#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <string>
#include <vector>


class MessageHandler
{

	typedef std::vector<std::string> Strings;

	static const std::string _SUPPORTED_HTTP_VERSION;
	static const Strings     _SUPPORTED_METHODS;

public:
	MessageHandler(const std::string &rootDir);
	std::string handleMessage(const std::string &request);


private:
	std::string handleGet(const Strings &requestLines);

	std::string findHeader(const Strings &lines, const std::string &header);

	std::string ok() const;
	std::string ok(const std::string &type, const std::string &result) const;
	std::string notFound() const;
	std::string badRequest() const;
	std::string internalError() const;
	std::string notImplemented() const;

private:
	std::string _rootDir;

};

#endif //REQUEST_HANDLER_H
