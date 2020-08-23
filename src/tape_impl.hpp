#ifndef TAPE__TAPE_IMPL_HPP__INCLUDED_
#define TAPE__TAPE_IMPL_HPP__INCLUDED_

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <type_traits>
#include <limits>
#include <utility>
#include <variant>

/* Enclose the public C API in a namespace */
namespace tape::c_api {
#include "tape.h"
}

/* Import option tags as a scoped enum */
namespace tape {
	#define OPTION(tag_, chr_, str_, arg_, help_) tag_ = c_api::tag_,
	enum struct tag {
	#include "options_p.hpp"
	};
}

/* Tuck away our helper types */
namespace tape::type {
	template <tape::tag V>
	using tag_constant = std::integral_constant<tape::tag, V>;

	template <char C>
	using char_constant = std::integral_constant<char, C>;

	template<char... Cs>
	using char_sequence = std::integer_sequence<char, Cs...>;
}

namespace tape::util::impl {
	template<typename T>
	struct is_int : public std::bool_constant<
		std::is_integral_v<T> &&
		!std::is_same_v<T, bool>
	> {};

	template<typename T, T v>
	struct is_valid_arg_type : public std::bool_constant<
		is_int<T>::value &&
		(v == static_cast<T>(c_api::TAPE_NO_ARG      ) ||
		 v == static_cast<T>(c_api::TAPE_REQUIRED_ARG) ||
		 v == static_cast<T>(c_api::TAPE_OPTIONAL_ARG))
	> {};

	template<typename I, typename T = void>
	struct enable_if_int : public std::enable_if<is_int<I>::value, T> {};

	template<typename I, I v, typename T = void>
	struct enable_if_valid_arg_type : public std::enable_if<is_valid_arg_type<I, v>::value, T> {};

	template<typename T, std::size_t... I>
	constexpr static inline auto make_char_sequence(std::index_sequence<I...>) noexcept
	{
		return type::char_sequence<T::data[I]...>{};
	}
}

/* Various useful things */
namespace tape::util {
	template<typename T>
	inline constexpr bool is_int_v = impl::is_int<T>::value;

	template<typename T, T v>
	inline constexpr bool is_valid_arg_type_v = impl::is_valid_arg_type<T, v>::value;

	template<typename I, typename T = void>
	using enable_if_int_t = typename impl::enable_if_int<I, T>::type;

	template<auto v, typename T = void>
	using enable_if_valid_arg_type_t = typename impl::enable_if_valid_arg_type<decltype(v), v, T>::type;

	template<typename T>
	constexpr static inline auto make_char_sequence() noexcept
	{
		return impl::make_char_sequence<T>(std::make_index_sequence<T::size>{});
	}

	template<typename /* void */, typename... Ts>
	struct beheaded_variant
	{
		using type = std::variant<Ts...>;
	};
}

namespace tape::type {
	using tag_variant = util::beheaded_variant<
		/* Begin with a dummy void type to allow OPTION to be defined
		 * with a leading comma. The util::beheaded_variant template
		 * chops off the dummy type and returns the rest unchanged.
		 */
		void

		#define OPTION(tag_, chr_, str__, arg_, help_) , tag_constant<tape::tag::tag_>::type
		#include "options_p.hpp"
	>::type;

	template<typename T>
	struct option {
		constexpr static const bool exists = false;
	};

	template<typename T>
	struct short_option {
		constexpr static const bool exists = false;
	};

	/* Specialize struct option for each command-line option
	 */
	#define OPTION(tag_, chr_, str_, arg_, help_) \
	template<>\
	struct option<typename tag_constant<tape::tag::tag_>::type> {\
		constexpr static const bool exists = true;\
		constexpr static const char short_option = chr_;\
		struct long_option {\
			constexpr static char const data[] = str_;\
			constexpr static std::size_t size = sizeof(data);\
			using sequence_type = decltype(util::make_char_sequence<long_option>());\
		};\
		struct help {\
			constexpr static char const data[] = help_;\
			constexpr static std::size_t size = sizeof(data);\
		};\
	};\
	template<>\
	struct option<typename char_constant<chr_>::type>\
		: public option<typename tag_constant<tape::tag::tag_>::type> {};
	#include "options_p.hpp"
}

namespace tape {
	/** Total number of available command-line options */
	static constexpr const std::size_t num_options = std::variant_size_v<type::tag_variant>;

	template <tape::tag V>
	using option = type::option<type::tag_constant<V>>;

	template <char C>
	using short_option = type::option<type::char_constant<C>>;

	template<typename T = std::uint8_t,
	         util::enable_if_int_t<T, int> = 0>
	struct option_arg
	{
		using type = option_arg<T>;
		using value_type = std::make_unsigned_t<T>;

		enum struct enum_type : value_type
		{
			No       = 1 + c_api::TAPE_NO_ARG,
			Required = 1 + c_api::TAPE_REQUIRED_ARG,
			Optional = 1 + c_api::TAPE_OPTIONAL_ARG
		};

		static constexpr const enum_type No       = enum_type::No;
		static constexpr const enum_type Required = enum_type::Required;
		static constexpr const enum_type Optional = enum_type::Optional;

		template<auto V,
		         util::enable_if_valid_arg_type_t<V, int> = 0>
		static constexpr inline enum_type from_arg_type() noexcept
		{
			using V_t = decltype(V);
			if constexpr (V == static_cast<V_t>(c_api::TAPE_NO_ARG))
				return enum_type::No;
			else if constexpr (V == static_cast<V_t>(c_api::TAPE_REQUIRED_ARG))
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
}

#endif /* TAPE__TAPE_IMPL_HPP__INCLUDED_ */
