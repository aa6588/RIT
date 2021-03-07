/** Implementation file for computing the 2D DFT of a matrix
 *
 *  \file ipcv/utils/Dft2.cpp
 *  \author Carl Salvaggio, Ph.D. (salvaggio@cis.rit.edu)
 *  \edited: Andrea Avendano (aa6588@rit.edu)
 *  \date 14 November 2020
 */

#include "Dft2.h"
#include "imgs/ipcv/utils/Dft.h"
#include "imgs/ipcv/utils/DftShift.h"
#include "imgs/ipcv/utils/Utils.h"
#include <iostream>
namespace ipcv {
using namespace std;
cv::Mat Dft2(cv::Mat f, const int flag) {
  // Create the output cv::Mat (double-precision complex)
  int M = f.rows;
  int N = f.cols;
  cv::Mat F(M, N, CV_64FC2);

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

  // Use the seperable nature of the 2-dimensional DFT to compute the
  // transformation by computing the column transforms for the original
  // matrix, followed by the row transforms of the column transformed
  // coefficients

  // define tranpose mats
  cv::Mat f_trans;
  cv::Mat f_trans_r;
  // row transform
  cv::transpose(f_complex, f_trans);
  for (int row = 0; row < f_complex.rows; row++) {
    f_trans.row(row) = ipcv::Dft(f_trans.row(row).t(), flag).t();
  }
  // column transform of transfomed rows
  cv::transpose(f_trans, f_trans_r);
  for (int col = 0; col < f_complex.cols; col++) {
    f_trans_r.row(col) = ipcv::Dft(f_trans_r.row(col).t(), flag).t();
  }

  F = f_trans_r;

  return F;
}
} // namespace ipcv
