#include <string>
#include <iostream>
#include <util/container/handlvec.hpp>
#include <ecs/core/core.hpp>
struct position {
  float x, y;
};
struct velocity {
  float x, y;
};
struct name {
  std::string name;
};

struct print_system {
  template <class World> void update(World& w) {
    w.template update<name>([](auto& name) { print(name.name); });
  }
};
struct print_pos_vel {
  template <class World> void update(World& w) {
    w.template update<position, velocity>([](auto& pos, auto& vel) {
      print(pos.x, pos.y);
      print(vel.x, vel.y);
    });
  }
};
auto make_monster(float x, float y, std::string n) {
  return hana::make_tuple(position{ x, y }, velocity{ 0, 0 }, name{ n });
}

template <class... Ts>
using component_group_handle = ecs::core::component_group<
    breeze::util::container::handle_container, Ts...>;

int main() {

  using namespace hana::literals;

  using moveable_object = component_group_handle<position, velocity>;
  using entity = component_group_handle<position, velocity, name>;
  using cg = ecs::core::component_groups<moveable_object, entity>;
  auto w = ecs::core::world<cg>{};
  auto sg = ecs::core::make_systems_group(w, print_system{}, print_pos_vel{});
  w.add(make_monster(10, 5, "Monster3"));
  w.add(make_monster(11, 5, "Monster2"));
  w.add(position{ 42, 24 }, velocity{ 5, 5 }, name{ "Monster1" });

  // moveable_object
  w.add(position{ 0, 0 }, velocity{ 10, 5 });
  sg.update();
}
