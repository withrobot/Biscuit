
#ifndef _SESSION_HPP_
#define _SESSION_HPP_

#include <set>
#include <map>
#include <string>


class SessionContext
{
	public:
		SessionContext(int sock, const std::string address)
		: m_sock(sock)
		, m_address(address)
		{
		}

		int m_sock;
		std::string m_address;
};

void* MobileMouseSession(void* context);

void process(char *buffer);

#endif
