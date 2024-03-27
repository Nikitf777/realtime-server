#include "RealTimeServer.h"
#include "AutoMap.h"

int main() {

	RealTimeServer server(12345, 4);
	std::thread([&server] {server.listen(); }).detach();
	//while (true)
	//{
	//	std::this_thread::yield();
	//}
	server.mainLoop();
	return 0;
}