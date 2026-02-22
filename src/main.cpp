#include "static_array.hpp"
#include <iostream>

int main() {
  auto a = nbody::make_static_array<int, 6>({1, 2, 3, 4, 5});
  for (auto x : a)
    std::cout << x << ' ';
}
