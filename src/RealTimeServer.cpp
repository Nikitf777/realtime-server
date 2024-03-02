#include "RealTimeServer.h"
using namespace clserv;
using namespace std;

void RealTimeServer::onClientConnected(clserv::TcpListenerAsync::NextAction* nextAction)
{
	std::cout << socket;
	nextAction->send("hello");
}

void RealTimeServer::onMessageReceived(clserv::TcpListenerAsync::NextAction* nextAction, std::string message)
{
	std::cout << message;
	nextAction->send("hello2");
}

void RealTimeServer::onMessageSended(clserv::TcpListenerAsync::NextAction* nextAction, std::string message)
{
	nextAction->receive();
}

void RealTimeServer::handleClient(clserv::TcpSocket client)
{
	TcpListenerAsync::NextAction* nextAction = new TcpListenerAsync::NextAction(this, client);
	onClientConnected(nextAction);
	while (true)
	{
		nextAction->next();
	}
}

RealTimeServer::RealTimeServer(int port, uint8_t serverCapacity) :
	TcpListenerAsync(port),
	_serverCapacity(serverCapacity)
{
	//_clients.reserve(_serverCapacity);
}

void RealTimeServer::listen()
{
	_isListening = true;
	while (_isListening)
	{
		_socket.listen();
		TcpSocket clientSocket = _socket.accept();
		ClientSocket* newClient = new ClientSocket(clientSocket);
		size_t newId = _clients.add(newClient);
		newClient->connect(newId);
	}
}

void RealTimeServer::mainLoop()
{
	std::vector<DataPackage> packagesToSend;
	packagesToSend.reserve(_serverCapacity);
	std::chrono::steady_clock::time_point time;
	while (true)
	{
		time = std::chrono::steady_clock::now();

		// some work
		for (auto client : _clients.getMap()) {
			//packagesToSend.push_back(client->_receivedPackages.back());
		}

	}
}
