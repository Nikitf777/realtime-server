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
	std::array<char, 19> _name;

	bool _connected;
	std::optional<Spawned> _spawned;
	std::optional<Moved> _moved;
	std::optional<float> _rotated;
	bool _shot;
	std::optional<char> _enemyKilled;
	std::optional<BulletSpawned> _bulletSpawned;
	std::optional<BulletMoved> _bulletMoved;
	std::optional<byte> _bulletCollided;
	std::optional<byte> _bulletDissapeared;

	char getSpawned(char bytes[], char from);
	char getMoved(char bytes[], char from);
	char getRotated(char bytes[], char from);
	char getShot(char bytes[], char from);
	char getEnemyKilled(char bytes[], char from);
	char getBulletSpawned(char bytes[], char from);
	char getBulletMoved(char bytes[], char from);
	char getBulletCollided(char bytes[], char from);
	char getBulletDissapeared(char bytes[], char from);

public:
	std::function<void(byte, std::array<char, 19> name)> connected;
	std::function<void(byte, Spawned)> spawned;
	std::function<void(byte, Moved)> moved;
	std::function<void(byte, float)> rotated;
	std::function<void(byte)> shot;
	std::function<void(byte, byte)> enemyKilled;
	std::function<void(byte, BulletSpawned)> bulletSpawned;
	std::function<void(byte, BulletMoved)> bulletMoved;
	std::function<void(byte, byte)> bulletCollided;
	std::function<void(byte, byte)> bulletDissapeared;

	ClientSocket(clserv::TcpSocket socket);
	void connect(byte id);
	void Sending();
	void Receiving();
	void stop();
	void send(std::vector<char>);
};

