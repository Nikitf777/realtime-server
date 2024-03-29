#pragma once
#include <array>
#include "PackageStructs.h"
#include "Include.h"
#include "SafeQueue.h"
#include "SafeList.h"
#include "../ByteStream/ByteStream.h"

class ServerPackageManager
{
private:
	SafeList<Player<Authorized>> _allConnectedClients;
	SafeList<std::pair<byte, Spawned>> _allSpawnedClients;
	SafeList<std::pair<byte, std::pair<byte, Transform>>> _allSpawnedBullets;

	SafeQueue<Player<Authorized>> _connectedClients;
	SafeQueue<std::pair<byte, Spawned>> _spawnedClients;
	SafeQueue<std::pair<byte, Moved>> _movedClients;
	SafeQueue<std::pair<byte, float>> _rotatedClients;
	SafeQueue<byte> _shotClients;
	SafeQueue<std::pair<byte, byte>> _enemyKilledClients;
	SafeQueue<std::pair<byte, byte>> _bulletSpawnedClients;
	SafeQueue<std::pair<byte, BulletMoved>> _bulletMovedClients;
	SafeQueue<std::pair<byte, byte>> _bulletCollidedClients;
	SafeQueue<std::pair<byte, byte>> _bulletDissapearedClients;

	SafeQueue<byte> _disconnectedClients;

public:

	void onClientAuthorized(Player<Authorized> event);
	void onClientSpawned(byte id, Spawned spawned);
	void onClientMoved(byte id, Moved moved);
	void onClientRotated(byte id, float rotation);
	void onClientShot(byte id);
	void onClientEnemyKilled(byte id, byte enemyId);
	void onClientBulletSpawned(byte id, byte bulletId);
	void onClientBulletMoved(byte id, BulletMoved bulletSpawned);
	void onClientBulletCollided(byte id, byte bulletId);
	void onClientBulletDissapeared(byte id, byte bulletId);

	void onClientDisconnected(byte id);

	//ByteStream getAllConnectedPlayers();
	//ByteStream getAllSpawnedPlayers();
	//ByteStream getAllSpawnedBullets();
	ByteStream getInitialWorldStateStream();
	ByteStream getEventsStream();
};

