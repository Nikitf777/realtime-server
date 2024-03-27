#include "ServerPackageManager.h"
#define DEBUG

void ServerPackageManager::onClientConnected(byte id, std::array<char, 15> name)
{
#ifdef DEBUG
	std::cout << "onClientConnected\t" << name.data() << std::endl;
#endif // DEBUG
	auto player = std::pair(id, name);
	_connectedClients.safeEnqueue(player);
	_allConnectedClients.safePushBack(player);
}

void ServerPackageManager::onClientSpawned(byte id, Spawned spawned)
{
	_spawnedClients.safeEnqueue(std::pair(id, spawned));
}

void ServerPackageManager::onClientMoved(byte id, Moved moved)
{
	_movedClients.safeEnqueue(std::pair(id, moved));
}

void ServerPackageManager::onClientRotated(byte id, float rotation)
{
	_rotatedClients.safeEnqueue(std::pair(id, rotation));
}

void ServerPackageManager::onClientShot(byte id)
{
	_shotClients.safeEnqueue(id);
}

void ServerPackageManager::onClientEnemyKilled(byte id, byte enemyId)
{
	_enemyKilledClients.safeEnqueue(std::pair(id, enemyId));
}

void ServerPackageManager::onClientBulletSpawned(byte id, byte bulletId)
{
	_bulletSpawnedClients.safeEnqueue(std::pair(id, bulletId));
}

void ServerPackageManager::onClientBulletMoved(byte id, BulletMoved bulletSpawned)
{
	_bulletMovedClients.safeEnqueue(std::pair(id, bulletSpawned));
}

void ServerPackageManager::onClientBulletCollided(byte id, byte bulletId)
{
	_bulletCollidedClients.safeEnqueue(std::pair(id, bulletId));
}

void ServerPackageManager::onClientBulletDissapeared(byte id, byte bulletId)
{
	_bulletDissapearedClients.safeEnqueue(std::pair(id, bulletId));
}

void ServerPackageManager::onClientDisconnected(byte id)
{
	_disconnectedClients.safeEnqueue(id);
}

ByteStream ServerPackageManager::getAllConnectedPlayers()
{
	ByteStream stream(512);
	stream << (byte)_allConnectedClients.size();

	//for (auto client : _allConnectedClients) {
	//	stream;
	//}
	std::cout << "getAllConnectedPlayers; size = " << (int)_allConnectedClients.size() << std::endl;
	for (char i = 0; i < _allConnectedClients.size(); i++) {
		auto& pack = _allConnectedClients[i];
		auto name = pack.second.data();
		stream << pack.first;
		//std::cout << "getAllConnectedPlayers; sended id " << (int)pack.first << std::endl;
		for (char letter : pack.second)
			stream << letter;
	}
	return stream;
}

ByteStream ServerPackageManager::getByteStream()
{
	ByteStream stream(1024);
	//std::cout << "Queue count: " << _connectedClients.size() << std::endl;
	stream << (byte)_connectedClients.size();
	for (char i = 0; i < _connectedClients.size(); i++) {
		auto pack = _connectedClients.safeDequeue();
		//byte bytes[sizeof std::pair<byte, std::array<char, 15>>];
		//memcpy(bytes, &pack, sizeof bytes);
		//stream << bytes;
		auto name = pack.second.data();
		stream << pack.first/* << stream(pack.second.data(), pack.second.size())*/;
		for (char letter : pack.second)
			stream << letter;
	}

	//stream << (byte)_spawnedClients.size();
	//for (char i = 0; i < _spawnedClients.size(); i++) {
	//	auto pack = _spawnedClients.safeDequeue();
	//	stream << pack.first << pack.second.x << pack.second.y;
	//	std::cout << "spawned id = " << pack.first << std::endl;
	//}

	//stream << (byte)_movedClients.size();
	//for (char i = 0; i < _movedClients.size(); i++) {
	//	auto pack = _movedClients.dequeue();
	//	stream << pack.first << pack.second.x << pack.second.y;
	//}

	//stream << (byte)_rotatedClients.size();
	//for (char i = 0; i < _rotatedClients.size(); i++) {
	//	auto pack = _rotatedClients.dequeue();
	//	stream << pack.first << pack.second;
	//}

	//stream << (byte)_shotClients.size();
	//for (char i = 0; i < _shotClients.size(); i++) {
	//	auto pack = _shotClients.dequeue();
	//	stream << pack;
	//}

	//stream << (byte)_enemyKilledClients.size();
	//for (char i = 0; i < _enemyKilledClients.size(); i++) {
	//	auto pack = _enemyKilledClients.dequeue();
	//	stream << pack.first << pack.second;
	//}

	//stream << (byte)_bulletSpawnedClients.size();
	//for (char i = 0; i < _bulletSpawnedClients.size(); i++) {
	//	auto pack = _bulletSpawnedClients.dequeue();
	//	stream << pack.first << pack.second;
	//}

	//stream << (byte)_bulletMovedClients.size();
	//for (char i = 0; i < _bulletMovedClients.size(); i++) {
	//	auto pack = _bulletMovedClients.dequeue();
	//	stream << pack.first << pack.second.bulletId << pack.second.x << pack.second.y;
	//}

	//stream << (byte)_bulletCollidedClients.size();
	//for (char i = 0; i < _bulletCollidedClients.size(); i++){
	//	auto pack = _bulletCollidedClients.dequeue();
	//	stream << pack.first << pack.second;
	//}

	//stream << (byte)_bulletDissapearedClients.size();
	//for (char i = 0; i < _bulletDissapearedClients.size(); i++){
	//	auto pack = _bulletDissapearedClients.dequeue();
	//	stream << pack.first << pack.second;
	//}

	return stream;
}
