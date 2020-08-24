#include "tape_impl.hpp"
#include <iostream>

template<typename T, T... ints>
void print_sequence(std::integer_sequence<T, ints...>)
{
	((std::cout << ints),...);
	std::cout << std::endl;
}

int main()
{
	using namespace tape;

	print_sequence(option<c_api::Ascii>::long_option::sequence_type{});
	print_sequence(option<tag::Binary>::long_option::sequence_type{});
	std::cout << tape::num_options << " options" << std::endl;
	option_attr<> attr = option_attr<>::Required;
	return sizeof(option<tag::Ascii>) + (attr == option_attr<>::Required);
}
