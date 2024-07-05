#include <iostream>
#include <vector>

using namespace std;

int main() {
  std::vector<double> a;
  for (size_t idx = 0; idx < 10; idx++) {
    a.push_back(idx);
  }

  //  cout << a << endl;

  // for (double value : a) {
  //  cout << value << endl;
  //}

  auto print_vector = [](auto& vector) {
    for (auto element : vector) {
      cout << element << endl;
    }
  };

  print_vector(a);

  auto print_vectors = [](auto& vector1, auto& vector2) {
    auto number_of_elements = vector1.size();
    for (size_t idx = 0; idx < number_of_elements; idx++) {
      cout << vector1[idx] << ", " << vector2[idx] << endl;
    }
  };

  print_vectors(a, a);

  auto reverse = [](auto& vector) {
    auto reversed_vector = vector;
    auto number_of_elements = vector.size();
    for (size_t idx = 0; idx < number_of_elements; idx++) {
      reversed_vector[number_of_elements - idx - 1] = vector[idx];
    }
    return reversed_vector;
  };

  auto e = reverse(a);

  print_vector(e);

  auto reverse_inplace = [](auto& vector) {
    auto number_of_elements = vector.size();
    for (size_t idx = 0; idx < number_of_elements / 2; idx++) {
      auto tmp = vector[number_of_elements - idx - 1];
      vector[number_of_elements - idx - 1] = vector[idx];
      vector[idx] = tmp;
    }
  };

  reverse_inplace(a);

  print_vector(a);

  auto swap = [](auto& element1, auto& element2) {
    auto tmp = element2;
    element2 = element1;
    element1 = tmp;
  };

  swap(a[3], a[7]);

  print_vector(a);
}
