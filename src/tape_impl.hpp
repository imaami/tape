#ifndef TAPE__TAPE_IMPL_HPP__INCLUDED_
#define TAPE__TAPE_IMPL_HPP__INCLUDED_

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <type_traits>
#include <limits>
#include <utility>
#include <variant>
#include "tape.h"

template<typename T>
struct is_int : public std::bool_constant<
	std::is_integral_v<T> &&
	!std::is_same_v<T, bool>
> {};

template<typename T>
inline constexpr bool is_int_v = is_int<T>::value;

template<typename T, T v>
struct is_arg_type : public std::bool_constant<
	is_int_v<T> &&
	(v == static_cast<T>(TAPE_NO_ARG      ) ||
	 v == static_cast<T>(TAPE_REQUIRED_ARG) ||
	 v == static_cast<T>(TAPE_OPTIONAL_ARG))
> {};

template<typename T, T v>
inline constexpr bool is_arg_type_v = is_arg_type<T, v>::value;

template<typename T = std::uint8_t,
         std::enable_if_t<is_int_v<T>, int> = 0>
struct option_arg
{
	using type = option_arg<T>;
	using value_type = std::make_unsigned_t<T>;

	enum struct enum_type : value_type
	{
		No       = 1 + TAPE_NO_ARG,
		Required = 1 + TAPE_REQUIRED_ARG,
		Optional = 1 + TAPE_OPTIONAL_ARG
	};

	static constexpr const enum_type No       = enum_type::No;
	static constexpr const enum_type Required = enum_type::Required;
	static constexpr const enum_type Optional = enum_type::Optional;

	template<auto v,
	         std::enable_if_t<is_arg_type_v<v>, int> = 0>
	static constexpr inline enum_type from_arg_type() noexcept {
		using v_t = decltype(v);
		if constexpr (v == static_cast<v_t>(TAPE_NO_ARG))
			return enum_type::No;
		else if constexpr (v == static_cast<v_t>(TAPE_REQUIRED_ARG))
			return enum_type::Required;
		else
			return enum_type::Optional;
	}

	template<enum_type V>
	static constexpr inline value_type as_int = static_cast<value_type>(V);
};

template<typename T>
struct option_attr_impl
{
	using arg_type = typename option_arg<T>::type;
	using value_type = typename arg_type::value_type;

private:
	using arg_enum_type = typename arg_type::enum_type;

	template<arg_enum_type V>
	static constexpr inline value_type arg_as_int = arg_type::template as_int<V>;

protected:
	enum struct enum_type : value_type
	{
		Undefined = 0,
		No        = arg_as_int<arg_type::No>,
		Required  = arg_as_int<arg_type::Required>,
		Optional  = arg_as_int<arg_type::Optional>
	};
};

template<typename T = std::uint8_t>
struct option_attr : public option_attr_impl<T>
{
	using typename option_attr_impl<T>::enum_type;

	static constexpr const enum_type Undefined = enum_type::Undefined;
	static constexpr const enum_type No        = enum_type::No;
	static constexpr const enum_type Required  = enum_type::Required;
	static constexpr const enum_type Optional  = enum_type::Optional;

	constexpr inline option_attr(enum_type v = Undefined) noexcept : m_data{v}{}
	constexpr inline operator enum_type() const noexcept { return m_data; }

private:
	enum_type m_data;
};

template<typename T>
struct char_lookup_element
{
	using value_type = std::make_unsigned_t<T>;
	using attr_type = typename option_attr<T>::enum_type;

private:
	value_type m_data;

	static constexpr const int index_bits {
		std::numeric_limits<value_type>::digits - 2
	};

public:
	constexpr inline char_lookup_element(value_type index,
	                                     attr_type  attr) noexcept
		: m_data{static_cast<value_type>(attr) | index} {}
};

template <tape_option_tag_t V>
using tag_constant = std::integral_constant<tape_option_tag_t, V>;

template <char C>
using char_constant = std::integral_constant<char, C>;

template<char... Chars>
using char_sequence = std::integer_sequence<char, Chars...>;

template<typename T, std::size_t... I>
constexpr static inline auto make_char_sequence_impl(std::index_sequence<I...>) noexcept
{
	return char_sequence<T::data[I]...>{};
}

template<typename T>
constexpr static inline auto make_char_sequence() noexcept
{
	return make_char_sequence_impl<T>(std::make_index_sequence<T::size>{});
}

template<typename /* void */, typename... Ts>
struct options_variant_helper
{
	using type = std::variant<Ts...>;
};

template<typename T, T Nul = 0>
struct char_lookup_table
{
	using value_type = T;
	value_type data[256];

	constexpr char_lookup_table() noexcept {
		for (std::uint16_t i = 0; i < 256; ++i) {
			data[i] = Nul;
		}
	}
};

template<typename... Ts>
struct short_option_to_index : public char_lookup_table<std::uint8_t>
{
	constexpr short_option_to_index(std::variant<Ts...> const &) noexcept
	{
		// TODO: save index of variant
		((data[Ts::short_option] = Ts::short_option), ...);
	}
};

template<typename T>
struct option {
	constexpr static const bool exists = false;
};

/* Specialize struct option for each command-line option
 */
#define OPTION(tag_, chr_, str_, arg_, help_) \
template<>\
struct option<typename tag_constant<tag_>::type> {\
	constexpr static const bool exists = true;\
	constexpr static const char short_option = chr_;\
	struct long_option {\
		constexpr static char const data[] = str_;\
		constexpr static std::size_t size = sizeof(data);\
		using sequence_type = decltype(make_char_sequence<long_option>());\
	};\
	struct help {\
		constexpr static char const data[] = help_;\
		constexpr static std::size_t size = sizeof(data);\
	};\
};
#include "options_p.hpp"

struct options {
	//template<char C>
	//using option = ::option<char_constant<C>>;

	template <tape_option_tag_t V>
	using option = ::option<tag_constant<V>>;

	using variant_type = options_variant_helper<
		/* Begin with a dummy void type to allow OPTION to be defined
		 * with a leading comma. The options_variant_helper template
		 * chops off the dummy type and returns the rest unchanged.
		 */
		void

		#define OPTION(tag_, chr_, str__, arg_, help_) , option<tag_>
		#include "options_p.hpp"
	>::type;

	/** Total number of available command-line options */
	constexpr static const std::size_t count = std::variant_size_v<variant_type>;
};

#endif /* TAPE__TAPE_IMPL_HPP__INCLUDED_ */
