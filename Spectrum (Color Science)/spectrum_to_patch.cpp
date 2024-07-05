/* Spectrum to Patch
 * Author: Andrea Avendano (aa6588@rit.edu)
 * Date April 20, 2020
 */

#include "imgs/color/color.h"
#include "imgs/color/spectrum/Spectrum.h"
#include <eigen3/Eigen/Dense>
#include <iostream>
#include "imgs/plot/plot.h"

using namespace std;

int main(int argc, char* argv[]) {

//from command-line get file name 
  auto filename = argv[1];

color::CIE cie;
//color::Spectrum spectrum;
color::Spectrum spectrum(filename);

  int observer_enum;
  int illuminant_enum;
  bool scaling;
  cout << "Illuminant a = [0] or d65 = [1]: ";
  cin >> illuminant_enum;
  cout << "Observer two_deg = [0] or ten_deg = [1]: ";
  cin >> observer_enum;
  cout << "Scaling? true = [1] or false = [0]: ";
  cin >> scaling;
  Eigen::Vector3d XYZ = spectrum.Ref2XYZ(illuminant_enum, observer_enum);
  Eigen::Vector3d sRGB = spectrum.Ref2sRGB(illuminant_enum, observer_enum, scaling);

  cout << "Calculated XYZ tristimulus values: " << endl;
  cout << XYZ << endl;
  cout << "Calculated sRGB values: " << endl;
  cout << sRGB << endl;

  //Plot reflectance
  Eigen::VectorXd reflectance = spectrum.get_Reflectance();
  Eigen::VectorXd wavelength = spectrum.get_Wavelength();
  plot::plot2d::Params params;
  params.set_x_label("Wavelength (nm)");
  params.set_y_label("Reflectance");
  params.set_y_max(100);
  plot::Plot2d(wavelength, reflectance, params);
  
  //Plot illuminant
  Eigen::VectorXd cie_wavelength = cie.wavelengths();
  Eigen::VectorXd illuminant = cie.reference_illuminant(illuminant_enum);
  plot::plot2d::Params params1;
  params1.set_x_label("Wavelength (nm)");
  params1.set_y_label("Spectral Power");
  params1.set_y_max(300);
  plot::Plot2d(cie_wavelength, illuminant, params1);
  
  //Plot observer
  Eigen::MatrixXd obs = cie.standard_observer(observer_enum);
  Eigen::MatrixXd w(cie_wavelength.size(),3);
  w.col(0) = cie_wavelength;
  w.col(1) = cie_wavelength;
  w.col(2) = cie_wavelength;
  plot::plot2d::Params params2;
  params2.set_x_label("Wavelength (nm)");
  params2.set_y_label("Spectral Sensitivity");
  params2.set_y_max(2.3);
  plot::Plot2d(w, obs, params2);

  //Display color patch
  spectrum.Patch(sRGB);

}
