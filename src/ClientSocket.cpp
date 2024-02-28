#include "ClientSocket.h"
char ClientSocket::getMoved(char bytes[], char from)
{
	if (bytes[from] == false) { moved = std::nullopt; return ++from; }

	float x, y;
	char X[] = { bytes[from + 1], bytes[from + 2], bytes[from + 3], bytes[from + 4] };
	char Y[] = { bytes[from + 5], bytes[from + 6], bytes[from + 7], bytes[from + 8] };
	memcpy(&x, &X, sizeof(x));
	memcpy(&y, &Y, sizeof(y));
	moved = { x, y };
	std::cout << '(' << x << ',' << y << ")\t";
	return from + 9;
}

char ClientSocket::getRotated(char bytes[], char from)
{
	if (bytes[from] == false) { rotated = std::nullopt; return ++from; }

	float r;
	char R[] = { bytes[from + 1], bytes[from + 2], bytes[from + 3], bytes[from + 4] };
	memcpy(&r, &R, sizeof(r));
	rotated = r;
	std::cout << "r = " << r << '\t';

	return from + 5;
}
char ClientSocket::getBulletSpawned(char bytes[], char from)
{
	if (bytes[from] == false) { rotated = std::nullopt; return ++from; }
	char id = bytes[from + 1];
	float r;
	char R[] = { bytes[from + 2], bytes[from + 3], bytes[from + 4], bytes[from + 5] };
	memcpy(&r, &R, sizeof(r));
	bulletSpawned.value() = { id, r };

	return from + 6;
}
char ClientSocket::getBulletMoved(char bytes[], char from)
{
	if (bytes[from] == false) { rotated = std::nullopt; return ++from; }
	char id = bytes[from + 1];
	float x, y;
	char X[] = { bytes[from + 2], bytes[from + 3], bytes[from + 4], bytes[from + 5] };
	char Y[] = { bytes[from + 6], bytes[from + 7], bytes[from + 8], bytes[from + 9] };
	memcpy(&x, &X, sizeof(x));
	memcpy(&y, &Y, sizeof(y));
	bulletMoved.value() = { id, x, y };

	return from + 10;
}
ClientSocket::ClientSocket(clserv::TcpSocket socket)
{
	_stop = true;
	std::thread([this, socket] {
		Receiving(socket);
		}).detach();

	std::thread([this, socket] {
		Sending(socket);
		}).detach();
}

void ClientSocket::Sending(const clserv::TcpSocket& socket)
{
	while (true)
	{
		while (_stop)
		{
			std::this_thread::yield();
		}
		socket.send("hello");
		_stop = true;
	}
}

void ClientSocket::Receiving(const clserv::TcpSocket& socket)
{
	//bool connected;
	//bool spawned;
	//std::optional<Moved> moved;
	//std::optional<float> rotated;
	//bool shot;
	//bool died;
	//std::optional<BulletSpawned> bulletSpawned;
	//std::optional<BulletMoved> bulletMoved;
	//bool bulletCollided;
	//bool bulletDissapeared;
while (true)
	{
		char buffer[64];
		char q = recv((SOCKET)socket, buffer, sizeof(buffer), 0);
		//std::cout << "received " << q << " bytes\n";
		char pos = 0;
		spawned = buffer[pos];
		pos++;
		pos = getMoved(buffer, pos);
		pos = getRotated(buffer, pos);
		shot = buffer[pos];
		pos++;
		died = buffer[pos];
		pos++;
		pos = getBulletSpawned(buffer, pos);
		pos = getBulletMoved(buffer, pos);
		bulletCollided = buffer[pos];
		pos++;
		bulletDissapeared = buffer[pos];
		std::cout << std::endl;
		//if (rotated.has_value())
		//	std::cout << rotated.value();
	}	
}

void ClientSocket::stop()
{
	_stop = true;
}

void ClientSocket::send(std::vector<char> bytes)
{
	_bytesToSend = bytes;
	_stop = false;
}