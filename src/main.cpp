#include "RealTimeServer.h"

int main() {
	RealTimeServer server(12345, 4);
	server.listen();
	return 0;
}