#pragma once
#include "Include.h"

constexpr size_t NICKNAME_SIZE = 15;

// Basic types
struct Vector2 { float x, y; };
struct Transform { Vector2 position; float rotation; };

using Id = byte;
template<size_t Size>
using StringName = std::array<char, Size>;

// Event types
struct Authorized { StringName<NICKNAME_SIZE> name; };
struct Spawned { float x, y; };
struct Moved { float x, y; };
struct Rotated { float rotation; };
struct Shot {};
struct EnemyKilled { Id enemyId; };
struct BulletSpawned { Id bulletId; };
struct BulletMoved { Id bulletId; float x, y; };
struct BulletCollided { Id buleltId; };
struct BulletDissapeared { Id bulletId; };
struct Disconnected {};

template<typename TPackage>
using Player = std::pair<Id, TPackage>;