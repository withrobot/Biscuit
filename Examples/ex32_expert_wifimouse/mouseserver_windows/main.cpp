#include <winsock2.h>
#include <Windows.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

#define BUFLEN 512
#define PORT 1980

#define BTN_UP (0)
#define BTN_DOWN (1)

int screen_width = 1920;
int screen_height = 1080;


void mouseMove(int x, int y)
{
    int x_pos = x * 65535 / screen_width;
    int y_pos = y * 65535 / screen_height;

    mouse_event(MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE, x_pos, y_pos, 0, GetMessageExtraInfo());
}

void mouseLeft(int up_down)
{
    POINT MousePos;
    GetCursorPos(&MousePos);

    int x_pos = MousePos.x * 65535 / screen_width;
    int y_pos = MousePos.y * 65535 / screen_height;

    if (up_down == BTN_DOWN) // down
        mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_ABSOLUTE, x_pos, y_pos, 0, GetMessageExtraInfo());
    else // up
        mouse_event(MOUSEEVENTF_LEFTUP | MOUSEEVENTF_ABSOLUTE, x_pos, y_pos, 0, GetMessageExtraInfo());
}

void mouseRight(int up_down)
{
    POINT MousePos;
    GetCursorPos(&MousePos);

    int x_pos = MousePos.x * 65535 / screen_width;
    int y_pos = MousePos.y * 65535 / screen_height;

    if (up_down == BTN_DOWN) // down
        mouse_event(MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_ABSOLUTE, x_pos, y_pos, 0, GetMessageExtraInfo());
    else // up
        mouse_event(MOUSEEVENTF_RIGHTUP | MOUSEEVENTF_ABSOLUTE, x_pos, y_pos, 0, GetMessageExtraInfo());
}

void process(char *buffer) {

    if (buffer[0] == '\0')
        return;
    //printf("process buffer :%s:", buffer);
    if (strncmp(buffer, "mos", 3) == 0) {
        char *p = buffer + 3;
        char sl[3];
        strncpy(sl, p, 3);
        int il = atoi(sl);

        if (buffer[6] == 'm') {
            p = p + 5;
            char *p1 = strstr(p, " ");
            char datax[6];
            memset(&datax, 0x00, 6);
            strncpy(datax, p, p1 - p);
            int ix = atoi(datax);

            p1++;
            char datay[6];
            memset(&datay, 0x00, 6);
            strncpy(datay, p1, il - strlen(datax) - 3);
            int iy = atoi(datay);

            //mouseMove(ix, iy);
        }
        else if (buffer[6] == 'p')
        {
            p = p + 5;
            char *p1 = strstr(p, " ");
            char datax[6];
            memset(&datax, 0x00, 6);
            strncpy(datax, p, p1 - p);
            int ix = atoi(datax);
            p1++;
            char datay[6];
            memset(&datay, 0x00, 6);
            strncpy(datay, p1, il - strlen(datax) - 3);
            int iy = atoi(datay);

            mouseMove(ix, iy);
        }
        else if (buffer[6] == 'r')
        {
            mouseMove(screen_width / 2, screen_height / 2);
        }
        else if (buffer[6] == 'R')
        {
            p = p + 5;
            if (p[0] == 'l') {
                if (p[2] == 'd') {
                    mouseLeft(BTN_DOWN);
                }
                else if (p[2] == 'u') {
                    mouseLeft(BTN_UP);
                }
            }
            else if (p[0] == 'r') {
                if (p[2] == 'd') {
                    mouseRight(BTN_DOWN);
                }
                else if (p[2] == 'u') {
                    mouseRight(BTN_UP);
                }
            }
            /*
            else if (p[0] == 'm') {
                if (p[2] == 'd') {
                    mouseMid(BTN_DOWN);
                }
                else if (p[2] == 'u') {
                    mouseMid(BTN_UP);
                }
            }
            */
        }
        else if (buffer[6] == 'c') {
            mouseLeft(BTN_DOWN);
            mouseLeft(BTN_UP);
        }

        process(buffer + 6 + il);
    }
}

void main(void)
{
//    screen_width = GetSystemMetrics(SM_CXSCREEN);
//    screen_height = GetSystemMetrics(SM_CYSCREEN);


    SOCKET sock;
    struct sockaddr_in server, si_other;
    int slen, recv_len;
    char buf[BUFLEN];
    WSADATA wsa;

    slen = sizeof(si_other);

    printf("Screen Size : %d, %d\r\n", screen_width, screen_height);
    //Initialise winsock
    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Failed. Error Code : %d", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    printf("Initialised.\n");

    //Create a socket
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d", WSAGetLastError());
    }
    printf("Socket created.\n");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    //Bind
    if (bind(sock, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    puts("Bind done");

    //keep listening for data
    while (1)
    {
        fflush(stdout);

        //clear the buffer by filling null, it might have previously received data
        memset(buf, '\0', BUFLEN);

        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(sock, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == SOCKET_ERROR)
        {
            printf("recvfrom() failed with error code : %d", WSAGetLastError());
            exit(EXIT_FAILURE);
        }

        //print details of the client/peer and the data received
        //printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        //printf("Data: %s\n", buf);
        printf("%s\n", buf);
        process(buf);

        /*
        //now reply the client with the same data
        if (sendto(sock, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == SOCKET_ERROR)
        {
            printf("sendto() failed with error code : %d", WSAGetLastError());
            exit(EXIT_FAILURE);
        }
        */
        Sleep(1);
    }

    closesocket(sock);
    WSACleanup();
}