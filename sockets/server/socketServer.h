
#include <winsock2.h>
#include <ws2tcpip.h>

#define CLIENT_MSG_BUFFER_LEN 1024

class socketServer {
private:
	const char* m_Port;
	int m_SocketId;
	unsigned int m_maxConnections;
	addrinfo* m_AddrInfo;

	struct clientInfo {
		char buffer[CLIENT_MSG_BUFFER_LEN];
	} m_clientInfo;

public:
	socketServer(const char* ipAddr, const char* port, unsigned int max_connections);
	~socketServer();
	void start(); // run server on port, while(__flag__)

};