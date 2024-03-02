#pragma once
#include "TcpSocket.h"
#include "Nullable.h"
#include <optional>

class ClientSocket {
public:
	struct Spawned { float x, y; };
	struct Moved { float x, y; };
	struct BulletSpawned { char bulletId; float rotation; };
	struct BulletMoved { char bulletId; float x, y; };

private:
	clserv::TcpSocket _socket;
	std::atomic<bool> _stop;
	std::vector<char> _bytesToSend;
	char _id;
	char _name[19];

	bool _connected;
	std::optional<Spawned> _spawned;
	std::optional<Moved> _moved;
	std::optional<float> _rotated;
	bool _shot;
	bool _died;
	std::optional<BulletSpawned> _bulletSpawned;
	std::optional<BulletMoved> _bulletMoved;
	bool _bulletCollided;
	bool _bulletDissapeared;

	char getMoved(char bytes[], char from);
	char getRotated(char bytes[], char from);
	char getBulletSpawned(char bytes[], char from);
	char getBulletMoved(char bytes[], char from);

public:
	std::function<void(char, char[19])> connected;
	std::function<void(char, Spawned)> spawned;
	std::function<void(char, Moved)> moved;
	std::function<void(char, float)> rotated;
	std::function<void(char)> shot;
	std::function<void(char, char)> died;
	std::function<void(char, BulletSpawned)> bulletSpawned;
	std::function<void(char, BulletMoved)> bulletMoved;
	std::function<void(char, char)> bulletCollided;
	std::function<void(char, char)> bulletDissapeared;

	ClientSocket(clserv::TcpSocket socket);
	void connect(char id);
	void Sending();
	void Receiving();
	void stop();
	void send(std::vector<char>);
};

