
#include "../sockets/server/socketServer.h"
#include "../sockets/client/socketClient.h"
#include <cstring>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <ws2def.h>
#include <stdio.h>

#include <thread>


#pragma comment(lib, "Ws2_32.lib")

void f() {}

int main() {

	WSAData wsdata;
	int result
		= WSAStartup(MAKEWORD(2, 2), &wsdata);

	if (result != 0) {
		std::cerr << "WSAStartup failed: " << result << "\n";
		return result;
	}

	// run server proccess (background)
	socketServer srv("127.0.0.1", "8080", 15);
	std::thread tr(&socketServer::start, &srv);


	socketClient client("8080");
	client.sockConnect();

	client.sendMsg("test test\n");

	// run clients (main process)
	tr.join();

	int t = 3;

	WSACleanup();
}