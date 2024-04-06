#include "PackageFromPlayer.h"

std::string PackageFromPlayer::toString() const
{
    std::stringstream stream;
    if (spawned.hasValue) {
        stream << "spawned {\n\t";
        stream << "rotation: " << spawned.value.transform.rotation << std::endl;
        stream << "position: " << spawned.value.transform.position.x << ',' << spawned.value.transform.position.y << std::endl;
        std::cout << "}\n";
    }

    if (moved.hasValue) {
        stream << "moved {\n\t";
        stream << "position: " << moved.value.position.x << ',' << moved.value.position.y << std::endl;
        std::cout << "}\n";
    }

    if (rotated.hasValue) {
        stream << "rotated {\n\t";
        stream << "rotation: " << rotated.value.rotation << std::endl;
        std::cout << "}\n";
    }

    if (shot.hasValue) {
        stream << "shot {\n\t";
        stream << "shot: " << (int)shot.value.shotType << std::endl;
        std::cout << "}\n";
    }

    if (enemyKilled.hasValue) {
        stream << "enemyKilled {\n\t";
        stream << "enemyId: " << (int)enemyKilled.value.enemyId << std::endl;
        std::cout << "}\n";
    }

    if (bulletSpawned.hasValue) {
        stream << "bulletSpawned {\n\t";
        stream << "bulletId: " << (int)bulletSpawned.value.bulletId << std::endl;
        std::cout << "}\n";
    }

    if (bulletMoved.hasValue) {
        stream << "bulletMoved {\n\t";
        stream << "position: " << bulletMoved.value.position.x << ',' << bulletMoved.value.position.y << std::endl;
        std::cout << "}\n";
    }

    if (bulletCollided.hasValue) {
        stream << "bulletCollided {\n\t";
        stream << "rotation: " << (int)bulletCollided.value.bulletId << std::endl;
        std::cout << "}\n";
    }

    if (bulletDissapeared.hasValue) {
        stream << "bulletDissapeared {\n\t";
        stream << "bulletId: " << (int)bulletDissapeared.value.bulletId << std::endl;
        std::cout << "}\n";
    }

    return stream.str();
}