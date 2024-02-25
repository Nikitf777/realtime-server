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
	_clients.reserve(_serverCapacity);
}

void RealTimeServer::listen()
{
	_isListening = true;
	while (_isListening)
	{
		_socket.listen();
		TcpSocket clientSocket = _socket.accept();
		_clients.push_back(new Client(clientSocket));
	}
}

RealTimeServer::Client::Client(clserv::TcpSocket socket)
{
	_stop = true;
	std::thread([socket, this] {
		while (true)
		{
			DataPackage package;
			socket.receiveString(512);
			_receivedPackages.push(package);
		}
		}).detach();

	std::thread([socket, this] {
		while (true)
		{
			while (_stop)
			{
				std::this_thread::yield();
			}
			socket.send("hello");
			_stop = true;
		}
		}).detach();
}

void RealTimeServer::Client::stop()
{
	_stop = true;
}

void RealTimeServer::Client::send(DataPackage data)
{
	_dataToSend = data;
	_stop = false;
}