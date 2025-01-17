#pragma once
#include "TcpListenerAsync.h"
#include "ClientSocket.h"
#include "AutoMap.h"
#include "ServerPackageManager.h"
#include <list>

struct Player {
	Transform transform;
};

struct Client {
	Player player;
	ClientSocket socket;
};

class RealTimeServer : clserv::TcpListenerAsync
{
public:
	struct DataPackage {
		float x, y;
	};
	
private:
	AutoMap<ClientSocket*> _clients;
	SafeQueue<std::function<ClientSocket*()>> _connectActions;
	std::vector<DataPackage> _dataToSend;
	const uint8_t _serverCapacity;
	ServerPackageManager _manager;

	void onClientDisconnected(PlayerDisconnected event);

public:
	RealTimeServer(int port, uint8_t serverCapacity);

	void listen() override;
	void mainLoop();
};

