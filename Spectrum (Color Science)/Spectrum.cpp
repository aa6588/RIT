/*Spectrum Implementation file
 * Author: Andrea Avendano (aa6588@rit.edu)
 * Date: April 20, 2020
 */

#include "imgs/color/spectrum/Spectrum.h"
#include "imgs/color/color.h"
#include "imgs/numerical/interpolation/interpolation.h"
#include <eigen3/Eigen/Dense>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

namespace color {
using namespace std;

// Illuminant and observer to XYZ function

Eigen::Vector3d Spectrum::Ref2XYZ(int illuminant_enum, int observer_enum) {

  // Define observer and illuminant functions
  Eigen::VectorXd xbar = cie.xbar(observer_enum);
  Eigen::VectorXd ybar = cie.ybar(observer_enum);
  Eigen::VectorXd zbar = cie.zbar(observer_enum);
  Eigen::VectorXd ill = cie.reference_illuminant(illuminant_enum);

  // Define interpolated versions
  Eigen::VectorXd cie_wavelength = cie.wavelengths();
  Eigen::VectorXd ill_interp;
  Eigen::VectorXd xbar_interp;
  Eigen::VectorXd ybar_interp;
  Eigen::VectorXd zbar_interp;

  // Check if the reference wavelengths equal the cie wavelengths
  if ((wavelength_[1] - wavelength_[0]) !=
      (cie_wavelength[1] - cie_wavelength[0])) {

    // Interpolate illuminant and observer data to be in the
    // same wavelength centers as the data in the file.
    ill_interp = numerical::Interp1(wavelength_, cie_wavelength, ill);
    xbar_interp = numerical::Interp1(wavelength_, cie_wavelength, xbar);
    ybar_interp = numerical::Interp1(wavelength_, cie_wavelength, ybar);
    zbar_interp = numerical::Interp1(wavelength_, cie_wavelength, zbar);

  } else {
    ill_interp = ill;
    xbar_interp = xbar;
    ybar_interp = ybar;
    zbar_interp = zbar;
  }

  // calculate xyz values
  Eigen::Vector3d xyz;
  Eigen::VectorXd n = ill_interp.array() * ybar_interp.array();
  Eigen::VectorXd x =
      (ill_interp.array() * reflectance_.array()) * xbar_interp.array();
  Eigen::VectorXd y =
      (ill_interp.array() * reflectance_.array()) * ybar_interp.array();
  Eigen::VectorXd z =
      (ill_interp.array() * reflectance_.array()) * zbar_interp.array();

  xyz[0] = (1 / n.sum()) * x.sum();
  xyz[1] = (1 / n.sum()) * y.sum();
  xyz[2] = (1 / n.sum()) * z.sum();

  return xyz;
}

// Illuminant and observer to sRBG function
Eigen::Vector3d Spectrum::Ref2sRGB(int illuminant_enum, int observer_enum,
                                   bool scaling) {

  // linear transformation matrix
  Eigen::Matrix<double, 3, 3> rgb;
  rgb << 3.2404542, -1.5371385, -0.4985314, -0.9692660, 1.8760108, 0.0415560,
      0.0556434, -0.2040259, 1.0572252;

  // Scale XYZ values to [0,1] range
  Eigen::Vector3d XYZ = Ref2XYZ(illuminant_enum, observer_enum);
  if (scaling == true) {
    XYZ = XYZ / 100;
  }

  // From linerar RGB values, calculate them to sRGB to be displayed
  Eigen::Vector3d RGB = rgb * XYZ;
  Eigen::Vector3d sRGB;
  for (double idx; idx < sRGB.size(); idx++) {
    if (RGB[idx] <= 0.0031308) {
      sRGB[idx] = 12.92 * RGB[idx];
    } else {
      sRGB[idx] = ((1 + .055) * pow(RGB[idx], (1 / 2.4))) - .055;
    }
  }
  for (double idx; idx <sRGB.size(); idx++){
    if (sRGB[idx] < 0){
      sRGB[idx] = 0;}
    if (sRGB[idx] > 1) {
      sRGB[idx] = 1;}
} 
  return sRGB;
}

// color patch function

void Spectrum::Patch(Eigen::Vector3d sRGB) {
  Eigen::Vector3d RGB = sRGB * 255;
  int R = static_cast<int>(RGB[0]);
  int G = static_cast<int>(RGB[1]);
  int B = static_cast<int>(RGB[2]);

  // Window name
  std::string name = "(";
  name.append(std::to_string(R));
  name.append(", ");
  name.append(std::to_string(G));
  name.append(", ");
  name.append(std::to_string(B));
  name.append(")");

  // Make and display color patch
  auto RGBs = cv::Mat(256, 256, CV_8UC3, cv::Scalar(B, G, R));
  cv::imshow(name, RGBs);
  cv::waitKey(0);
  auto k = cv::waitKey(0);
  if (k == 27) {
    cv::destroyWindow(name);
  }
}

} // namespace color
