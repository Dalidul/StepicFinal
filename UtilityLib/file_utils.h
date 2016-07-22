#ifndef UTILITY_LIB_H
#define UTILITY_LIB_H

#include <string>


std::string getFileExt(const std::string &filePath);

bool fileExists(const std::string &filePath);

bool fileIsExecutable(const std::string &filePath);

bool getFileModificationTime(const std::string &filePath, time_t &time);

#endif //UTILITY_LIB_H
