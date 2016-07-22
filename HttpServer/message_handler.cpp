#include "message_handler.h"

#include <vector>
#include <algorithm>
#include <cassert>
#include <fstream>

#include "string_utils.h"
#include "file_utils.h"
#include "uri.h"


// static consts ---------------------------------------------------------------

const std::string MessageHandler::_SUPPORTED_HTTP_VERSION = "HTTP/1.0";
const MessageHandler::Strings MessageHandler::_SUPPORTED_METHODS = {"GET"};


// public ----------------------------------------------------------------------

MessageHandler::MessageHandler(const std::string &rootDir)
{
	assert(!rootDir.empty());

	_rootDir = rootDir;
	if (_rootDir.back() != '/') _rootDir += '/';
}

std::string MessageHandler::handleMessage(const std::string &request)
{
	Strings lines;
	split(request, "\n", lines);

	if (lines.empty()) return badRequest();

	if (lines[0].back() == '\r') {
		for (auto iter = lines.begin(); iter != lines.end(); ++iter) {
			iter->pop_back();
		}
	}

	Strings titleParts;
	split(lines[0], " ", titleParts);

	if (titleParts.empty()) return badRequest();

	std::string methodType = titleParts[0];

	if (methodType == "GET") {
		return handleGet(lines);
	}
	else return notImplemented();
}


// private ---------------------------------------------------------------------

std::string MessageHandler::handleGet(const Strings &requestLines)
{
	Strings tokens = split<Strings>(requestLines[0], " ");

	if (tokens.size() != 3) return badRequest();

	std::string httpVersion = tokens[2];
	if (httpVersion != _SUPPORTED_HTTP_VERSION) return badRequest();

	//TODO: rewrite it later:

	std::string uriStr = tokens[1];
	URI_MOCK uri(uriStr);
	std::string requestedFile = _rootDir + uri.getFile();

	if (!fileExists(requestedFile)) return notFound();

	std::string conditionHeader =
	        findHeader(requestLines, "If-Modified-Since");
	if (!conditionHeader.empty()) {
		std::string::const_iterator pos=
		        std::find(conditionHeader.begin(), conditionHeader.end(), ':');
		if (pos == conditionHeader.end()) return badRequest();

		time_t lastFileModificationTime = 0;
		if (getFileModificationTime(requestedFile, lastFileModificationTime)) {
			return internalError();
		}

		//TODO: time reformat and check
		return notImplemented();
	}

	if (fileIsExecutable(requestedFile)) return notImplemented();

	std::ifstream file(requestedFile);
	if (!file.good()) return internalError();
	std::string fileData;
	file >> fileData;
	file.close();

	return ok(getFileExt(uri.getFile()), fileData);
}


std::string MessageHandler::findHeader(const Strings &lines,
                                       const std::string &header)
{
	bool prevLineIsEmpty = false;
	for (auto iter = lines.begin(); iter != lines.end(); ++iter) {
		if (iter->empty()) {
			if (prevLineIsEmpty) return std::string();  // body reached
			else prevLineIsEmpty = true;
		}
		else {
			prevLineIsEmpty = false;
			if (std::string(iter->begin(), iter->begin() + header.size())
			        == header) {
				return *iter;
			}
		}
	}
	return std::string();
}

std::string MessageHandler::ok() const
{
	return "HTTP/1.0 200 OK\r\n\r\n";
}

std::string MessageHandler::ok(const std::string &type,
                               const std::string &result) const
{
	std::string response = "HTTP/1.0 200 OK\r\n";
	response += "Content-Type: " + type + "\r\n";
	response += "Content-Length: " + std::to_string(result.size()) + "\r\n";
	response += "\r\n\r\n";
	response += result;
	return response;
}

std::string MessageHandler::notFound() const
{
	return "HTTP/1.0 404 Not found\r\n\r\n";
}

std::string MessageHandler::badRequest() const
{
	return "HTTP/1.0 400 Bad Request\r\n\r\n";
}

std::string MessageHandler::internalError() const
{
	return "HTTP/1.0 500 Internal Server Error\r\n\r\n";
}

std::string MessageHandler::notImplemented() const
{
	return "HTTP/1.0 501 Not Implemented\r\n\r\n";
}
