#include "RealTimeServer.h"
#include "AutoMap.h"
#include "array"

int main() {

	Vector2 v1{ 1,2 };
	Vector2 v2{ 5,6 };
	Vector2 v3{ 7,12 };
	unsigned char* ptr = reinterpret_cast<unsigned char*>(&v1);
	float x, y;
	ByteStream stream(512);
	stream << v1 << v2 << v3;
	Vector2 test;
	stream >> test;
	stream >> test;
	stream >> test;
	std::cout << test.x << std::endl << test.y << std::endl;
	std::cout << sizeof std::array<char, 15> << std::endl;

	RealTimeServer server(12345, 4);
	std::thread([&server] {server.listen(); }).detach();
	//while (true)
	//{
	//	std::this_thread::yield();
	//}
	server.mainLoop();
	return 0;
}