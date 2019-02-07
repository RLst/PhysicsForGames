#include "object_class.h"
#include <iostream>

object_class::object_class()
{
	std::cout << "Constructor" << std::endl;
}

object_class::~object_class()
{
	std::cout << "Destructor" << std::endl;
}

object_class::object_class(const object_class& a_other)
{
	std::cout << "Copy Constructor" << std::endl;
}

object_class& object_class::operator=(const object_class& a_other)
{
	std::cout << "Assignment Operator" << std::endl;
	return *this;
}

// Member wise deep copy and assume a_other is going out of scope so clean up any pointers
object_class::object_class(object_class&& a_other) noexcept
{
	std::cout << "Move Constructor" << std::endl;
}

// Member wise deep copy and assume a_other is going out of scope so clean up any pointers
object_class& object_class::operator=(object_class&& a_other) noexcept
{
	std::cout << "Move Assignment Operator" << std::endl;
	return *this;
}