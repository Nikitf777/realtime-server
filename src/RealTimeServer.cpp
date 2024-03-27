#include "RealTimeServer.h"
using namespace clserv;

void RealTimeServer::onClientDisconnected(byte id)
{
	_clients.remove(id);
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
	_socket.listen();
	while (_isListening)
	{
		TcpSocket clientSocket = _socket.accept();
		ClientSocket* newClient = new ClientSocket(clientSocket);

		_connectActions.safeEnqueue([this, newClient]() {
			size_t newId = _clients.add(newClient);
			newClient->authorize(newId);
			return newClient;
			});

		newClient->connected = [this](byte id, std::array<char, 15> name) {
			_manager.onClientConnected(id, name); };
		//newClient->spawned = [this](byte id, Spawned spawned) {
		//	_manager.onClientSpawned(id, spawned); };
		//newClient->moved = [this](byte id, Moved moved) {
		//	_manager.onClientMoved(id, moved); };
		//newClient->rotated = [this](byte id, float rotation) {
		//	_manager.onClientRotated(id, rotation); };
		//newClient->shot = [this](byte id) {
		//	_manager.onClientShot(id); };
		//newClient->enemyKilled = [this](byte id, char enemyId) {
		//	_manager.onClientEnemyKilled(id, enemyId); };
		//newClient->bulletSpawned = [this](byte id, byte bulletId) {
		//	_manager.onClientBulletSpawned(id, bulletId); };
		//newClient->bulletMoved = [this](byte id, BulletMoved bulletMoved) {
		//	_manager.onClientBulletMoved(id, bulletMoved); };
		//newClient->bulletCollided = [this](byte id, char bulletId) {
		//	_manager.onClientBulletCollided(id, bulletId); };
		//newClient->bulletDissapeared = [this](byte id, char bulletDissapeared) {
		//	_manager.onClientBulletDissapeared(id, bulletDissapeared); };

		newClient->disconnected = [this](byte id) {
			onClientDisconnected(id);
			_manager.onClientDisconnected(id);
			};

	}
}

void RealTimeServer::mainLoop()
{
	while (true)
	{
		//const clock_t begin_time = clock();
		//std::cout << float(clock() - begin_time) / CLOCKS_PER_SEC;

		auto startTime = std::chrono::high_resolution_clock::now();
		if (_connectActions.size() > 0)
		{
			ByteStream stream = _manager.getAllConnectedPlayers();
			auto message = stream.getVector();
				for (int i = 0; i < _connectActions.size(); i++) {
					ClientSocket* client = _connectActions.safeDequeue()();
					std::cout
						<< "mailLoop; "
						<< "_allConnectedPlayers.size() = "
						<< (int)message[0]
						<< "; message.size() = "
						<< (int)message.size()
						<< std::endl;
					char size;
					if (message[0] == 3)
						size = message[0];
					size = message[0];
					for (int j = 0; j < size; j++) {
						int index = 16 * j + 1;
						std::cout << "sended id " << (int)message[index] << std::endl;
					}
					client->send(message);
				}
		}

		ByteStream stream = _manager.getByteStream();
		std::vector<char> message = stream.getVector();
		//unsigned char streamBytes[1024];
		//auto length = stream.getBuf(streamBytes);
		//

		//message.resize(length);

		//for (unsigned short i = 0; i < length; i++)
		//	message[i] = streamBytes[i];

		auto& clients = _clients.getMap();
		for (auto& client : clients) {
			if (client.second->isConnected())
				client.second->send(message);
		}

		auto endTime = std::chrono::high_resolution_clock::now();
		double delta = std::chrono::duration<double, std::milli>(endTime - startTime).count();
		const int tickRate = 100;
		if (delta < tickRate)
			std::this_thread::sleep_for(std::chrono::microseconds(tickRate * 1000 - (long long)(delta * 1000)));
	}
}
