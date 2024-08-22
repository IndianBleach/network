#define _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS

#define _WIN32_WINNT 0x501

#include "socketServer.h"
#include <cstring>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

socketServer::socketServer(const char* ipAddr, const char* port, unsigned int maxConnections)
	: m_SocketId(0),
	m_Port(port),
	m_maxConnections(maxConnections) 
{

	//char port_str[4];
	//char* ptr =itoa(port, port_str, 10);

	addrinfo addr_hints;
	ZeroMemory(&addr_hints, sizeof(addr_hints));
	addr_hints.ai_family = AF_INET;
	addr_hints.ai_socktype = SOCK_STREAM;
	addr_hints.ai_flags = AI_PASSIVE;
	addr_hints.ai_protocol = IPPROTO_TCP;

	int get_addr_result;
	get_addr_result = getaddrinfo(ipAddr, port, &addr_hints, &m_AddrInfo);

	if (get_addr_result != 0) {
		std::cerr << "socketServer: err: getaddrinfo bad result; code=" << get_addr_result << std::endl;
		return;
	}

	int socket_result;
	socket_result = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (socket_result == INVALID_SOCKET) {
		std::cerr << "socketServer: err: create socket bad result; code=" << socket_result << std::endl;
		freeaddrinfo(m_AddrInfo);
		return;
	}

	m_SocketId = socket_result;
} 

void socketServer::start() {
	
	// bind
	int bind_result
		= bind(m_SocketId, m_AddrInfo->ai_addr, m_AddrInfo->ai_addrlen);

	if (bind_result == SOCKET_ERROR) {
		std::cerr << "socketServer: err: call=start.bind(); code=" << bind_result << std::endl;
		return;
	}

	// listen
	int listen_result
		= listen(m_SocketId, m_maxConnections);

	if (listen_result == SOCKET_ERROR) {
		std::cerr << "socketServer: err: call=start.listen(); code=" << bind_result << std::endl;
		return;
	}

	std::cout << "socketServer: started. port=" << m_Port << "\n";

	for (;;) {
		// accept
		int client_socket
			= accept(m_SocketId, NULL, NULL);

		if (client_socket == INVALID_SOCKET) {
			std::cerr << "socketServer: err: call=start.accept(); code=" << client_socket << std::endl;
			return;
		}

		int result
			= recv(m_SocketId, m_clientInfo.buffer, CLIENT_MSG_BUFFER_LEN, 0);

		if (result < 0) {
			std::cerr << "socketServer: err: call=start.recv(); code=" << result << std::endl;
		}
		else if (result == 0) {
			std::cerr << "socketServer: 'client close connection' err: call=start.recv(); code=" << result << std::endl;
		}
		else {
			// result = length of client message
			m_clientInfo.buffer[result] = '\0';

			std::cout << "socketServer: msg received: " << m_clientInfo.buffer << std::endl;
		}

		std::string response = "response from socket";

		result = send(m_SocketId, response.c_str(), response.size(), 0);

		if (result == SOCKET_ERROR) {
			std::cerr << "socketServer: err: call=start.send(); code=" << result << std::endl;
		}

		// close client connection
		closesocket(client_socket);
	}
}

socketServer::~socketServer() {
	freeaddrinfo(m_AddrInfo);
	closesocket(m_SocketId);
}