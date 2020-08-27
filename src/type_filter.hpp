#ifndef TAPE__TYPE_FILTER_HPP__INCLUDED_
#define TAPE__TYPE_FILTER_HPP__INCLUDED_

#include <type_traits>

/**
 * Type list.
 */
namespace type {
	template <typename...>
	struct list {};
}

/**
 * Join type lists.
 */
namespace util {
	namespace impl {
		template <typename...>
		struct join {};

		template <>
		struct join<>
		{
			using type = type::list<>;
		};

		template <typename... Ts>
		struct join<type::list<Ts...>>
		{
			using type = type::list<Ts...>;
		};

		template <typename... Ts0, typename... Ts1, typename... Ts>
		struct join<type::list<Ts0...>, type::list<Ts1...>, Ts...>
			: join<type::list<Ts0..., Ts1...>, Ts...> {};
	}

	template <typename... Ts>
	using join_t = typename impl::join<Ts...>::type;
}

/**
 * Conditionally wrap a type in a type list.
 */
namespace util {
	namespace impl {
		template <bool>
		struct wrap_if
		{
			template <typename T>
			using type = type::list<>;
		};

		template <>
		struct wrap_if<true>
		{
			template <typename T>
			using type = type::list<T>;
		};
	}

	template <template <typename> typename Pred, typename T>
	using wrap_if_t = typename impl::wrap_if<Pred<T>::value>::template type<T>;
}

/**
 * Filter a list of types.
 */
namespace util {
	namespace impl {
		template <template <typename> typename, typename>
		struct filter;

		template <template <typename> typename Pred, typename... Ts>
		struct filter<Pred, type::list<Ts...>>
		{
			using type = join_t<wrap_if_t<Pred, Ts>...>;
		};
	}

	template <template <typename> typename Pred, typename... Ts>
	using filter_t = typename impl::filter<Pred, type::list<Ts...>>::type;
}

#endif /* TAPE__TYPE_FILTER_HPP__INCLUDED_ */
