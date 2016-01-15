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

#ifndef UTIL_H
#define UTIL_H

namespace hana = boost::hana;
template <class Tup> auto tuple_ref(Tup& tup) {
  return hana::transform(tup, [](auto& t) { return std::ref(t); });
}

template <class... Ts> constexpr auto has_duplicates() {
  constexpr auto comp_tuple = hana::tuple_t<Ts...>;
  return hana::size(comp_tuple) ==
         hana::size(hana::to<hana::set_tag>(comp_tuple));
}
template <typename Iterable, typename T>
constexpr auto index_of(Iterable const& iterable, T const& element) {
  auto size = hana::size(iterable);
  auto dropped = decltype(
      hana::size(hana::drop_while(iterable, hana::not_equal.to(element)))){};
  return size - dropped;
}

template <class... Ts> void print(Ts&&... ts) {
  auto t = hana::make_tuple(std::forward<Ts>(ts)...);
  hana::for_each(t, [](auto&& v) { std::cout << v << " "; });
  std::cout << std::endl;
}

template <class Tup, class T> constexpr auto contains_type(Tup&& tup, T&) {
  return hana::contains(tup, hana::type_c<T>);
}
template <class TypeTup, class ValueTup>
constexpr auto contains_all_types(TypeTup&& tup_type, ValueTup&& tup_value) {
  auto filter_tup = hana::filter(tup_value, [&](auto& v) {
    return hana::contains(tup_type, hana::decltype_(v));
  });
  return hana::equal(hana::size(tup_type), hana::size(filter_tup));
}
template <class TypeTup, class TypeTup2>
constexpr auto contains_all_types_t(TypeTup&& tup_type, TypeTup2&& tup_value) {
  auto filter_tup = hana::filter(
      tup_value, [&](auto& v) { return hana::contains(tup_type, v); });
  return hana::equal(hana::size(tup_type), hana::size(filter_tup));
}

template <size_t... Indices, class Tup>
auto to_hana_tup_impl(std::index_sequence<Indices...>, Tup&& tup) {
  return hana::make_tuple(std::ref(std::get<Indices>(tup))...);
}
template <class... Ts> auto to_hana_tup(std::tuple<Ts&...>& tup) {
  return to_hana_tup_impl(std::make_index_sequence<sizeof...(Ts)>{}, tup);
}
template <class T1, class T2> auto to_hana_tup(std::pair<T1, T2>& tup) {
  return to_hana_tup_impl(std::make_index_sequence<2>{}, tup);
}

namespace internal {

#ifdef _MSC_VER
#define FUNC_MACRO __FUNCTION__
static const char FRONT[] = "GetTypeNameHelper<";
static const char BACK[] = ">::GetTypeName";
#else
#define FUNC_MACRO __PRETTY_FUNCTION__
static const char FRONT[] = "T = ";
static const char BACK[] = ";";
static const char BACK_ALT[] = "]";
#endif

template <typename T> struct GetTypeNameHelper {
  static const std::string GetTypeName(void) {

    std::string func_str = FUNC_MACRO;
    size_t front_pos = func_str.find(FRONT) + sizeof(FRONT) - 1;
    size_t back_pos = func_str.find(BACK);

#ifndef _MSC_VER
    if (back_pos == std::string::npos)
      back_pos = func_str.find(BACK_ALT);
#endif

    if (front_pos == std::string::npos || back_pos == std::string::npos)
      return "GetTypeNameError";

    return func_str.substr(front_pos, back_pos - front_pos);
  }
};
}
template <typename T> const std::string get_type_name(void) {
  return internal::GetTypeNameHelper<T>::GetTypeName();
}

#endif // UTIL_H
