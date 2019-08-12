/*
	Simple TCP Server
*/

#include<stdio.h>
#include<string.h>
#include<winsock2.h>
#include<Windows.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define BUFLEN 512	//Max length of buffer
#define PORT 1980	//The port on which to listen for incoming data

int main()
{
	SOCKET sock,clsock;
	struct sockaddr_in server, si_other;
	int slen, recv_len;
	char buf[BUFLEN];
	WSADATA wsa;

	slen = sizeof(si_other);
	char message[512];

	//Initialise winsock
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Initialised.\n");

	//Create a socket
	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}
	printf("Socket created.\n");

	//Prepare the sockaddr_in structure
	server.sin_family = PF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);

	//Bind
	if (bind(sock, (struct sockaddr*) & server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	puts("Bind done");
	
	
	//keep listening for data
	if (listen(sock, SOMAXCONN) == SOCKET_ERROR) {

		printf("listen failed with error code : %d ", WSAGetLastError());
	}
	puts("listen done");
	

	while (0 < (clsock = accept(sock, (sockaddr*)& si_other, &slen)))
	{
		//fflush(stdout);
		//clear the buffer by filling null, it might have previously received data
		
		//try to receive some data, this is a blocking call

		while (1)
		{
			memset(buf, '\0', BUFLEN);
			int result = recv(clsock, buf, BUFLEN, 0);
			if (result == SOCKET_ERROR)
			{
				printf("recv() failed with error code : %d", WSAGetLastError());
				exit(EXIT_FAILURE);
			}
			else if (result == 0) {
				printf("recv() failed with error code : %d", WSAGetLastError());
				break;
			}
			printf("%s", buf);
		}

		//print details of the client/peer and the data received
		//printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
		//printf("Data: %s\n", buf);
		

		/*
		//now reply the client with the same data
		if (sendto(sock, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == SOCKET_ERROR)
		{
			printf("sendto() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}
		*/
	}
	closesocket(clsock);
	closesocket(sock);
	WSACleanup();
}