#include <iostream>
#include <mavsentinel/version.hpp>

int main() {
  std::cout << "mavsentinel " << mavsentinel::version() << '\n';

  return 0;
}
