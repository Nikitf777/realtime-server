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
	_socket = socket;
	_stop = true;
	std::thread([this] {
		Receiving();
		}).detach();

	std::thread([this] {
		Sending();
		}).detach();
}

void ClientSocket::connect(char id)
{
	_id = id;
	char buffer[1] = { _id };
	_socket.send(buffer, sizeof(buffer));
	char name[19];
	_socket.receive(name, sizeof(name));
	for (char i = 0; i < 19; i++)
		_name[i] = name[i];
}

void ClientSocket::Sending()
{
	while (true)
	{
		while (_stop)
		{
			std::this_thread::yield();
		}
		_socket.send(_bytesToSend.data(), _bytesToSend.size());
		_stop = true;
	}
}

void ClientSocket::Receiving()
{
	while (true)
	{
		char buffer[64];
		char q = recv((SOCKET)_socket, buffer, sizeof(buffer), 0);
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