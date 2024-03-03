#include "RealTimeServer.h"
using namespace clserv;

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

		newClient->connected = [this](byte id, std::array<char, 19> name) {
			_manager.onClientConnected(id, name); };
		newClient->spawned = [this](byte id, Spawned spawned) {
			_manager.onClientSpawned(id, spawned); };
		newClient->moved = [this](byte id, Moved moved) {
			_manager.onClientMoved(id, moved); };
		newClient->rotated = [this](byte id, float rotation) {
			_manager.onClientRotated(id, rotation); };
		newClient->shot = [this](byte id) {
			_manager.onClientShot(id); };
		newClient->enemyKilled = [this](byte id, char enemyId) {
			_manager.onClientEnemyKilled(id, enemyId); };
		newClient->bulletSpawned = [this](byte id, BulletSpawned bulletSpawned) {
			_manager.onClientBulletSpawned(id, bulletSpawned); };
		newClient->bulletMoved = [this](byte id, BulletMoved bulletMoved) {
			_manager.onClientBulletMoved(id, bulletMoved); };
		newClient->bulletCollided = [this](byte id, char bulletId) {
			_manager.onClientBulletCollided(id, bulletId); };
		newClient->bulletDissapeared = [this](byte id, char bulletDissapeared) {
			_manager.onClientBulletDissapeared(id, bulletDissapeared); };

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
		//const clock_t begin_time = clock();
		//std::cout << float(clock() - begin_time) / CLOCKS_PER_SEC;

		auto startTime = std::chrono::high_resolution_clock::now();

		ByteStream stream = _manager.getByteStream();
		auto length = stream.getLength();
		//std::cout << "stream length: " << length << '\n';
		unsigned char streamBytes[1024];
		stream.getBuf(streamBytes);

		std::vector<char> message;
		message.reserve(length);

		for (unsigned short i = 0; i < length; i++)
			message[i] = streamBytes[i];

		auto clients = _clients.getMap();
		for (auto& client : clients)
			client.second->send(message);

		auto endTime = std::chrono::high_resolution_clock::now();
		double delta = std::chrono::duration<double, std::milli>(endTime - startTime).count();
		const int tickRate = 100;
		if (delta < tickRate)
			std::this_thread::sleep_for(std::chrono::microseconds(tickRate * 1000 - (long long)(delta * 1000)));
	}
}
