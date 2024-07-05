/* Temperature-Emissivity Separation
 * Author: Andrea Avendano (aa6588@rit.edu)
 * Date: April 03, 2020
 */

#include "imgs/plot/plot.h"
#include "imgs/radiometry/blackbody/blackbody.h"
#include "imgs/radiometry/blackbody_fit/BlackbodyFit.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {

  //from command-line get file name 
  auto filename = argv[1];

  // Create vectors to hold comma-separated data
  vector<double> x; // wavelengths
  vector<double> y; // measured radiance

  // Open file and instantiate the file stream
  std::ifstream file(filename);

  // Read the first line as the header
  std::string axes;
  std::getline(file, axes);

  /* Read each line from the file, create a string stream from each line
   * parse the string stream into the indivudually delimited string
   * elements, convert the string elements to numeric types, and push each
   * element on to its respective vector.
   */

  std::string line;

  while (std::getline(file, line)) {
    std::stringstream ss(line);
    std::string str;
    std::getline(ss, str, ',');
    x.push_back(stod(str));
    std::getline(ss, str, ',');
    y.push_back(stod(str));
  }

  /* Define wavelength interval, and acceptable temperature error
   */

  double lower = 7;
  double upper = 15;
  double t_error = .0001;
  vector<double> emissivity;

  /* Call BlackbodyFit function to find the derived material 
   * temperature when fitting a blackbody in the wavelength
   *  interval.
   */

  double temperature =
      radiometry::BlackbodyFit(x, y, lower, upper, t_error, emissivity);
  cout << "Derived Temperature = " << temperature << " [K]" << endl;

  /* Plot the wavelength and radiance vectors
   * on a range from [7,15] microns.
   */

  plot::plot2d::Params params;
  params.set_x_label("Wavelength (microns)");
  params.set_y_label("Radiance (W/m2/sr/micron)");
  params.set_x_min(lower);
  params.set_x_max(upper);
  params.set_y_min(0);
  params.set_y_max(11);
  plot::Plot2d(x, y, params);

  /* Plot the wavelength and emissivity spectrum vectors
   * on a range of [7,15] microns
   */

  plot::plot2d::Params params2; 
  params2.set_x_label("Wavelength (microns)");
  params2.set_y_label("Emissivity");
  params2.set_x_min(lower);
  params2.set_x_max(upper);
  params2.set_y_min(0);
  params2.set_y_max(1.1);
  plot::Plot2d(x, emissivity, params2);

}
