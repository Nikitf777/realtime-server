#include "ClientSocket.h"

//#define DEBUG

byte ClientSocket::getSpawned(char bytes[], byte from)
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
byte ClientSocket::getMoved(char bytes[], byte from)
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

byte ClientSocket::getRotated(char bytes[], byte from)
{
	if (bytes[from] == false) { _rotated = std::nullopt; return ++from; }

	float r;
	char R[] = { bytes[from + 1], bytes[from + 2], bytes[from + 3], bytes[from + 4] };
	memcpy(&r, &R, sizeof(r));
	_rotated = r;
	std::cout << "r = " << r << '\n';
#ifdef DEBUG
#endif // DEBUG

	if (rotated != nullptr)
		rotated(_id, _rotated.value());
	return from + 5;
}
byte ClientSocket::getShot(char bytes[], byte from)
{
	_shot = bytes[from];
	if (_shot && shot != nullptr)
		shot(_id);
	if (_shot)
#ifdef DEBUG
		std::cout << "Shot: " << _shot << '\n';
#endif // DEBUG

	return from + 1;
}
byte ClientSocket::getEnemyKilled(char bytes[], byte from)
{
	if (bytes[from] == false) { _enemyKilled = std::nullopt; return ++from; }
	_enemyKilled = bytes[from + 1];

	if (enemyKilled != nullptr)
		enemyKilled(_id, _enemyKilled.value());
	return from + 2;
}
byte ClientSocket::getBulletSpawned(char bytes[], byte from)
{
	if (bytes[from] == false) { _bulletSpawned = std::nullopt; return ++from; }
	byte id = bytes[from + 1];
	_bulletSpawned = id;
#ifdef DEBUG
	std::cout << "Spawned with rotation: " << r << '\n';
#endif // DEBUG


	if (bulletSpawned != nullptr)
		bulletSpawned(_id, _bulletSpawned.value());

	return from + 2;
}
byte ClientSocket::getBulletMoved(char bytes[], byte from)
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
byte ClientSocket::getBulletCollided(char bytes[], byte from)
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
byte ClientSocket::getBulletDissapeared(char bytes[], byte from)
{
	if (bytes[from] == false) { _bulletDissapeared = std::nullopt; return ++from; }
	_bulletDissapeared = bytes[from + 1];
	std::cout << _bulletDissapeared.value() << " bullet dissapeared\n";
#ifdef DEBUG
#endif // DEBUG


	if (bulletDissapeared != nullptr)
		bulletDissapeared(_id, _bulletDissapeared.value());
	return from + 2;
}
ClientSocket::ClientSocket(clserv::TcpSocket socket)
{
	_socket = socket;
	_stop = true;
}

void ClientSocket::authorize(byte id)
{
	_id = id;
	char buffer[1] = { _id };
	_socket.send(buffer, sizeof(buffer));
	char nameBuffer[15];
	char nameLength = _socket.receive(nameBuffer, sizeof(nameBuffer));
	std::array<char, 15> name{};
	for (char i = 0; i < nameLength; i++)
		_name[i] = nameBuffer[i];
	if (connected != nullptr)
		connected(_id, _name);

	_connected = true;

	std::thread([this] {
		Receiving();
		}).detach();

	std::thread([this] {
		Sending();
		}).detach();
}

void ClientSocket::Sending()
{
	while (true)
	{
		while (_stop)
		{
			std::this_thread::yield();
		}

		try
		{
			int bytesCount = _socket.send(_bytesToSend.data(), _bytesToSend.size());
#ifdef DEBUG
			std::cout << "Sended " << bytesCount << " bytes to client " << (int)_id << '\n';
#endif // DEBUG
			_stop = true;
		}
		catch (const std::exception&)
		{
			if (disconnected != nullptr)
				disconnected(_id);
#ifdef DEBUG
			std::cout << "Client " << _id << " disconnected\n";
#endif // DEBUG

			return;
		}
	}
}

void ClientSocket::Receiving()
{
	while (true)
	{
		char buffer[128];
		byte q = recv((SOCKET)_socket, buffer, sizeof(buffer), 0);
		byte pos = getSpawned(buffer, 0);
		//pos = getMoved(buffer, pos);
		//pos = getRotated(buffer, pos);
		//pos = getShot(buffer, pos);
		//pos = getEnemyKilled(buffer, pos);
		//pos = getBulletSpawned(buffer, pos);
		//pos = getBulletMoved(buffer, pos);
		//pos = getBulletCollided(buffer, pos);
		//getBulletDissapeared(buffer, pos);
	}	
}

void ClientSocket::stop()
{
	_stop = true;
}

void ClientSocket::send(std::vector<char> bytes)
{
	if (!_connected) return;
	_bytesToSend = bytes;
	_stop = false;
}

bool ClientSocket::isConnected() const
{
	return _connected;
}
