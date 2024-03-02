#include "RealTimeServer.h"
#include "AutoMap.h"

int main() {

	AutoMap<float> map;
	//std::cout << sizeof(RealTimeServer::DataPackage);
	RealTimeServer server(12345, 4);
	server.listen();
	return 0;
}