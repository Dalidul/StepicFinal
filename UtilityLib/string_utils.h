#ifndef STRING_UTILS_H
#define STRING_UTILS_H


#include <type_traits>
#include <string>
#include <algorithm>


// split =======================================================================

template <typename ContainerT, typename ValueT>
void split(const ValueT &str,
           const ValueT &delimer,
           ContainerT &outContainer,
           bool saveEmpty = true)
{
	static_assert(std::is_same<typename ContainerT::value_type, ValueT>::value,
	              "Inconsistent container value and function arg types.");

	using ValueIter = typename ValueT::const_iterator;

	ValueIter start = str.begin();

	while(true) {
		ValueIter occurrencePos = std::search(start, str.end(),
		                                      delimer.begin(), delimer.end());
		if(occurrencePos != str.end()) {
			if (!saveEmpty && (start == occurrencePos)) {
				start = ++occurrencePos;
				continue;
			}
			else {
				outContainer.push_back(ValueT(start, occurrencePos));
				start = ++occurrencePos;
			}
		}
		else {
			if (!saveEmpty && (start == occurrencePos)) break;
			else {
				outContainer.push_back(ValueT(start, occurrencePos));
				break;
			}
		}
	}
}

template <typename ContainerT>
void split(const char *str,
           const char *delimer,
           ContainerT &outContainer,
           bool saveEmpty = true)
{
	split(std::string(str), std::string(delimer), outContainer, saveEmpty);
}

template <typename ContainerT>
void split(const std::string &str,
           const char *delimer,
           ContainerT &outContainer,
           bool saveEmpty = true)
{
	split(str, std::string(delimer), outContainer, saveEmpty);
}

template <typename ContainerT, typename ValueT>
ContainerT split(const ValueT &str,
                 const ValueT &delimer,
                 bool saveEmpty = true)
{
	ContainerT result;
	split(str, delimer, result, saveEmpty);
	return result;
}

template <typename ContainerT>
ContainerT split(const char *str,
                 const char *delimer,
                 bool saveEmpty = true)
{
	return split<ContainerT>(std::string(str), std::string(delimer), saveEmpty);
}

template <typename ContainerT>
ContainerT split(const std::string &str,
                 const char *delimer,
                 bool saveEmpty = true)
{
	return split<ContainerT>(str, std::string(delimer), saveEmpty);
}


// join ========================================================================

template <typename ContainerT, typename ValueT>
void join(const ContainerT &lines, const ValueT &combiner, ValueT &result)
{
	static_assert(std::is_same<typename ContainerT::value_type, ValueT>::value,
	              "Inconsistent container value and function arg types.");

	typename ContainerT::size_type fullLength = 0;
	for (const auto &line: lines) {
		fullLength += line.size();
	}
	fullLength += (lines.size() - 1) * combiner.size();

	result.reserve(fullLength + 1);

	for (const auto &line: lines) {
		result += line;
		if (&line != &lines.back()) result += combiner;
	}
}

template <typename ContainerT, typename ValueT>
void join(const ContainerT &lines, const char *combiner, ValueT &result)
{
	join(lines, std::string(combiner), result);
}

template <typename ContainerT, typename ValueT>
std::string join(const ContainerT &lines, const ValueT &combiner)
{
	ValueT result;
	join(lines, combiner, result);
	return result;
}

template <typename ContainerT>
std::string join(const ContainerT &lines, const char *combiner)
{
	return join(lines, std::string(combiner));
}


// =============================================================================

// outStr size must be >= srcStr size
void truncateStr(const char * const srcStr, char * const outStr);

bool isNumber(const char * const testedStr);
#endif //STRING_UTILS_H
