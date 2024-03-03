#include "ClientSocket.h"

//#define DEBUG

char ClientSocket::getSpawned(char bytes[], char from)
{
	if (bytes[from] == false) { _spawned = std::nullopt; return ++from; }

	float x, y;
	char X[] = { bytes[from + 1], bytes[from + 2], bytes[from + 3], bytes[from + 4] };
	char Y[] = { bytes[from + 5], bytes[from + 6], bytes[from + 7], bytes[from + 8] };
	memcpy(&x, &X, sizeof(x));
	memcpy(&y, &Y, sizeof(y));
	_spawned = { x, y };
#ifdef DEBUG
	std::cout << '(' << x << ',' << y << ")\n";
#endif // DEBUG

	if (spawned != nullptr)
		spawned(_id, _spawned.value());
	return from + 9;
}
char ClientSocket::getMoved(char bytes[], char from)
{
	if (bytes[from] == false) { _moved = std::nullopt; return ++from; }

	float x, y;
	char X[] = { bytes[from + 1], bytes[from + 2], bytes[from + 3], bytes[from + 4] };
	char Y[] = { bytes[from + 5], bytes[from + 6], bytes[from + 7], bytes[from + 8] };
	memcpy(&x, &X, sizeof(x));
	memcpy(&y, &Y, sizeof(y));
	_moved = { x, y };
#ifdef DEBUG
	std::cout << '(' << x << ',' << y << ")\n";
#endif // DEBUG

	if (moved != nullptr)
		moved(_id, _moved.value());
	return from + 9;
}

char ClientSocket::getRotated(char bytes[], char from)
{
	if (bytes[from] == false) { _rotated = std::nullopt; return ++from; }

	float r;
	char R[] = { bytes[from + 1], bytes[from + 2], bytes[from + 3], bytes[from + 4] };
	memcpy(&r, &R, sizeof(r));
	_rotated = r;
#ifdef DEBUG
	std::cout << "r = " << r << '\n';
#endif // DEBUG

	if (rotated != nullptr)
		rotated(_id, _rotated.value());
	return from + 5;
}
char ClientSocket::getShot(char bytes[], char from)
{
	_shot = bytes[from];
	if (_shot && shot != nullptr)
		shot(_id);
	if (_shot)
#ifdef DEBUG
		std::cout << "Shot: " << _shot << '\n';
#endif // DEBUG

	return ++from;
}
char ClientSocket::getEnemyKilled(char bytes[], char from)
{
	if (bytes[from] == false) { _enemyKilled = std::nullopt; return ++from; }
	_enemyKilled = bytes[from + 1];

	if (enemyKilled != nullptr)
		enemyKilled(_id, _enemyKilled.value());
	return from + 2;
}
char ClientSocket::getBulletSpawned(char bytes[], char from)
{
	if (bytes[from] == false) { _bulletSpawned = std::nullopt; return ++from; }
	byte id = bytes[from + 1];
	float r = 1;
	byte R[] = { bytes[from + 2], bytes[from + 3], bytes[from + 4], bytes[from + 5] };
	memcpy(&r, &R, sizeof(r));
	_bulletSpawned = { id, r };
#ifdef DEBUG
	std::cout << "Spawned with rotation: " << r << '\n';
#endif // DEBUG


	if (bulletSpawned != nullptr)
		bulletSpawned(_id, _bulletSpawned.value());

	return from + 6;
}
char ClientSocket::getBulletMoved(char bytes[], char from)
{
	if (bytes[from] == false) { _bulletMoved = std::nullopt; return ++from; }
	byte id = bytes[from + 1];
	float x, y;
	char X[] = { bytes[from + 2], bytes[from + 3], bytes[from + 4], bytes[from + 5] };
	char Y[] = { bytes[from + 6], bytes[from + 7], bytes[from + 8], bytes[from + 9] };
	memcpy(&x, &X, sizeof(x));
	memcpy(&y, &Y, sizeof(y));
	_bulletMoved = { id, x, y };
#ifdef DEBUG
	std::cout << "Bullet: " << id << " - (" << x << ',' << y << ")\n";
#endif // DEBUG


	if (bulletMoved != nullptr)
		bulletMoved(_id, _bulletMoved.value());

	return from + 10;
}
char ClientSocket::getBulletCollided(char bytes[], char from)
{
	if (bytes[from] == false) { _bulletCollided = std::nullopt; return ++from; }
	_bulletCollided = bytes[from + 1];
#ifdef DEBUG
	std::cout << _bulletCollided.value() << " bullet collided\n";
#endif // DEBUG


	if (bulletCollided != nullptr)
		bulletCollided(_id, _bulletCollided.value());
	return from + 2;
}
char ClientSocket::getBulletDissapeared(char bytes[], char from)
{
	if (bytes[from] == false) { _bulletDissapeared = std::nullopt; return ++from; }
	_bulletDissapeared = bytes[from + 1];
#ifdef DEBUG
	std::cout << _bulletDissapeared.value() << " bullet dissapeared\n";
#endif // DEBUG


	if (bulletDissapeared != nullptr)
		bulletDissapeared(_id, _bulletDissapeared.value());
	return from + 2;
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

void ClientSocket::connect(byte id)
{
	_id = id;
	char buffer[1] = { _id };
	_socket.send(buffer, sizeof(buffer));
	char nameBuffer[19];
	_socket.receive(nameBuffer, sizeof(nameBuffer));
	std::array<char, 19> name;
	for (char i = 0; i < 19; i++)
		name[i] = nameBuffer[i];
	_name = name;
	if (connected != nullptr)
		connected(_id, _name);
}

void ClientSocket::Sending()
{
	while (true)
	{
		while (_stop)
		{
			std::this_thread::yield();
		}
		int bytesCount = _socket.send(_bytesToSend.data(), _bytesToSend.size());
		//std::cout << "sended " << bytesCount << " bytes" << '\n';
		_stop = true;
	}
}

void ClientSocket::Receiving()
{
	while (true)
	{
		char buffer[64];
		char q = recv((SOCKET)_socket, buffer, sizeof(buffer), 0);
		char pos = getSpawned(buffer, 0);
		pos = getMoved(buffer, pos);
		pos = getRotated(buffer, pos);
		pos = getShot(buffer, pos);
		pos = getEnemyKilled(buffer, pos);
		pos = getBulletSpawned(buffer, pos);
		pos = getBulletMoved(buffer, pos);
		pos = getBulletCollided(buffer, pos);
		getBulletDissapeared(buffer, pos);
		//std::cout << "good morning\n";
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