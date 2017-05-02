

#include "utils.hpp"
#include <sstream>
#include <string.h>
#include <stdio.h>

std::list<std::string> SplitString(const std::string &input, char delimiter)
{
	std::list<std::string> result;
	std::string tmp;
	std::stringstream ss(input);
	while(std::getline(ss, tmp, delimiter))
	{
		result.push_back(tmp);
	}
	return result;
}

void dumpPacket(const char* buffer) 
{
	unsigned int p = 0;
	while(p < strlen((char*)buffer))
	{
		printf("  0x%04x: ", p);
		for(unsigned int i = 0, i2 = p; i < 16; i++, i2++)
		{
			if (i2 < strlen((char*)buffer))
				printf(" %02x", 0xff & buffer[i2]);
			else
				printf("   ");
		}
		printf("    ");
		for(unsigned int i = 0, i2 = p; i < 16; i++, i2++)
		{
			if (i2 < strlen((char*)buffer))
				printf("%c", isprint(buffer[i2])?buffer[i2]:'.');
			else
				printf(" ");
		}
		p += 16;
		printf("\n");
	}
}
