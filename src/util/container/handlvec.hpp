#ifndef HANDLEVEC_H
#define HANDLEVEC_H

#include <vector>
#include <unordered_map>
#include <memory>

namespace breeze {
namespace util {
namespace container {
template <class T>
class handle;

template <class T>
class handle_container
{
public:
  friend class handle<T>;
  using this_handle = handle<T>;

  std::vector<T> container;
  std::unordered_map<std::size_t, std::shared_ptr<this_handle>> handle_map;

  std::size_t size() { return container.size(); }
  template <class Arg>
  void push_back(Arg&& arg)
  {
    container.push_back(std::forward<Arg>(arg));
  }

  handle_container() = default;
  handle_container<T>& operator=(const handle_container<T>&) = delete;
  handle_container(const handle_container<T>&) = delete;
  handle_container(handle_container<T>&& other)
    : container(std::move(other.container))
    , handle_map(std::move(other.handle_map))
  {
    other.container.clear();
    other.handle_map.clear();

    // update pointer
    for (auto& sp : handle_map) {
      sp.second->hc = this;
    }
  }

  template <class... Args>
  void emplace_back(Args&&... args)
  {
    container.emplace_back(std::forward<Args>(args)...);
  }
  /* Swaps the last element with the element that we are trying to delete
   * and if there is an active handle to the last element we need to update
   * it. We also delete the active handle to the element that we are trying
   * to delete. After that we are removing the last element.
   */
  void erase(std::size_t index)
  {
    if (index >= container.size()) {
      return;
    }
    const auto last_index = get_last_index();
    swap(index, last_index);
    container.erase(std::next(container.begin(), last_index));
    handle_map.erase(last_index);
  }
  void erase(std::weak_ptr<this_handle> handle_ptr)
  {
    if (!handle_ptr.expired()) {
      erase(handle_ptr.lock()->index);
    }
  }
  void swap(std::size_t from, std::size_t to)
  {
    if (from == to) {
      return;
    }
    if (from >= container.size() || to >= container.size()) {
      return;
    }
    std::iter_swap(std::next(container.begin(), from),
                   std::next(container.begin(), to));

    if (handle_map.count(from) && handle_map.count(to)) {
      auto tmp_handle_from = handle_map[from];
      tmp_handle_from->index = to;
      auto tmp_handle_to = handle_map[to];
      tmp_handle_to->index = from;
      handle_map.erase(from);
      handle_map.erase(to);

      handle_map.insert({ to, std::move(tmp_handle_from) });
      handle_map.insert({ from, std::move(tmp_handle_to) });
    } else if (handle_map.count(from)) {
      auto tmp_handle_from = handle_map[from];
      tmp_handle_from->index = to;
      handle_map.erase(from);
      handle_map.insert({ to, std::move(tmp_handle_from) });
    } else if (handle_map.count(to)) {
      auto tmp_handle_to = handle_map[to];
      tmp_handle_to->index = from;
      handle_map.erase(to);
      handle_map.insert({ from, std::move(tmp_handle_to) });
    }
  }

  std::size_t get_last_index()
  {
    return container.size() == 0 ? 0 : container.size() - 1;
  }

  void transfair(std::weak_ptr<this_handle>& wp, handle_container<T>& other)
  {
    if (wp.expired()) {
      return;
    }
    const std::size_t last_index = get_last_index();
    const std::size_t last_index_other = other.get_last_index();
    auto sp = wp.lock();
    swap(sp->index, last_index);
    other.container.push_back(std::move(container[last_index]));
    container.erase(std::next(container.begin(), last_index));
    handle_map.erase(last_index);

    sp->hc = &other;
    sp->index = last_index_other;
    other.handle_map.insert({ last_index_other, std::move(sp) });
  }

  std::weak_ptr<this_handle> get_handle(size_t index)
  {
    if (index >= container.size()) {
      return std::weak_ptr<this_handle>{};
    }
    if (!handle_map.count(index)) {
      handle_map.insert(
        { index, std::make_shared<this_handle>(this_handle{ this, index }) });
    }
    return handle_map[index];
  }
};
template <class T>
class handle
{
public:
  friend class handle_container<T>;
  handle_container<T>* hc;
  std::size_t index;
  handle(handle_container<T>* hc, std::size_t index)
    : hc(hc)
    , index(index)
  {
  }
  auto& get() { return hc->container[index]; }
  handle(const handle<T>&) = delete;
  handle(handle<T>&& h)
    : hc(h.hc)
    , index(h.index)
  {
    h.hc = nullptr;
    h.index = 0;
  }
};
}
}
}

#endif //HANDLEVEC_H
