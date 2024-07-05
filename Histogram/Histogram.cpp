/** Implementation file for computing an image histogram
 *
 *  \file ipcv/utils/Histogram.cpp
 *  \author Carl Salvaggio, Ph.D. (salvaggio@cis.rit.edu)
 *  \edited Andrea Avendano (aa6588@rit.edu)
 *  \date 27 April 2020
 */

#include "Histogram.h"

namespace ipcv {

void Histogram(const cv::Mat &src, cv::Mat &h) {
  h = cv::Mat_<int>::zeros(3, 256);

  for (int j = 0; j < src.rows; j++) {
    for (int i = 0; i < src.cols; i++) {
      h.at<int>(0, src.at<cv::Vec3b>(j, i)[0]) += 1; // Blue channel
      h.at<int>(1, src.at<cv::Vec3b>(j, i)[1]) += 1; // Green channel
      h.at<int>(2, src.at<cv::Vec3b>(j, i)[2]) += 1; // Red channel
    }
  }
}

} // namespace ipcv
