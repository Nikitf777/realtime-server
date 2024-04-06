#pragma once
#include "PackageStructs.h"
#include <optional>
#include <sstream>

template<typename T>
struct ByteNullable {
	T value;
	bool hasValue;
};

struct PackageFromPlayer {
	ByteNullable<Spawned> spawned;
	ByteNullable<Moved> moved;
	ByteNullable<Rotated> rotated;
	ByteNullable<Shot> shot;
	ByteNullable<EnemyKilled> enemyKilled;
	ByteNullable<BulletSpawned> bulletSpawned;
	ByteNullable<BulletMoved> bulletMoved;
	ByteNullable<BulletCollided> bulletCollided;
	ByteNullable<BulletDissapeared> bulletDissapeared;

	std::string toString() const;
};

//struct PackageFromPlayer {
//	std::optional<Spawned> spawned;
//	std::optional<Moved> moved;
//	std::optional<Rotated> rotated;
//	std::optional<Shot> shot;
//	std::optional<EnemyKilled> enemyKilled;
//	std::optional<BulletSpawned> bulletSpawned;
//	std::optional<BulletMoved> bulletMoved;
//	std::optional<BulletCollided> bulletCollided;
//	std::optional<BulletDissapeared> bulletDissapeared;
//
//	std::string toString() const;
//};