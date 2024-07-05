#include <iostream>
#include <vector>

using namespace std;

void foo(std::vector<double>& v) {
  cout << "In foo(): Address of vector is " << &v << endl;
  cout << "In foo() [before change]: ";
  for (size_t idx = 0; idx < v.size(); idx++) {
    cout << v[idx] << " ";
  }
  cout << endl;

  v[2] = 17;

  cout << "In foo() [after change]: ";
  for (size_t idx = 0; idx < v.size(); idx++) {
    cout << v[idx] << " ";
  }
  cout << endl;
}

int main() {
  std::vector<double> v = {0, 1, 2, 3, 4, 5};
  cout << "In main(): Address of vector is " << &v << endl;
  cout << "In main() [before call]: ";
  for (size_t idx = 0; idx < v.size(); idx++) {
    cout << v[idx] << " ";
  }
  cout << endl;

  foo(v);

  cout << "In main() [after call]: ";
  for (size_t idx = 0; idx < v.size(); idx++) {
    cout << v[idx] << " ";
  }
  cout << endl;
}
