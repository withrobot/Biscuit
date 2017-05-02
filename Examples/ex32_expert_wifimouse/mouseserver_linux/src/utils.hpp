

#ifndef _UTILS_HPP_
#define _UTILS_HPP_

#include <list>
#include <string>

std::list<std::string> SplitString(const std::string &input, char delimiter);

void dumpPacket(const char* buffer);

#endif
