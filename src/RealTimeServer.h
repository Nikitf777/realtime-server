#pragma once
#include "TcpListenerAsync.h"
#include "ClientSocket.h"
#include "AutoMap.h"

class RealTimeServer : clserv::TcpListenerAsync
{
public:
	struct DataPackage {
		float x, y;
	};
	
private:
	AutoMap<ClientSocket*> _clients;
	std::vector<DataPackage> _dataToSend;
	const uint8_t _serverCapacity;

	void onClientConnected(
		clserv::TcpListenerAsync::NextAction* nextAction);
	void onMessageReceived(
		clserv::TcpListenerAsync::NextAction* nextAction, std::string message);
	void onMessageSended(
		clserv::TcpListenerAsync::NextAction* nextAction, std::string message);

	void handleClient(clserv::TcpSocket client);

public:
	RealTimeServer(int port, uint8_t serverCapacity);

	void listen() override;
	void mainLoop();
};

