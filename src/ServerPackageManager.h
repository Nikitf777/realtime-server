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
	SafeList<Player<Spawned>> _allSpawnedClients;
	SafeList<Player<BulletSpawned>> _allSpawnedBullets;

	SafeQueue<Player<Authorized>> _authorizedClients;
	SafeQueue<Player<Spawned>> _spawnedClients;
	SafeQueue<Player<Moved>> _movedClients;
	SafeQueue<Player<Rotated>> _rotatedClients;
	SafeQueue<Player<Shot>> _shotClients;
	SafeQueue<Player<EnemyKilled>> _enemyKilledClients;
	SafeQueue<Player<BulletSpawned>> _bulletSpawnedClients;
	SafeQueue<Player<BulletMoved>> _bulletMovedClients;
	SafeQueue<Player<BulletCollided>> _bulletCollidedClients;
	SafeQueue<Player<BulletDissapeared>> _bulletDissapearedClients;
	SafeQueue<PlayerDisconnected> _disconnectedClients;

public:

	void onClientAuthorized(Player<Authorized> event);
	void onClientSpawned(Player<Spawned> event);
	void onClientMoved(Player<Moved> event);
	void onClientRotated(Player<Rotated> event);
	void onClientShot(Player<Shot> event);
	void onClientEnemyKilled(Player<EnemyKilled> event);
	void onClientBulletSpawned(Player<BulletSpawned> event);
	void onClientBulletMoved(Player<BulletMoved> event);
	void onClientBulletCollided(Player<BulletCollided> event);
	void onClientBulletDissapeared(Player<BulletDissapeared> event);
	void onClientDisconnected(PlayerDisconnected event);

	//ByteStream getAllConnectedPlayers();
	//ByteStream getAllSpawnedPlayers();
	//ByteStream getAllSpawnedBullets();
	ByteStream getInitialWorldStateStream();
	ByteStream getEventsStream();
};

