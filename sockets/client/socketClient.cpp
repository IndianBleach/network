#define _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS

#include "socketClient.h"
#include <cstring>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <ws2def.h>
#include <stdio.h>

void socketClient::close() {
	closesocket(m_SocketId);
}

bool socketClient::sockConnect() {

	std::string s = "127.0.0.1";

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	
	inet_pton(AF_INET, s.c_str(), &addr.sin_addr.s_addr);
	addr.sin_port = htons(8080);

	int result = 
		connect(m_SocketId, (sockaddr*)&addr, sizeof(addr));

	if (result == SOCKET_ERROR) {
		std::cerr << "socketClient: err: call=connect(), code=" << result << ", msg=" << WSAGetLastError() << std::endl;
		return false;
	}
	else return true;
}

void socketClient::sendMsg(const char* msg) {
	int len = strlen(msg);
	send(m_SocketId, msg, len, 0);
}

socketClient::socketClient(const char* port) {
	m_SocketId = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	m_Port = port;
}

socketClient::~socketClient() {
	if (m_SocketId > 0) {
		this->close();
	}
}