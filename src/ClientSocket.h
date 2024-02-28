#pragma once
#include "TcpSocket.h"
#include "Nullable.h"
#include <optional>

class ClientSocket {
	std::atomic<bool> _stop;
	std::vector<char> _bytesToSend;

	char getMoved(char bytes[], char from);
	char getRotated(char bytes[], char from);
	char getBulletSpawned(char bytes[], char from);
	char getBulletMoved(char bytes[], char from);

public:
	struct Moved { float x, y; };
	struct BulletSpawned { char id; float rotation; };
	struct BulletMoved { char id; float x, y; };

	bool connected;
	bool spawned;
	std::optional<Moved> moved;
	std::optional<float> rotated;
	bool shot;
	bool died;
	std::optional<BulletSpawned> bulletSpawned;
	std::optional<BulletMoved> bulletMoved;
	bool bulletCollided;
	bool bulletDissapeared;

	ClientSocket(clserv::TcpSocket socket);
	void Sending(const clserv::TcpSocket& socket);
	void Receiving(const clserv::TcpSocket& socket);
	void stop();
	void send(std::vector<char>);
};

