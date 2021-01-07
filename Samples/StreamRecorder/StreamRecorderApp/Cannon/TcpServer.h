#pragma once
#include <list>
#include <thread>

class TcpServer {
public:
	TcpServer();
	~TcpServer();

	void Init(int listenPort = 9527);
	void Shutdown();

	void Start();
	void Stop();

	bool IsReady() const { return _isReady; }
	void Send(const uint8_t* data, size_t len);

private:
	void createSocket(int port);

	int _sock;
	bool _isReady;
	
	std::list<int> _clientSockets;
	std::thread* _acceptClientThread;
};