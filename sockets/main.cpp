
#include "../sockets/server/socketServer.h"
#include "../sockets/client/socketClient.h"
#include <cstring>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <ws2def.h>


#pragma comment(lib, "Ws2_32.lib")

int main() {

	WSAData wsdata;
	int result
		= WSAStartup(MAKEWORD(2, 2), &wsdata);

	if (result != 0) {
		std::cerr << "WSAStartup failed: " << result << "\n";
		return result;
	}

	// run server proccess (background)
	//socketServer srv("127.0.0.1", "8080", 15);
	//srv.start();

	socketClient client("8080");
	client.sockConnect();

	// run clients (main process)
	int t = 3;

	WSACleanup();
}