#include <iostream>
#include <memory>

#include "object_class.h"

int main(int argument_count, char* argument_array)
{
	// This function will dump memory leaks to the visual studio output window upon termination
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	/* Rule of 3 */
	object_class* my_object = new object_class();	// constructor
	object_class another_object = *my_object;		// copy construction
	*my_object = another_object;					// copy assignment
	delete my_object;								// destructor
	my_object = nullptr;

	my_object = new object_class(another_object);	// copy construction
	delete my_object;								// destructor
	my_object = nullptr;

	// Move construction example
	my_object = new object_class();							// constructor
	object_class my_moved_object(std::move(*my_object));	// move constructor
		// my_object no longer presumed valid - In some cases it may be
	delete my_object;										// destructor
	my_object = nullptr;

	// Move assignment example
	my_object = new object_class();				// constructor
	object_class my_moved_assigned;				// constructor
	my_moved_assigned = std::move(*my_object);	// move assignment
		// my_object no longer presumed valid - In some cases it may be
	delete my_object;							// destructor
	my_object = nullptr;

	/* Raw pointers */
	int* ptr = nullptr;	// A pointer to address 0 of type integer 
	int x = 5;			// an integer with the initial stored value of 5 
	ptr = &x;			// store the address of x, in int_ptr 
	*ptr = 10;			// access the value at the stored address and assign the value 10 

	if (x == 10) // x now equals 10
		std::cout << std::endl << "Value of x reassigned though pointer" << std::endl;


	std::cout << std::endl << "Smart pointers" << std::endl;
	/* Smart pointers */
	std::unique_ptr<object_class> unique_ptr_object = std::make_unique<object_class>();		//ctr
	std::shared_ptr<object_class> shard_ptr_object = std::make_shared<object_class>();		//ctr
	std::shared_ptr<object_class> shard_from_unique_object = std::move(unique_ptr_object);	// pointer level move to shared
	std::cout << (unique_ptr_object ? "has value" : "null") << std::endl;					// Moved unique becomes null
	std::cout << (shard_from_unique_object ? "has value" : "null") << std::endl;			// has value

	// Scoping matters
	{
		std::unique_ptr<object_class> unique_ptr_object = std::make_unique<object_class>(); // ctr
		shard_from_unique_object = std::move(unique_ptr_object);							// scope calls the destructor on object after move
		std::cout << (unique_ptr_object ? "has value" : "null") << std::endl;				// Moved unique becomes null
		std::cout << (shard_from_unique_object ? "has value" : "null") << std::endl;		// has value
	}

	std::weak_ptr<object_class> weak_ptr_object = shard_from_unique_object;					// Weak pointer should be checked before use.


	std::cin.get();
	return 0;
}