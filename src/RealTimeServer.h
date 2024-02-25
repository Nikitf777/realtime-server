#pragma once
#include "TcpListenerAsync.h"

class RealTimeServer : clserv::TcpListenerAsync
{
public:
	struct DataPackage {

	};

	class NextActionQueue : TcpListenerAsync::NextAction {
		std::queue<DataPackage>* _packages;
	};

	class Client {
		std::queue<DataPackage> _receivedPackages;
		std::atomic<bool> _stop;
		DataPackage _dataToSend;

	public:
		Client(clserv::TcpSocket socket);

		void stop();
		void send(DataPackage data);
	};
	
private:
	std::vector<Client*> _clients;
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

