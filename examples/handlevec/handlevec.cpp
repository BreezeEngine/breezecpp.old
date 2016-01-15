#include <util/container/handlvec.hpp>
#include <iostream>
#include <boost/hana.hpp>
using namespace breeze::util::container;
struct Foo{
  int i;

  Foo(int i): i(i){}
  Foo(const Foo&) = delete;
  Foo& operator=(const Foo&) = delete;
  Foo(Foo&& f){

  }
};

int
main()
{

  auto tup = boost::hana::make_tuple(Foo{42});
  auto tup_ref = boost::hana::transform(tup, [](auto& t){
    return std::ref(t);
  });

  boost::hana::filter(tup_ref, [](auto& tref){
    auto& test = tref.get();
    return boost::hana::bool_c<true>;
  });

  breeze::util::container::handle_container<int> hc;
  auto rhc = std::ref(hc);
  rhc.get().push_back(15);

  hc.push_back(5);
  auto h1 = hc.get_handle(0);
  auto v1 = hc.get(h1);
  if(v1){
    std::cout << v1.value() << std::endl;
  }
  //auto v2 = hf.get(&h2);
  //auto v3 = hc.get(&h2);

  //std::vector<breeze::util::container::handle_id*> hids;
  //hids.push_back(&h1);
  //hids.push_back(&h2);
}
