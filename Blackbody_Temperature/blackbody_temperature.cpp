/* Blackbody Temperature
 * Author: Andrea Avendano (aa6588@rit.edu)
 * Date: February 26, 2020
 */

#include "BlackbodyTemperature.h"
#include "blackbody.h"
#include <iostream>

using namespace std;

int main() {

  double radiance; // define radiance
  double y;        // define wavelength
  double tol;      // define acceptable tolerance

  /* user interface asks for parameters used in function
   */
  cout << "Radiance [W/,^2/sr/micron]: ";
  cin >> radiance;
  cout << "Wavelength [micron]: ";
  cin >> y;
  cout << "Acceptable tolerance [K]: ";
  cin >> tol;

  /*prints out given parameters and matching temperature
   */
  cout << "Temperature = " <<  bb::BlackbodyTemperature(y, tol, radiance) << " [K]" << endl;
}
