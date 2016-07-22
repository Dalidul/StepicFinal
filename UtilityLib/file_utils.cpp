#include "file_utils.h"


#include <fstream>

//#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>



bool fileExists(const std::string &filePath)
{
	std::ifstream file(filePath);
	return file.good();
}

bool fileIsExecutable(const std::string &filePath)
{
	struct stat info;
	if(stat(filePath.c_str(), &info) != -1) {
		return info.st_mode & S_IXUSR;
	}
	else return false;
}

bool getFileModificationTime(const std::string &filePath, time_t &time)
{
	struct stat info;
	if(stat(filePath.c_str(), &info) != -1) {
		time = info.st_mtime;
		return true;
	}
	else return false;
}

std::string getFileExt(const std::string &filePath)
{
	return filePath.substr(filePath.find_last_of('.') + 1);
}
