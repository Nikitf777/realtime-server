#define DEBUG

#include "RealTimeServer.h"

using namespace clserv;

void RealTimeServer::onClientDisconnected(PlayerDisconnected event)
{
	_clients.remove(event.id);
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

		newClient->connected = [this](Player<Authorized> event) {
			_manager.onClientAuthorized(event); };
		newClient->spawnedEvent = [this](Player<Spawned> event) {
			_manager.onClientSpawned(event); };
		newClient->movedEvent = [this](Player<Moved> event) {
			_manager.onClientMoved(event); };
		newClient->rotatedEvent = [this](Player<Rotated> event) {
			_manager.onClientRotated(event); };
		newClient->shotEvent = [this](Player<Shot> event) {
			_manager.onClientShot(event); };
		newClient->enemyKilledEvent = [this](Player<EnemyKilled> event) {
			_manager.onClientEnemyKilled(event); };
		newClient->bulletSpawnedEvent = [this](Player<BulletSpawned>  event) {
			_manager.onClientBulletSpawned(event); };
		newClient->bulletMoved = [this](Player<BulletMoved> event) {
			_manager.onClientBulletMoved(event); };
		newClient->bulletCollidedEvent = [this](Player<BulletCollided> event) {
			_manager.onClientBulletCollided(event); };
		newClient->bulletDissapearedEvent = [this](Player<BulletDissapeared> event) {
			_manager.onClientBulletDissapeared(event); };

		newClient->disconnected = [this](PlayerDisconnected event) {
			onClientDisconnected(event);
			_manager.onClientDisconnected(event);
			};

	}
}

void RealTimeServer::mainLoop()
{
	while (true)
	{
		auto startTime = std::chrono::high_resolution_clock::now();
		if (_connectActions.size() > 0)
		{
			ByteStream stream = _manager.getInitialWorldStateStream();

			unsigned char* message = new unsigned char[stream.getLength()];
			size_t size = stream.getLength();
			stream.getBuf(message);
				for (int i = 0; i < _connectActions.size(); i++) {
					ClientSocket* client = _connectActions.safeDequeue()();
#ifdef DEBUG
					std::cout
						<< "mailLoop; "
						<< "_allConnectedPlayers.size() = "
						<< (int)message[0]
						<< "; message.size() = "
						<< (int)size
						<< std::endl;
					//char size;
					//if (message[0] == 3)
					//	size = message[0];
					//size = message[0];
					//for (int j = 0; j < size; j++) {
					//	int index = 16 * j + 1;
					//	std::cout << "sended id " << (int)message[index] << std::endl;
					//}
#endif // DEBUG

					client->send((char*)message, size);
				}
		}

		ByteStream stream = _manager.getEventsStream();
		
		std::vector<char> message = stream.getVector();
		unsigned char* buffer = new unsigned char[stream.getLength()];
		stream.getBuf(buffer);
		auto& clients = _clients.getMap();
		for (auto& client : clients) {
			if (client.second->isConnected()) {
				client.second->send((char*)buffer, stream.getLength());
			}
		}

		auto endTime = std::chrono::high_resolution_clock::now();
		double delta = std::chrono::duration<double, std::micro>(endTime - startTime).count();
		const float tickRate = 0.5;
		const int sec_microsec = 1000000;
		int period = std::micro::den / tickRate;
		if (delta < period)
			std::this_thread::sleep_for(std::chrono::microseconds(period - (long long)delta));
	}
}
