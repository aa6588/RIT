/* Integration header file (template functions)
 */

#include <cmath>
#include <functional>
#include <vector>
using namespace std;

template <typename V> double Integral(vector<V> &x, vector<V> &y) {
  double deltax = x[1] - x[0]; //width
  double Integral = 0;
  double sum = 0;
  for (size_t i = 1; i < x.size(); i++) { //calculate 2f(a + deltax)
    sum += (2 * y[i]);
  }
  Integral = 0.5 * deltax * (y[0] + y.back() + sum); //calculate integral

  return Integral;
}

template <typename CALLABLE>
double Integral(const CALLABLE &f, double a, double b, double n) {

  double deltaX = (b - a) / (n - 1); //width
  double Integral = 0;
  for (double i = 0; i < n - 1; i++) { 
    double x1 = a + (i * deltaX); //2f(a + deltax)
    double x2 = a + ((i + 1) * deltaX); //2f(a + 2deltaX)
    Integral += 0.5 * (x2 - x1) * (f(x1) + f(x2)); //calculate integral
  }

  return Integral;
}
