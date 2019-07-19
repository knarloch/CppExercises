#include <utility>
#include <iostream>
#include <type_traits>

template<typename T>
void eatFloatingPointImpl(T&& meal, std::false_type)
{
	std::cout<<"bleh! "<<meal<<"\n";
}

template<typename T>
void eatFloatingPointImpl(T&& meal, std::true_type)
{
	std::cout<<"nom nom nom "<<meal<<"\n";
}

template<typename T>
void eatFloatingPoint(T&& meal)
{
	//eatFloatingPointImpl(meal, std::is_floating_point<std::remove_reference_t<T>>()); //cpp14
	eatFloatingPointImpl(std::forward<T>(meal), std::is_floating_point<typename std::remove_reference<T>::type>()); //cpp11
}

int main()
{
	eatFloatingPoint(1);
	eatFloatingPoint(1.5);
	double f = 2.5;
	eatFloatingPoint(f);
	eatFloatingPoint(std::string{"string"});
	return 0;
}
	
	
