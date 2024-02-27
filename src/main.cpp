#include "RealTimeServer.h"

int main() {
	//std::cout << sizeof(RealTimeServer::DataPackage);
	RealTimeServer server(12345, 4);
	server.listen();
	return 0;
}