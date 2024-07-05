/* Blackbody Implementation file
 */
#include "blackbody.h"
#include <cmath>
#include <vector>

using namespace std;

namespace bb {

const double h = 6.62607004e-34;
const double b = 2.897771955e-3;
const double k = 1.38064852e-23;
const double c = 2.99792458e8;
const double top = (2 * M_PI * h * c * c) * pow(10, -6); // numerator
const vector<double> x = {8,    8.5, 9,    9.5, 10,   10.5, 11,
                         11.5, 12,  12.5, 13,  13.5, 14};

void Blackbody::set_T(double T) { temperature_ = T; }

double Blackbody::exitance(double y) {
  return (top) / (pow((y * pow(10, -6)), 5) *
                  (exp((h * c) / ((y * pow(10, -6)) * k * temperature_)) - 1));
}
double Blackbody::radiance(double y) { return (this->exitance(y)) / M_PI; }
double Blackbody::peakwavelength() { return pow(10, 6) * (b / temperature_); }

void Blackbody::vector_exitance(vector<double> &z, double y) {
  for (int i = 0; i < 13; i++) {
    y = x[i];
    z.push_back(this->exitance(y));
  }
}
void Blackbody::vector_radiance(vector<double> &r, double y) {
  for (int i = 0; i < 13; i++) {
    y = x[i];
    r.push_back(this->exitance(y) / M_PI);
  }
}
} // namespace bb
