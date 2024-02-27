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

void RealTimeServer::mainLoop()
{
	std::vector<DataPackage> packagesToSend;
	packagesToSend.reserve(_serverCapacity);
	std::chrono::steady_clock::time_point time;
	while (true)
	{
		time = std::chrono::steady_clock::now();

		// some work
		for (auto client : _clients) {
			packagesToSend.push_back(client->_receivedPackages.back());
		}

	}
}

RealTimeServer::Client::Client(clserv::TcpSocket socket)
{
	_stop = true;
	std::thread([socket, this] {
		while (true)
		{
			DataPackage package;
			//char buffer[512];
			char buffer[8];
			//socket.receive(buffer, sizeof(buffer));
			recv((SOCKET)socket, buffer, sizeof(buffer), 0);
			//std::cout << buffer << '\n';
			float x, y;
			char xBytes[4] = {buffer[0], buffer[1], buffer[2], buffer[3]};
			char yBytes[4] = {buffer[4], buffer[5], buffer[6], buffer[7]};
			//memcpy(&x, &xBytes, sizeof(x));
			//memcpy(&y, &yBytes, sizeof(y));
			memcpy(&package, &buffer, sizeof(package));
			std::cout << '(' << package.x << ',' << package.y << ')' << '\n';
			//_receivedPackages.push(package);
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
