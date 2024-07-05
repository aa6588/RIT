#include <cmath>
#include <iostream>
#include <vector>

int main() {
  int xhat;
  std::cout << "Search for: ";
  std::cin >> xhat;

  std::vector<int> x = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};

  std::cout << "Searching for " << xhat << " in ";
  for (size_t idx = 0; idx < x.size(); idx++) {
    std::cout << x[idx] << " ";
  }
  std::cout << std::endl;
  std::cout << std::endl;

  size_t low = 0;
  size_t high = x.size() - 1;
  size_t middle;
  while (low <= high) {
    middle = (low + high) / 2;
    std::cout << "[" << low << ", " << middle << ", " << high << "] -> ["
              << x[low] << ", " << x[middle] << ", " << x[high] << "]"
              << std::endl;
    if (xhat > x[middle]) {
      low = middle + 1;
      std::cout << "... reset low to " << low << std::endl;
    } else if (xhat < x[middle]) {
      high = middle - 1;
      std::cout << "... reset high to " << high << std::endl;
    } else {
      std::cout << "Breaking loop ..." << std::endl;
      break;
    }
  }
  std::cout << std::endl;

  if (xhat == x[middle]) {
    std::cout << "Middle: " << middle << " -> " << x[middle] << std::endl;
  } else {
    std::cout << "Value not found, but it is between elements ";
    std::cout << high << " and " << low << std::endl;
  }
  std::cout << std::endl;
}
