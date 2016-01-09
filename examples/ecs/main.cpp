#include <string>
#include <iostream>
#include <tuple>
#include <utility>
#include <array>
#include <vector>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <experimental/optional>

#include "ecs/src/core/core.hpp"
#include "ctti/type_id.hpp"

struct position
{
  float x, y;
};
struct velocity
{
  float x, y;
};
struct name
{
  std::string name;
};

struct print_system
{
  template <class World>
  void update(World& w)
  {
    w.template update<position, velocity, name>(
      [](auto& pos, auto& vel, auto& name) {
        print(pos.x);
        print(vel.x);
        print(name.name);
      });
  }
};
auto make_monster(float x, float y, std::string n)
{
  return hana::make_tuple(position{ x, y }, name{ n });
}
template <class Handle>
void
print_handle(Handle& handle)
{
  if (handle.expired()) {
    return;
  }
  print("handle add", &(*handle.lock()->hc));
  print("handle index", handle.lock()->index);
  print("handle value", handle.lock()->get());
}
template <class... Ts>
using component_group_vector = ecs::core::component_group<std::vector, Ts...>;

#include <iostream>
#include <string>

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

template <typename T>
struct GetTypeNameHelper
{
  static const std::string GetTypeName(void)
  {

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

template <typename T>
const std::string
GetTypeName(void)
{
  return internal::GetTypeNameHelper<T>::GetTypeName();
}

int
main()
{

  using namespace hana::literals;
  // std::unordered_map<int, handle_container<int>> mhc;
  // mhc.insert(std::make_pair(1, handle_container<int>{}));
  // mhc[1].emplace_back(1);
  // mhc[1].emplace_back(2);
  // mhc[1].emplace_back(3);

  // auto wp1 = mhc[1].get_handle(0);

  // mhc[1].emplace_back(4);
  // mhc[1].emplace_back(5);
  // mhc[1].emplace_back(6);

  // mhc.insert(std::make_pair(2, handle_container<int>{}));
  // mhc.insert(std::make_pair(3, handle_container<int>{}));
  // mhc.insert(std::make_pair(4, handle_container<int>{}));

  // mhc[1].emplace_back(1);
  // mhc[1].emplace_back(1);
  // mhc[1].emplace_back(1);
  // mhc[1].erase(0);

  // print_handle(wp1);

  // print_handle(wp);
  // print_handle(wp1);
  // print_handle(wp2);

  // auto eq = [](const int &a, const int &b) { return a == b; };
  // auto hash = [](const int &a) -> std::size_t const { return a; };
  // std::unordered_set<int, decltype(hash), decltype(eq)> s(0, hash, eq);

  // auto eq1 = [](const std::shared_ptr<handle<int>> &a,
  //              const std::shared_ptr<handle<int>> &b) {
  //  return a->index == b->index;
  //};
  // auto hash1 = [](const std::shared_ptr<handle<int>> &a) -> std::size_t const
  // {
  //  return a->index;
  //};
  // std::unordered_set<std::shared_ptr<handle<int>>, decltype(hash1),
  //                   decltype(eq1)> s1(0, hash1, eq1);

  // s1.insert(sp1);
  // s1.insert(sp2);

  // s1.erase(sp1);
  // sp1->index = 100;
  // s1.insert(sp1);

  // print((*s1.find(sp1))->index);

  // for(auto p: hc.handle_map){
  //  print(std::get<0>(p), ", ", std::get<1>(p)->index);
  //}
  // for(auto p: hc.container){
  //  print("con ", p);
  //}
  // if (!wp.expired()) {
  //  print(wp.lock()->get());
  //}
  // if (!h.expired()) {
  //  print(h.lock()->get());
  //}

  using cg1 = component_group_vector<position, velocity>;
  using cg2 = component_group_vector<velocity>;
  using cg3 = component_group_vector<velocity, name>;
  using cg4 = component_group_vector<position, name, velocity>;
  using cg5 = component_group_vector<position, name>;

  using cg = ecs::core::component_groups<cg1, cg2, cg3, cg4, cg5>;

  auto w = ecs::core::world<cg>{};
  std::cout << GetTypeName<std::make_integer_sequence<std::size_t, 10>>()
            << std::endl;
  auto sg = ecs::core::make_systems_group(w, print_system{});
  w.add(make_monster(10, 5, "Monster3"));
  w.add(position{1, 2}, velocity{10, 10});
  w.add(position{1, 2}, velocity{10, 10});
  //auto rcg1 = w.get_cg<cg1>().get_range<position, velocity>();
  //auto r1cg1 = ranges::view::zip(
  //    rcg1, ranges::view::closed_ints(0ul, ranges::size(rcg1)));
  w.add(velocity{11, 10});
  w.add(velocity{12, 10}, name{"Monster"});
  w.add(position{42, 24}, name{"Monster1"}, velocity{5, 5});
  w.add(position{42, 24}, name{"Monster1"}, velocity{5, 5});
  w.add(position{42, 24}, name{"Monster1"}, velocity{5, 5});
  //for (auto i : ranges::view::closed_ints(1, 10)) {
  //  //}
  //  std::vector<position> pv{position{5, 5}, position{5, 5}};
  //  std::vector<velocity> vv{velocity{10, 10}, velocity{10, 10}};
  // auto r = ranges::view::zip(ranges::view::all(pv), ranges::view::all(vv));
  // constexpr size_t zero = 0;
  // auto r1 =
  //    ranges::view::zip(r, ranges::view::closed_ints(zero, ranges::size(r)));
  // for (auto t : r1) {
  //  print(std::get<1>(t));
  //}
  // std::vector<int> iv{1,2,3,4,5,6};
  // std::vector<float> fv{1,2,3,4,5,6};
  // auto r = ranges::view::zip(ranges::view::all(iv), ranges::view::all(fv));
  // constexpr size_t zero = 0;
  // auto r1 = ranges::view::zip(r, ranges::view::closed_ints(zero,
  // ranges::size(r)));
  // for(auto t: r1){
  //  std::cout << std::get<1>(t) << std::endl;
  //}
}
