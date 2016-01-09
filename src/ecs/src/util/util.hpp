#include <string>
#include <iostream>
#include <tuple>
#include <utility>
#include <array>
#include <vector>
#include <typeinfo>

#include "range/v3/view.hpp"
#include "boost/hana.hpp"
#include <boost/hana/ext/std/tuple.hpp>
#include <boost/hana/ext/std/pair.hpp>

namespace hana = boost::hana;
template <class Tup>
auto tuple_ref(Tup& tup)
{
  return hana::transform(tup, [](auto& t) { return std::ref(t); });
}

template <class... Ts>
constexpr auto has_duplicates()
{
  constexpr auto comp_tuple = hana::tuple_t<Ts...>;
  return hana::size(comp_tuple) ==
         hana::size(hana::to<hana::set_tag>(comp_tuple));
}
template <typename Iterable, typename T>
constexpr auto index_of(Iterable const& iterable, T const& element)
{
  auto size = hana::size(iterable);
  auto dropped = decltype(
      hana::size(hana::drop_while(iterable, hana::not_equal.to(element)))){};
  return size - dropped;
}

template <class... Ts>
void print(Ts&&... ts)
{
  auto t = hana::make_tuple(std::forward<Ts>(ts)...);
  hana::for_each(t, [](auto&& v) { std::cout << v << " "; });
  std::cout << std::endl;
}

template <class Tup, class T>
constexpr auto contains_type(Tup&& tup, T)
{
  return hana::contains(tup, hana::type_c<T>);
}
template <class TypeTup, class ValueTup>
constexpr auto contains_all_types(TypeTup&& tup_type, ValueTup&& tup_value)
{
  auto filter_tup = hana::filter(tup_value, [&](auto&& v) {
    return hana::contains(tup_type, hana::decltype_(v));
  });
  return hana::equal(hana::size(tup_type), hana::size(filter_tup));
}

template <size_t... Indices, class Tup>
auto to_hana_tup_impl(std::index_sequence<Indices...>, Tup&& tup)
{
  return hana::make_tuple(std::ref(std::get<Indices>(tup))...);
}
template <class... Ts>
auto to_hana_tup(std::tuple<Ts&...>& tup)
{
  return to_hana_tup_impl(std::make_index_sequence<sizeof...(Ts)>{}, tup);
}
