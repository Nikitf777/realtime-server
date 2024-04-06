#include "ClientSocket.h"

//#define DEBUG
using std::optional;

void ClientSocket::getSpawned(ByteStream& stream)
{
	optional<Spawned> event;
	stream >> event;
	if (!event)
		return;

	if (spawnedEvent != nullptr)
		spawnedEvent({ _id, event.value()});
}

void ClientSocket::getMoved(ByteStream& stream)
{
	optional<Moved> event;
	stream >> event;
	if (!event)
		return;

	if (movedEvent != nullptr)
		movedEvent({ _id, event.value()});
}

void ClientSocket::getRotated(ByteStream& stream)
{
	optional<Rotated> event;
	stream >> event;
	if (!event)
		return;

	if (rotatedEvent != nullptr)
		rotatedEvent({_id, event.value()});
}
void ClientSocket::getShot(ByteStream& stream)
{
	optional<Shot> event;
	stream >> event;
	if (!event)
		return;

		if (shotEvent != nullptr)
			shotEvent({ _id, event.value() });
}
void ClientSocket::getEnemyKilled(ByteStream& stream)
{
	optional<EnemyKilled> event;
	stream >> event;
	if (!event)
		return;
	if (enemyKilledEvent != nullptr)
		enemyKilledEvent({ _id, event.value()});
}
void ClientSocket::getBulletSpawned(ByteStream& stream)
{
	optional<BulletSpawned> event;
	stream >> event;
	if (!event)
		return;


	if (bulletSpawnedEvent != nullptr)
		bulletSpawnedEvent({ _id, event.value() });
}
void ClientSocket::getBulletMoved(ByteStream& stream)
{
	optional<BulletMoved> event;
	stream >> event;
	if (!event)
		return;


	if (bulletMoved != nullptr)
		bulletMoved({ _id, event.value() });
}
void ClientSocket::getBulletCollided(ByteStream& stream)
{
	optional<BulletCollided> event;
	stream >> event;
	if (!event)
		return;


	if (bulletCollidedEvent != nullptr)
		bulletCollidedEvent({ _id, event.value() });
}
void ClientSocket::getBulletDissapeared(ByteStream& stream)
{
	optional<BulletDissapeared> event;
	stream >> event;
	if (!event)
		return;


	if (bulletDissapearedEvent != nullptr)
		bulletDissapearedEvent({ _id, event.value() });
}
void ClientSocket::emitSignals(PackageFromPlayer& package)
{
	if (package.spawned.hasValue && spawnedEvent) {
		spawnedEvent({ _id, package.spawned.value });
	}

	if (package.moved.hasValue && movedEvent) {
		movedEvent({ _id, package.moved.value });
	}

	if (package.rotated.hasValue && rotatedEvent) {
		rotatedEvent({ _id, package.rotated.value });
	}

	if (package.shot.hasValue && shotEvent) {
		shotEvent({ _id, package.shot.value });
	}

	if (package.enemyKilled.hasValue && enemyKilledEvent) {
		enemyKilledEvent({ _id, package.enemyKilled.value });
	}

	if (package.bulletSpawned.hasValue && bulletSpawnedEvent) {
		bulletSpawnedEvent({ _id, package.bulletSpawned.value });
	}

	if (package.bulletCollided.hasValue && bulletCollidedEvent) {
		bulletCollidedEvent({ _id, package.bulletCollided.value });
	}

	if (package.bulletDissapeared.hasValue && bulletDissapearedEvent) {
		bulletDissapearedEvent({ _id, package.bulletDissapeared.value });
	}
}

void ClientSocket::printAligned(char* buffer, size_t size, unsigned char alignment)
{
	size_t rowCount = size / alignment;
	for (size_t i = 0; i < rowCount; i++) {
		std::cout << (unsigned int)(i * alignment) << ":\t";
		for (char j = 0; j < alignment; j++) {
			std::cout << (unsigned int)(unsigned char)buffer[i * alignment + j] << '\t';
		}
		std::cout << std::endl;
	}
}

ClientSocket::ClientSocket(clserv::TcpSocket socket)
{
	_socket = socket;
	_stop = true;
}

void ClientSocket::authorize(byte id)
{
	_id = id;
	char buffer[1] = { _id };
	send(buffer, sizeof(buffer));
	char nameBuffer[15];
	char nameLength = _socket.receive(nameBuffer, sizeof(nameBuffer));
	std::array<char, 15> name{};
	for (char i = 0; i < nameLength; i++)
		_name[i] = nameBuffer[i];
	Player<Authorized> event{ _id, name };
	if (connected != nullptr)
		connected(event);

	_connected = true;

	std::thread([this] {
		Receiving();
		}).detach();
}

void ClientSocket::Receiving()
{
	while (true)
	{
		auto startTime = std::chrono::high_resolution_clock::now();
		char stopBuffer[1] = { 1 };
		send((char*)stopBuffer, 1);
		char buffer[sizeof PackageFromPlayer];
		auto receivedCount = recv((SOCKET)_socket, buffer, sizeof(buffer), 0);
		ByteStream stream((unsigned char*)buffer, receivedCount);

		//getSpawned(stream);
		//getMoved(stream);
		//getRotated(stream);
		//getShot(stream);
		//getEnemyKilled(stream);
		//getBulletSpawned(stream);
		//getBulletMoved(stream);
		//getBulletCollided(stream);
		//getBulletDissapeared(stream);

#ifdef DEBUG
		std::cout << "Received " << receivedCount << " bytes\n";
		char xarr[] = { buffer[16], buffer[17], buffer[18], buffer[19] };
		float x = *(float*)xarr;
		std::cout << "24 byte = " << (bool)buffer[24] << std::endl;
		std::cout << "moved.position.x (from buffer) = " << x << std::endl;
		
#endif // !DEBUG

		auto package = stream.read<PackageFromPlayer>();
		emitSignals(package);
		std::cout << package.toString();

		auto endTime = std::chrono::high_resolution_clock::now();
		double delta = std::chrono::duration<double, std::micro>(endTime - startTime).count();
		const float tickRate = 0.5;
		const int sec_microsec = 1000000;
		int period = std::micro::den / tickRate;
		if (delta < period)
			std::this_thread::sleep_for(std::chrono::microseconds(period - (long long)delta));
	}
}

void ClientSocket::stop()
{
	_stop = true;
}

void ClientSocket::send(const char* buffer, int size)
{
	_mutex.lock();
	try
	{
		std::cout << _socket.send(buffer, size) << std::endl;
	}
	catch (const std::exception&)
	{
		std::cout << "Player " << _id << " disconnected\n";
	}
	_mutex.unlock();
}

bool ClientSocket::isConnected() const
{
	return _connected;
}
