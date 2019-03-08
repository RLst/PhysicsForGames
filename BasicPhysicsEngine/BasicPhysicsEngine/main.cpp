#include "BasicPhysicsEngine.h"

int main() {
	
	// allocation
	auto app = new BasicPhysicsEngine();

	// initialise and loop
	app->run("AIE", 1440, 900, false);

	// deallocation
	delete app;

	return 0;
}