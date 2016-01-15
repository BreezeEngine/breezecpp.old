#include <string>
#include <iostream>
#include <tuple>
#include <utility>
#include <array>
#include <vector>
#include <typeinfo>
#include <functional>

#include "range/v3/view.hpp"
#include "boost/hana.hpp"
#include <boost/hana/ext/std/tuple.hpp>
#include <boost/hana/ext/std/pair.hpp>

#include "../util/util.hpp"
#include "util/container/handlvec.hpp"
namespace hana = boost::hana;
namespace ecs {
namespace core {

template <template <class...> class Container, class... Components>
struct component_group {
  static_assert(has_duplicates<Components...>(),
                "component_group contains duplicates");
  constexpr static auto comp_tup = hana::tuple_t<Components...>;
  hana::tuple<Container<Components>...> components_tuple;
  template <class... Ts> auto get_range_with_index() {
    auto r = get_range<Ts...>();
    constexpr size_t zero = 0;
    return ranges::view::zip(r,
                             ranges::view::closed_ints(zero, ranges::size(r)));
  }
  template <class... Ts> auto get_range() {
    auto filter = hana::tuple_t<Container<Ts>...>;
    const auto components_ref = tuple_ref(components_tuple);
    auto components_filter = hana::filter(components_ref, [&filter](auto cref) {
      return contains_type(filter, cref.get());
    });
    auto sorted_components_filter = hana::sort(components_filter, [&](auto&& a, auto&& b) {
      return index_of(filter, hana::decltype_(a.get())) <
             index_of(filter, hana::decltype_(b.get()));
    });
    auto components_filter_range = hana::transform(
        sorted_components_filter, [](auto&& cref) { return ranges::view::all(cref.get()); });
    return hana::unpack(components_filter_range, [](auto&... comps) {
      return ranges::view::zip(comps...);
    });
  }
  void add(Components&&... components) {
    auto tuple_comp = hana::make_tuple(std::forward<Components>(components)...);
    add(std::move(tuple_comp));
  }
  void add(hana::tuple<Components...>&& tuple_comp) {
    using namespace hana::literals;
    auto components_ref =
        hana::transform(components_tuple, [&](auto& c) { return std::ref(c); });
    auto zipped_tuple = hana::zip(components_ref, tuple_comp);
    hana::for_each(std::move(zipped_tuple),
                   [](auto&& tup) { tup[0_c].get().push_back(tup[1_c]); });
  }
};
// template<size_t... Indices, class Tup>
// auto to_hana_tup_impl(std::index_sequence<Indices...>, Tup&& tup){
//  return hana::make_tuple(std::get<Indices...>(tup));
//}
// template<class... Ts>
// auto to_hana_tup(std::tuple<Ts...>& tup){
//  return to_hana_tup_impl(std::make_index_sequence<sizeof...(Ts)>{}, tup);
//}

template <class... ComponentGroups> struct component_groups {
  static_assert(has_duplicates<ComponentGroups...>(),
                "component_groups contains duplicates");
};

template <class World, class... Systems> struct systems_group {
  World& world;
  hana::tuple<Systems...> systems_tuple;
  explicit systems_group(World& world, Systems&&... systems)
      : world(world),
        systems_tuple(hana::make_tuple(std::forward<Systems>(systems)...)) {}

  void update() {
    hana::for_each(systems_tuple, [this](auto& s) { s.update(this->world); });
  }
};
template <class World, class... Systems>
auto make_systems_group(World& world, Systems&&... systems) {
  return systems_group<World, Systems...>{ world,
                                           std::forward<Systems>(systems)... };
}

template <class ComponentGroup> struct world;
template <class... ComponentGroups>
struct world<component_groups<ComponentGroups...> > {
  hana::tuple<ComponentGroups...> component_groups_tuple;

  template <class... Components>
  void add(hana::tuple<Components...>&& components_tuple) {
    auto filter = hana::tuple_t<Components...>;
    auto cg_ref = tuple_ref(component_groups_tuple);
    auto filterd_cg_ref = hana::filter(cg_ref, [&filter](auto& cg) {
      return hana::equal(cg.get().comp_tup, filter);
    });
    constexpr auto size = decltype(hana::size(filterd_cg_ref))::value;

    static_assert(size != 0, "Empty");
    static_assert(size <= 1, "More than 1");
    hana::front(filterd_cg_ref).get().add(std::move(components_tuple));
  }
  template <class... Components> void add(Components&&... components) {
    return add(hana::make_tuple(std::forward<Components>(components)...));
  }
  template <class Cg> auto& get_cg() {
    auto types = hana::tuple_t<ComponentGroups...>;
    auto component_group_ref = hana::transform(
        component_groups_tuple, [&](auto& c) { return std::ref(c); });
    auto maybe_index = hana::filter(component_group_ref, [](auto& cgref) {
      return hana::equal(hana::decltype_(cgref.get()), hana::type_c<Cg>);
    });
    static_assert(hana::size(maybe_index) == hana::size_c<1>,
                  "Contains duplicates");
    return hana::front(maybe_index).get();
  }

  template <class... Ts> auto get_range() {
    auto filter_tup = hana::tuple_t<Ts...>;
    auto cg_ref = tuple_ref(component_groups_tuple);
    auto cg_ref_filtered = hana::filter(cg_ref, [&](auto& cg) {
      return contains_all_types_t(filter_tup, cg.get().comp_tup);
    });
    auto range = hana::transform(cg_ref_filtered, [](auto& c_ref) {
      return c_ref.get().template get_range<Ts...>();
    });
    return hana::unpack(range,
                        [](auto&&... r) { return ranges::view::concat(r...); });
  }

  template <class... Ts, class F> void update(F&& f) {
    for (auto t : get_range<Ts...>()) {
      hana::unpack(to_hana_tup(t), [&](auto&&... t) { f(t.get()...); });
    }
  }
};
}
}
