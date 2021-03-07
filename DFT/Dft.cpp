/** Implementation file for computing the DFT of a vector
 *
 *  \file ipcv/utils/Dft.cpp
 *  \author Carl Salvaggio, Ph.D. (salvaggio@cis.rit.edu)
 *  \edited: Andrea Avendano (aa6588@rit..edu)
 *  \date 14 November 2020
 */

#include "Dft.h"
#include <iostream>
namespace ipcv {
using namespace std;

cv::Mat Dft(cv::Mat f, const int flag) {
  const std::complex<double> i(0, 1);

  // Create the output cv::Mat (double-precision complex)
  int N = f.rows;
  cv::Mat F(N, 1, CV_64FC2);

  // Determine if the input cv::Mat is already complex (CV_64FC2).
  // If it is, create a shared pointer to the original data so no new
  // memory is required.
  // If it is not, create a local complex version of the input for
  // this computation.
  // In either case, the local version of the data to be transformed
  // will be referred to by the variable f_complex.
  cv::Mat f_complex;

  uchar depth = f.type() & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (f.type() >> CV_CN_SHIFT);
  if (depth == CV_64F && chans == 2) {
    // Create a shared pointer with the original cv::Mat
    f_complex = f;
  } else {
    // Convert the input cv::Mat to CV_64F representing the real part
    auto real = cv::Mat(f.size(), CV_64F);
    f.convertTo(real, CV_64F);

    // Create a CV_64F cv::Mat representing the imagingary part
    auto imag = cv::Mat(f.size(), CV_64F, cv::Scalar(0));

    // Merge the real and imagingary to form a CV_64FC2 cv::Mat
    std::vector<cv::Mat> channels;
    channels.push_back(real);
    channels.push_back(imag);
    cv::merge(channels, f_complex);
  }

  // Compute the forward DFT of 1D cv::Mat
  double a, norm;
  double b = 0;
  double realPart = 0;
  double imgPart = 0;
  auto n = f_complex.rows;
//Define positive exponent if inverse transform
  if (flag == DFT_INVERSE) {
    a = 1;
    norm = 1;
//Define negative exponent and scaling if forward transform
  } else {
    a = -1;
    norm = n;
  }
//for the rows of the matrix
  for (double k = 0; k < f_complex.rows; k++) {
    F.at<complex<double>>(k, 0) = 0;
    for (double x = 0; x < f_complex.rows; x++) {
//Define e and break up into real and imaginary parts (e^b = cos(b)-i*sin(b))
       b = (a * 2 * M_PI * x * (k / static_cast<double>(n)));
       realPart = cos(b);
       imgPart = sin(b);
      F.at<complex<double>>(k, 0) += 
          ((f_complex.at<complex<double>>(x, 0) * realPart) -
           (f_complex.at<complex<double>>(x, 0) * i * imgPart));
    }
    F.at<complex<double>>(k, 0) /= norm;
  }

  return F;
}
} // namespace ipcv
