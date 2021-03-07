/** Implementation file for finding corner features using Harris
 *
 *  \file ipcv/corners/Harris.cpp
 *  \author Carl Salvaggio, Ph.D. (salvaggio@cis.rit.edu)
 *  \edited: Andrea Avendano (aa6588@rit.edu)
 *  \date 7 Nov 2020
 */

#include "Corners.h"

#include "opencv2/imgproc.hpp"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

using namespace std;

namespace ipcv {

/** Apply the Harris corner detector to a color image
 *
 *  \param[in] src     source cv::Mat of CV_8UC3
 *  \param[out] dst    destination cv:Mat of CV_32FC1
 *  \param[in] sigma   standard deviation of the Gaussian blur kernel
 *  \param[in] k       free parameter in the equation
 *                        dst = (lambda1)(lambda2) - k(lambda1 + lambda2)^2
 */
bool Harris(const cv::Mat &src, cv::Mat &dst, const float sigma,
            const float k) {
  cv::Mat src_gray;
  cv::cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);

  dst.create(src_gray.size(), CV_32FC1);
  // dst.create(src.size(),CV_8UC1);
  // Compute derivatives

  cv::Mat Ix;
  cv::Mat Iy;
  Ix.create(src_gray.size(), CV_32FC1);
  Iy.create(src_gray.size(), CV_32FC1);

  // Horizontal and Vertical Gradients

  cv::Mat kernelH;
  kernelH.create(3, 3, CV_32FC1);
  kernelH = 0;
  kernelH.at<float>(0, 0) = -1;
  kernelH.at<float>(0, 2) = 1;
  kernelH.at<float>(1, 0) = -1;
  kernelH.at<float>(1, 2) = 1;
  kernelH.at<float>(2, 0) = -1;
  kernelH.at<float>(2, 2) = 1;

  kernelH /= 1;

  cv::Mat kernelV;
  kernelV.create(3, 3, CV_32FC1);
  kernelV = 0;
  kernelV.at<float>(0, 0) = -1;
  kernelV.at<float>(2, 0) = 1;
  kernelV.at<float>(0, 1) = -1;
  kernelV.at<float>(2, 1) = 1;
  kernelV.at<float>(0, 2) = -1;
  kernelV.at<float>(2, 2) = 1;
  kernelV /= 1;
  cv::filter2D(src_gray, Ix, CV_32FC1, kernelH);
  cv::filter2D(src_gray, Iy, CV_32FC1, kernelV);

  // Create Hessian Matrix
  cv::Mat A;
  cv::Mat B;
  cv::Mat C;

  A.create(Ix.size(), CV_32FC1);
  B.create(Iy.size(), CV_32FC1);
  C.create(Ix.size(), CV_32FC1);
  A = Ix.mul(Ix);
  B = Iy.mul(Iy);
  C = Ix.mul(Iy);

  // Gaussian blur the matrix
  cv::GaussianBlur(A, A, cv::Size(0, 0), sigma, 0);
  cv::GaussianBlur(B, B, cv::Size(0, 0), sigma, 0);
  cv::GaussianBlur(C, C, cv::Size(0, 0), sigma, 0);

  // Compute the Harris response
  cv::Mat R;
  R.create(A.size(), CV_32FC1);

  for (int r = 0; r < Ix.rows; r++) {
    for (int c = 0; c < Ix.cols; c++) {
      float eigenA;
      float eigenB;
      float eigenC;

      eigenA = A.at<float>(r, c);
      eigenB = B.at<float>(r, c);
      eigenC = C.at<float>(r, c);

      float det = eigenA * eigenB - eigenC * eigenC;
      float trace = eigenA + eigenB;

      R.at<float>(r, c) = det - k * trace * trace;
    }
  }
  // output image, filter out corners to be 1 pixel

  cv::Mat dst2;
  R.copyTo(dst2);

  for (int row = 2; row < dst2.rows - 2; row++) {
    for (int col = 2; col < dst2.cols - 2; col++) {
      cv::Mat neighborhood(dst2, cv::Range(col - 2, col + 2),
                           cv::Range(row - 2, row + 2));
      double max;
      double min;
      cv::minMaxLoc(neighborhood, &min, &max);
      if (neighborhood.at<float>(2, 2) == max &&
          neighborhood.at<float>(2, 2) > 0) {
        dst.at<float>(row, col) = 255;
      }
    }
  }
  dst.convertTo(dst, CV_8UC1);
  return true;
}
} // namespace ipcv
