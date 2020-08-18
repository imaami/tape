#include "tape_impl.hpp"
#include <iostream>

template<typename T, T... ints>
void print_sequence(std::integer_sequence<T, ints...>)
{
	((std::cout << ints),...);
}

int main()
{
	print_sequence(options::option<Ascii>::long_option::sequence_type{});
	print_sequence(std::variant_alternative_t<0, options::variant_type>::long_option::sequence_type{});
	option_attr<> attr = option_attr<>::Required;
	return sizeof(options::option<Ascii>) + (attr == option_attr<>::Required);
}
