#include "BasicPhysicsEngine.h"

int main() {
	
	// allocation
	auto app = new BasicPhysicsEngine();

	// initialise and loop
	app->run("pkr Basic Physics Engine", 1440, 900, false);

	// deallocation
	delete app;

	return 0;
}