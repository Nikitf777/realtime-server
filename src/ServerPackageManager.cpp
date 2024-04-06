#include "ServerPackageManager.h"
#define DEBUG

void ServerPackageManager::onClientAuthorized(Player<Authorized> event)
{
#ifdef DEBUG
	std::cout << "onClientConnected\t" << event.package.name.data() << std::endl;
#endif // DEBUG
	_authorizedClients.safeEnqueue(event);
	_allConnectedClients.safePushBack(event);
}

void ServerPackageManager::onClientSpawned(Player<Spawned> event)
{
#ifdef DEBUG
	std::cout << "onClientSpawned\t" << (int)event.id << std::endl;
#endif // DEBUG
	_spawnedClients.safeEnqueue(event);
	_allSpawnedClients.safePushBack(event);
}

void ServerPackageManager::onClientMoved(Player<Moved> event)
{
	_movedClients.safeEnqueue(event);
}

void ServerPackageManager::onClientRotated(Player<Rotated> event)
{
	_rotatedClients.safeEnqueue(event);
}

void ServerPackageManager::onClientShot(Player<Shot> event)
{
	_shotClients.safeEnqueue(event);
}

void ServerPackageManager::onClientEnemyKilled(Player<EnemyKilled> event)
{
	_enemyKilledClients.safeEnqueue(event);
}

void ServerPackageManager::onClientBulletSpawned(Player<BulletSpawned> event)
{
	_bulletSpawnedClients.safeEnqueue(event);
	_allSpawnedBullets.safePushBack(event);
}

void ServerPackageManager::onClientBulletMoved(Player<BulletMoved> event)
{
	_bulletMovedClients.safeEnqueue(event);
}

void ServerPackageManager::onClientBulletCollided(Player<BulletCollided> event)
{
	_bulletCollidedClients.safeEnqueue(event);
}

void ServerPackageManager::onClientBulletDissapeared(Player<BulletDissapeared> event)
{
	_bulletDissapearedClients.safeEnqueue(event);
}

void ServerPackageManager::onClientDisconnected(PlayerDisconnected event)
{
	_disconnectedClients.safeEnqueue(event);
}

ByteStream ServerPackageManager::getInitialWorldStateStream()
{
	ByteStream stream(512);
	stream << byte(0);
	stream << (byte)_allConnectedClients.size();
	std::cout << "getAllConnectedPlayers; size = " << (int)_allConnectedClients.size() << std::endl;
	for (char i = 0; i < _allConnectedClients.size(); i++) {
		stream << _allConnectedClients[i];
	}

	stream << (byte)_allSpawnedClients.size();
	for (char i = 0; i < _allSpawnedClients.size(); i++) {
		stream << _allConnectedClients[i];
	}

	stream << (byte)_allSpawnedBullets.size();
	for (char i = 0; i < _allSpawnedBullets.size(); i++) {
		stream << _allSpawnedBullets[i];
	}
	return stream;
}

ByteStream ServerPackageManager::getEventsStream()
{
	ByteStream stream(1024);
	stream << byte(0);

	//std::cout << "Queue count: " << _connectedClients.size() << std::endl;
	
	stream << (byte)_authorizedClients.size();
	for (char i = 0; i < _authorizedClients.size(); i++) {
		stream << _authorizedClients.safeDequeue();
	}

	stream << (byte)_spawnedClients.size();
	for (char i = 0; i < _spawnedClients.size(); i++) {
		stream << _spawnedClients.safeDequeue();
	}

	stream << (byte)_movedClients.size();
	for (char i = 0; i < _movedClients.size(); i++) {
		stream << _movedClients.safeDequeue();
	}

	stream << (byte)_rotatedClients.size();
	for (char i = 0; i < _rotatedClients.size(); i++) {
		stream << _rotatedClients.safeDequeue();
	}

	stream << (byte)_shotClients.size();
	for (char i = 0; i < _shotClients.size(); i++) {
		stream << _shotClients.safeDequeue();
	}

	stream << (byte)_enemyKilledClients.size();
	for (char i = 0; i < _enemyKilledClients.size(); i++) {
		stream << _enemyKilledClients.safeDequeue();
	}

	stream << (byte)_bulletSpawnedClients.size();
	for (char i = 0; i < _bulletSpawnedClients.size(); i++) {
		stream << _bulletSpawnedClients.safeDequeue();
	}

	stream << (byte)_bulletMovedClients.size();
	for (char i = 0; i < _bulletMovedClients.size(); i++) {
		stream << _bulletMovedClients.safeDequeue();
	}

	stream << (byte)_bulletCollidedClients.size();
	for (char i = 0; i < _bulletCollidedClients.size(); i++){
		stream << _bulletCollidedClients.safeDequeue();
	}

	stream << (byte)_bulletDissapearedClients.size();
	for (char i = 0; i < _bulletDissapearedClients.size(); i++){
		stream << _bulletDissapearedClients.safeDequeue();
	}

	return stream;
}
