/* @brief A base object with some imposter data for example purposes
	for use as a second year c++ booster shot
*/

#pragma once
class object_class
{
private:

public:
	/* @brief Constructor for default initialisation
	*/
	object_class();


	/* @brief Destructor to clean up any raw pointers or
		unregister from any managers
	*/
	~object_class();

	/* @brief Copy Constructor for memberwise deep copy
	   @param The object to be copied
	*/
	object_class(const object_class& a_other);

	/* @brief Copy assignment operator for memberwise deep copy
	   @param The object to be copied
	   @return New copy by reference
	*/
	object_class& operator=(const object_class& a_other);

	/* @brief Move Constructor for relocating internal data
	   @param The object to be copied
	*/
	object_class(object_class&& a_other) noexcept;

	/* @brief Move assignment operator for relocating internal data
	   @param The object to be copied
	   @return New copy by reference
	*/
	object_class& operator=(object_class&& a_other) noexcept;


};

