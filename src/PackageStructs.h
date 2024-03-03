#pragma once
#include "Include.h"


struct Spawned { float x, y; };
struct Moved { float x, y; };
struct BulletSpawned { byte bulletId; float rotation; };
struct BulletMoved { byte bulletId; float x, y; };