
class socketClient {
private:
	int m_SocketId;
	const char* m_Port;
public:
	void close();
	void sendMsg(const char* msg);
	bool sockConnect();
	socketClient(const char* port);
	~socketClient();
};