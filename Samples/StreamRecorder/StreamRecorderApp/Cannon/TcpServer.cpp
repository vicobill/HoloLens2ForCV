#include "TcpServer.h"
#include <thread>
#include <mutex>
#include <WinSock2.h>

#pragma comment(lib,"ws2_32.lib")

TcpServer::TcpServer() :
	_isReady(false),
	_sock(INVALID_SOCKET)
{
	
}

TcpServer::~TcpServer() {
	Shutdown();
}
void TcpServer::Init(int port) {
	WSADATA data;
	int err = WSAStartup(MAKEWORD(2, 2), &data);
	if (err != 0) {
		return;
	}
	createSocket(port);
}

void TcpServer::Shutdown() {
	if (_sock != INVALID_SOCKET) {
		closesocket(_sock);
	}
	WSACleanup();
}



void TcpServer::createSocket(int port) {
	_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_sock == INVALID_SOCKET) {
		return;
	}

	sockaddr_in addr;
	addr.sin_port = port;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int err = bind(_sock, (sockaddr*)&addr, sizeof(addr));
	if (err == SOCKET_ERROR) {
		return;
	}

	err = listen(_sock, SOMAXCONN_HINT(5));
	if (err == SOCKET_ERROR) {
		return;
	}

	_isReady = true;
}

	static std::mutex mtx;
void TcpServer::Start() {
	_acceptClientThread  = new 
		std::thread([&]() {
		while (true) {
			sockaddr_in addr;
			int addrlen = 0;
			int clientsock = accept(_sock, (sockaddr*)&addr, &addrlen);
			if (clientsock != INVALID_SOCKET) {
				std::lock_guard<std::mutex> lock(mtx);

				_clientSockets.push_back(clientsock);
			}
		}});
	//acceptThread.
}

void TcpServer::Stop() {
	if (_acceptClientThread != nullptr) {
		_acceptClientThread->join();
		delete _acceptClientThread;
	}
	
}
void TcpServer::Send(const uint8_t* data, size_t len) {
	std::lock_guard<std::mutex> lock(mtx);
	for (auto cl : _clientSockets) {
		send(cl, (const char*)data, len, 0);
	}
}
