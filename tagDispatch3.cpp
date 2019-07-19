#include <utility>
#include <iostream>
#include <type_traits>

struct TagFloat{};
struct TagIntegral{};
struct TagOther{};

template<typename T>
void eatFloatingPointImpl(T&& meal, TagOther)
{
	std::cout<<"bleh! "<<meal<<"\n";
}

template<typename T>
void eatFloatingPointImpl(T&& meal, TagIntegral)
{
	std::cout<<"nom..bleh! "<<meal<<"\n";
}

template<typename T>
void eatFloatingPointImpl(T&& meal, TagFloat)
{
	std::cout<<"nom nom nom "<<meal<<"\n";
}

template <typename T, typename Enable = void>
struct Select : public TagOther {};

template <typename T>
struct Select<T, std::enable_if_t<std::is_integral<T>{}>> : public TagIntegral {};

template <typename T>
struct Select<T, typename std::enable_if<std::is_floating_point<T>::value>::type> : public TagFloat {};


template<typename T>
void eatFloatingPoint(T&& meal)
{
	eatFloatingPointImpl(std::forward<T>(meal), Select<std::remove_reference_t<T>>());
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
