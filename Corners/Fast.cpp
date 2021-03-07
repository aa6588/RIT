/** Implementation file for finding corner features using FAST
 *
 *  \file ipcv/corners/Fast.cpp
 *  \author Carl Salvaggio, Ph.D. (salvaggio@cis.rit.edu)
 *  \edited: Andrea Avendano (aa6588@rit.edu)
 *  \date 07 Nov 2020
 */

#include "Corners.h"

#include "opencv2/imgproc.hpp"
#include <Eigen/Dense>
#include <iostream>
#include <numeric>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <vector>

using namespace std;

namespace ipcv {

/** Apply the FAST corner detector to a color image
 *
 *  \param[in] src     source cv::Mat of CV_8UC3
 *  \param[out] dst    destination cv:Mat of CV_8UC3
 *  \param[in] difference_threshold
 *                     brightness threshold to be used to determine whether
 *                     a surrounding pixels is brighter than or darker than
 *                     the candidate corner pixel
 *  \param[in] contiguous_threshold
 *                     number of contiguous pixels that must appear in
 *                     sequence in order for a candidate pixel to be
 *                     considered a corner pixel
 *  \param[in] nonmaximal_suppression
 *                     boolean parameter indicating whether non-maximal
 *                     suppression should be used to eliminate "clumping"
 *                     of identified corner points
 */
bool Fast(const cv::Mat &src, cv::Mat &dst, const int difference_threshold,
          const int contiguous_threshold, const bool nonmaximal_supression) {
  cv::Mat src_gray;
  cv::cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);

  dst.create(src.size(), CV_32FC1);

  for (int row = 3; row < src_gray.rows - 3; row++) {
    for (int col = 3; col < src_gray.cols - 3; col++) {

      int p = src_gray.at<uint8_t>(row, col);
      Eigen::VectorXd radius(16);

      radius(0) = src_gray.at<uint8_t>(row - 3, col + 0);
      radius(1) = src_gray.at<uint8_t>(row - 3, col + 1);
      radius(2) = src_gray.at<uint8_t>(row - 2, col + 2);
      radius(3) = src_gray.at<uint8_t>(row - 1, col + 3);
      radius(4) = src_gray.at<uint8_t>(row - 0, col + 3);
      radius(5) = src_gray.at<uint8_t>(row + 1, col + 3);
      radius(6) = src_gray.at<uint8_t>(row + 2, col + 2);
      radius(7) = src_gray.at<uint8_t>(row + 3, col + 1);
      radius(8) = src_gray.at<uint8_t>(row + 3, col + 0);
      radius(9) = src_gray.at<uint8_t>(row + 3, col - 1);
      radius(10) = src_gray.at<uint8_t>(row + 2, col - 2);
      radius(11) = src_gray.at<uint8_t>(row + 1, col - 3);
      radius(12) = src_gray.at<uint8_t>(row + 0, col - 3);
      radius(13) = src_gray.at<uint8_t>(row - 1, col - 3);
      radius(14) = src_gray.at<uint8_t>(row - 2, col - 2);
      radius(15) = src_gray.at<uint8_t>(row - 3, col - 1);

      Eigen::VectorXd diff(16);
      // give pixels values based on intensity. (Darker = -1, brighter = 1)
      for (int i = 0; i < 16; i++) {
        if (radius(i) > p + difference_threshold) { // if the pixel brighter
          diff(i) = 1;
        } else if (radius(i) <
                   p - difference_threshold) { // if the pixel is darker
          diff(i) = -1;
        } else {
          diff(i) = 0;
        } // pixel is not brighter or darker
      }
      auto n = diff.size();
      double sum = 0;
      Eigen::VectorXd shifted(16);
      for (int shift = 0; shift < 16; shift++) {
         // double sum = 0;
        for (int idx = 0; idx < n; idx++) {
          shifted(idx) = diff((n + (idx - shift)) % n); // shift vector
        }

        // add up first n elements and see if they are +n or -n
        sum = shifted.block(0,0,contiguous_threshold,0).sum();

        // sum = std::accumulate(shifted.begin(), shifted.end() - (16 - n),
        if (abs(sum) == contiguous_threshold) { // if the sum is n then it
          goto stop;
        }
      }
    stop:
      dst.at<float>(row, col) = 255;
    }
  }

  dst.convertTo(dst, CV_8UC1);

  return true;
}
} // namespace ipcv
