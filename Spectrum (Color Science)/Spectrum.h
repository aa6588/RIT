/* Spectrum class Header file
 * Author: Andrea Avendano (aa6588@rit.edu)
 * Date: April 20, 2020
 */
#pragma once

#include "imgs/color/color.h"
#include <eigen3/Eigen/Dense>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace color {
using namespace std;
color::CIE cie;

class Spectrum {
public:
  /*Default constructor
   *creates a Spectrum object on the same wavelength centers as the CIE
   *reference data and exhibits a uniform 100% reflectance
   */

  Spectrum() {
    Eigen::VectorXd w = cie.wavelengths();
    wavelength_ = w;
    reflectance_ = Eigen::VectorXd::Ones(w.size());
  }

  /*Parametized constructor
   * Read data from a .csv file and input into eigen vectors
   */

  Spectrum(std::string filename) {
    // Check if file exisits
    auto exists = std::filesystem::exists(filename);
    if (exists) {
      auto msg = "Found specified file";
      cout << msg << endl;
    } else {
      auto msg = "Specified file not found, exiting ...";
      throw std::runtime_error(msg);
    }

    // Create vectors to hold comma-separated data
    std::vector<double> x;
    std::vector<double> y;

    // Open the input file and instantiate the input file stream
    std::ifstream file(filename);

    // Read the first line as the header
    std::string axes;
    std::getline(file, axes);

    // Read each line from the file, create a string stream from each line,
    // parse the string stream into the indivudually delimited string
    // elements, convert the string elements to numeric types, and push each
    // element on to its respective vector
    std::string line;
    while (std::getline(file, line)) {
      std::stringstream ss(line);
      std::string str;
      std::getline(ss, str, ',');
      x.push_back(stod(str));
      std::getline(ss, str, ',');
      y.push_back(stod(str));
    }
    // turn std::vectors to Eigen::VectorsXd
    wavelength_ =
        Eigen::Map<Eigen::VectorXd, Eigen::Unaligned>(x.data(), x.size());
    reflectance_ =
        Eigen::Map<Eigen::VectorXd, Eigen::Unaligned>(y.data(), y.size());
  }

  /*Wavelength getter
   */
  Eigen::VectorXd get_Wavelength() const { return wavelength_; }

  /*Reflectance getter
   */
  Eigen::VectorXd get_Reflectance() const { return reflectance_; }

  /* Illuminant and observer to XYZ function
   * param[in] illuminant function
   * param[in] observer function (2 or 10 deg)
   */
  Eigen::Vector3d Ref2XYZ(int illuminant_enum, int observer_enum);

  /*Illuminant and observer to sRBG function
   *param[in] illuminant function
   *param[in] observer function (2 or 10 deg)
   */
  Eigen::Vector3d Ref2sRGB(int illuminant_enum, int observer_enum,
                           bool scaling);

  /*Creates and displays a color patch from RGB values
   *param[in] sRGB values
   */
  void Patch(Eigen::Vector3d sRGB);

private:
  Eigen::VectorXd wavelength_;
  Eigen::VectorXd reflectance_;
};
} // namespace color
