#pragma once
#include "TcpSocket.h"
#include <optional>
#include <array>
#include "../ByteStream/ByteStream.h"

#include "PackageStructs.h"
#include "PackageFromPlayer.h"

class ClientSocket {
private:
	clserv::TcpSocket _socket;
	std::atomic<bool> _stop;
	std::mutex _mutex;
	byte _id;
	std::array<char, 15> _name;

	bool _connected = false;
	//std::optional<Spawned> _spawned;
	//std::optional<Moved> _moved;
	//std::optional<float> _rotated;
	//bool _shot;
	//std::optional<byte> _enemyKilled;
	//std::optional<byte> _bulletSpawned;
	//std::optional<BulletMoved> _bulletMoved;
	//std::optional<byte> _bulletCollided;
	//std::optional<byte> _bulletDissapeared;

	void getSpawned(ByteStream& stream);
	void getMoved(ByteStream& stream);
	void getRotated(ByteStream& stream);
	void getShot(ByteStream& stream);
	void getEnemyKilled(ByteStream& stream);
	void getBulletSpawned(ByteStream& stream);
	void getBulletMoved(ByteStream& stream);
	void getBulletCollided(ByteStream& stream);
	void getBulletDissapeared(ByteStream& stream);

	void emitSignals(PackageFromPlayer& package);

	void printAligned(char* buffer, size_t size, unsigned char alignment);

public:
	std::function<void(Player<Authorized>)> connected;
	std::function<void(Player<Spawned>)> spawnedEvent;
	std::function<void(Player<Moved>)> movedEvent;
	std::function<void(Player<Rotated>)> rotatedEvent;
	std::function<void(Player<Shot>)> shotEvent;
	std::function<void(PlayerDisconnected)> disconnected;
	std::function<void(Player<EnemyKilled>)> enemyKilledEvent;
	std::function<void(Player<BulletSpawned>)> bulletSpawnedEvent;
	std::function<void(Player<BulletMoved>)> bulletMoved;
	std::function<void(Player<BulletCollided>)> bulletCollidedEvent;
	std::function<void(Player<BulletDissapeared>)> bulletDissapearedEvent;

	ClientSocket(clserv::TcpSocket socket);
	void authorize(byte id);
	void Receiving();
	void stop();
	void send(const char* buffer, int size);

	bool isConnected() const;
};

