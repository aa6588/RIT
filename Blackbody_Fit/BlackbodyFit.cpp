/* BlackbodyFit Implementation file
 * Author: Andrea Avendano (aa6588@rit.edu)
 * Date: April 03, 2020
 */

#include "BlackbodyFit.h"
#include "imgs/radiometry/blackbody/blackbody.h"
#include <algorithm>
#include <math.h>
#include <vector>
using namespace std;

namespace radiometry {

double BlackbodyFit(vector<double> &x, vector<double> &y, double lower,
                    double upper, double t_error, vector<double> &emissivity) {

  /* Reverse order of vectors so they are increasing
   */

  std::reverse(x.begin(), x.end());
  std::reverse(y.begin(), y.end());

  /* Find the indices of the lower and upper wavelengths
   *  in the wavelength vector. (idx_lower, idx_upper)
   */

  double idx_lower = 0;
  double idx_upper = 0;

  for (size_t idx = 0; idx < x.size(); idx++) {
    if (x[idx] < lower) {
      if (x[idx + 1] > lower) {
        idx_lower = idx;
      }
    }
  }

  for (size_t idx1 = 0; idx1 < x.size(); idx1++) {
    if (x[idx1] < upper) {
      if (x[idx1 + 1] > upper) {
        idx_upper = idx1 + 1;
      }
    }
  }

  /* Use blackbody class to compute the difference between
   * the blackbody radiance and the measured radiance spectral
   *  values within the interval.
   */

  double temp = 6000;
  double delta_t = 1000;
  radiometry::Blackbody first(temp);
  while (delta_t > t_error) {
    for (double idx = idx_lower; idx <= idx_upper; idx++) {
      if ((first.radiance(x[idx]) - y[idx]) < 0) {
        temp = temp + delta_t;
        delta_t = delta_t / 10;
        first.set_T(temp);
      }
    }
    temp = temp - delta_t;
    first.set_T(temp);
  }

  /*Calculate the emissivity spectrum from the measured relativity
   * and the spectral blackbody radiance at temperature T.
   */

  for (size_t idx2 = 0; idx2 < x.size(); idx2++) {
    emissivity.push_back(y[idx2] / first.radiance(x[idx2]));
  }

  // Reverse all vectors so they are in decreasing order

  std::reverse(x.begin(), x.end());
  std::reverse(y.begin(), y.end());
  std::reverse(emissivity.begin(), emissivity.end());

  return temp;
  }

} // namespace radiometry
