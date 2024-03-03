#include "ServerPackageManager.h"

void ServerPackageManager::onClientConnected(byte id, std::array<char, 19> name)
{
	_connectedClients.enqueue(std::pair(id, name));
}

void ServerPackageManager::onClientSpawned(byte id, Spawned spawned)
{
	_spawnedClients.enqueue(std::pair(id, spawned));
}

void ServerPackageManager::onClientMoved(byte id, Moved moved)
{
	_movedClients.enqueue(std::pair(id, moved));
}

void ServerPackageManager::onClientRotated(byte id, float rotation)
{
	_rotatedClients.enqueue(std::pair(id, rotation));
}

void ServerPackageManager::onClientShot(byte id)
{
	_shotClients.enqueue(id);
}

void ServerPackageManager::onClientEnemyKilled(byte id, byte enemyId)
{
	_enemyKilledClients.enqueue(std::pair(id, enemyId));
}

void ServerPackageManager::onClientBulletSpawned(byte id, BulletSpawned bulletSpawned)
{
	_bulletSpawnedClients.enqueue(std::pair(id, bulletSpawned));
}

void ServerPackageManager::onClientBulletMoved(byte id, BulletMoved bulletSpawned)
{
	_bulletMovedClients.enqueue(std::pair(id, bulletSpawned));
}

void ServerPackageManager::onClientBulletCollided(byte id, byte bulletId)
{
	_bulletCollidedClients.enqueue(std::pair(id, bulletId));
}

void ServerPackageManager::onClientBulletDissapeared(byte id, byte bulletId)
{
	_bulletDissapearedClients.enqueue(std::pair(id, bulletId));
}

ByteStream ServerPackageManager::getByteStream()
{
	ByteStream s(1024);

	s << float(4) << float(8) << float(1);
	std::cout << s.getLength();

	s << (byte)_connectedClients.size();
	for (char i = 0; i < _connectedClients.size(); i++) {
		auto pack = _connectedClients.dequeue();
		auto name = pack.second.data();
		s << pack.first << name;
	}

	s << (byte)_spawnedClients.size();
	for (char i = 0; i < _spawnedClients.size(); i++) {
		auto pack = _spawnedClients.dequeue();
		s << pack.first << pack.second.x << pack.second.y;
	}

	s << (byte)_movedClients.size();
	for (char i = 0; i < _movedClients.size(); i++) {
		auto pack = _movedClients.dequeue();
		s << pack.first << pack.second.x << pack.second.y;
	}

	s << (byte)_rotatedClients.size();
	for (char i = 0; i < _rotatedClients.size(); i++) {
		auto pack = _rotatedClients.dequeue();
		s << pack.first << pack.second;
		s << 'C' << 'b';
		std::cout << "first = " << pack.first << " second = " << pack.second << " length = " << s.getLength() << std::endl;
	}

	s << (byte)_shotClients.size();
	for (char i = 0; i < _shotClients.size(); i++) {
		auto pack = _shotClients.dequeue();
		s << pack;
	}

	s << (byte)_enemyKilledClients.size();
	for (char i = 0; i < _enemyKilledClients.size(); i++) {
		auto pack = _enemyKilledClients.dequeue();
		s << pack.first << pack.second;
	}

	s << (byte)_bulletSpawnedClients.size();
	for (char i = 0; i < _bulletSpawnedClients.size(); i++) {
		auto pack = _bulletSpawnedClients.dequeue();
		s << pack.first << pack.second.bulletId << pack.second.rotation;
	}

	s << (byte)_bulletMovedClients.size();
	for (char i = 0; i < _bulletMovedClients.size(); i++) {
		auto pack = _bulletMovedClients.dequeue();
		s << pack.first << pack.second.bulletId << pack.second.x << pack.second.y;
	}

	s << (byte)_bulletCollidedClients.size();
	for (char i = 0; i < _bulletCollidedClients.size(); i++){
		auto pack = _bulletCollidedClients.dequeue();
		s << pack.first << pack.second;
	}

	s << (byte)_bulletDissapearedClients.size();
	for (char i = 0; i < _bulletDissapearedClients.size(); i++){
		auto pack = _bulletDissapearedClients.dequeue();
		s << pack.first << pack.second;
	}
	//if (s.getLength() > 0)
	//std::cout << "Length = " << s.getLength() << std::endl;
	return s;
}
