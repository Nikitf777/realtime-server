#pragma once
#include "TcpSocket.h"
#include <optional>
#include <array>

#include "PackageStructs.h"

class ClientSocket {
private:
	clserv::TcpSocket _socket;
	std::atomic<bool> _stop;
	std::vector<char> _bytesToSend;
	byte _id;
	std::array<char, 15> _name;

	bool _connected = false;
	std::optional<Spawned> _spawned;
	std::optional<Moved> _moved;
	std::optional<float> _rotated;
	bool _shot;
	std::optional<byte> _enemyKilled;
	std::optional<byte> _bulletSpawned;
	std::optional<BulletMoved> _bulletMoved;
	std::optional<byte> _bulletCollided;
	std::optional<byte> _bulletDissapeared;

	byte getSpawned(char bytes[], byte from);
	byte getMoved(char bytes[], byte from);
	byte getRotated(char bytes[], byte from);
	byte getShot(char bytes[], byte from);
	byte getEnemyKilled(char bytes[], byte from);
	byte getBulletSpawned(char bytes[], byte from);
	byte getBulletMoved(char bytes[], byte from);
	byte getBulletCollided(char bytes[], byte from);
	byte getBulletDissapeared(char bytes[], byte from);

public:
	std::function<void(Player<Authorized>)> connected;
	std::function<void(byte, Spawned)> spawned;
	std::function<void(byte, Moved)> moved;
	std::function<void(byte, float)> rotated;
	std::function<void(byte)> shot;
	std::function<void(byte, byte)> enemyKilled;
	std::function<void(byte, byte)> bulletSpawned;
	std::function<void(byte, BulletMoved)> bulletMoved;
	std::function<void(byte, byte)> bulletCollided;
	std::function<void(byte, byte)> bulletDissapeared;

	std::function<void(byte)> disconnected;

	ClientSocket(clserv::TcpSocket socket);
	void authorize(byte id);
	void Sending();
	void Receiving();
	void stop();
	void send(std::vector<char>);

	bool isConnected() const;
};

