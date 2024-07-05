#include <cmath>

#include "plot_function.h"
#include "imgs/plot/plot.h"

double F(double x) { return cos(x); }

int main() {
  double a = 0;
  double b = 4 * M_PI;
  size_t n = 100;

  plot_function(F, a, b, n);
  // plot_function(cos, a, b, n);
  // plot_function([](auto x) { return cos(x); }, a, b, n);

  plot::plot2d::Params params;
  params.set_x_min(a);
  params.set_x_max(b);
  params.set_y_label("f(x)");

  plot_function(F, params);
}
