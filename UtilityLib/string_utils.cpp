#include "string_utils.h"

void truncateStr(const char * const srcStr, char * const outStr)
{
	const char *srcIter = srcStr;
	char *outIter = outStr;
	while ((*srcIter != '\0') && isspace(*srcIter)) ++srcIter;
	--srcIter;
	--outIter;
	do {
		++srcIter;
		++outIter;
		*outIter = *srcIter;
	} while (*srcIter != '\0');

	if (outIter == outStr) return;
	do --outIter; while (isspace(*outIter));
	*(outIter+1) = '\0';
}

bool isNumber(const char * const testedStr)
{
	for(const char *iter = testedStr; *iter; ++iter) {
		if (!isdigit(*iter)) return false;
	}
	return true;
}
