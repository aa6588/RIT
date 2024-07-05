/* Blackbody Temperature implementation file
 */

#include "BlackbodyTemperature.h"
#include "blackbody.h"
#include <cmath>

using namespace std;

namespace bb {
double BlackbodyTemperature(double y, double tol, double radiance) {
  double low = 0;     // lowest possible temperature
  double high = 6000; // highest possible temperature
  double middle = (low + high) / 2;
  double range = high - low;

  bb::Blackbody first(3000); // using Blackbody class

  while (range > tol) {  // within the specified tolerance
    first.set_T(middle); // set the temperature to the middle of the range
    if (first.radiance(y) >
        radiance) {  // if calculated radiance is higher than given radiance
      high = middle; // top limit is moved down
    } else {
      low = middle; // bottom limit is moved up
    }
    middle = (low + high) / 2; // redefine middle
    range = high - low;        // redefine range
  }
  return middle; // return temperature that matches given radiance
}
} // namespace bb
