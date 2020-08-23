#include "tape_impl.hpp"
#include <iostream>

template<typename T, T... ints>
void print_sequence(std::integer_sequence<T, ints...>)
{
	((std::cout << ints),...);
}

int main()
{
	using namespace tape;

	print_sequence(option<tag::Ascii>::long_option::sequence_type{});
	print_sequence(option<std::variant_alternative_t<0, type::tag_variant>::value>::long_option::sequence_type{});
	std::cout << std::variant_size_v<tape::type::tag_variant> << " options" << std::endl;
	option_attr<> attr = option_attr<>::Required;
	return sizeof(option<tag::Ascii>) + (attr == option_attr<>::Required);
}
