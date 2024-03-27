#pragma once
#include "Include.h"

struct Transform { float x, y, rotation; };

struct Spawned { float x, y; };
struct Moved { float x, y; };
struct BulletSpawned { byte bulletId; };
struct BulletMoved { byte bulletId; float x, y; };