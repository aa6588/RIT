/** Implementation file for image filtering
 *
 *  \file ipcv/spatial_filtering/Filter2D.cpp
 *  \author Carl Salvaggio, Ph.D. (salvaggio@cis.rit.edu)
 *  \edited Andrea Avendano (aa6588@rit.edu)
 *  \date 10 Oct 2020
 */

#include "Filter2D.h"

#include <iostream>

using namespace std;

namespace ipcv {

/** Correlates an image with the provided kernel
 *
 *  \param[in] src          source cv::Mat of CV_8UC3
 *  \param[out] dst         destination cv::Mat of ddepth type
 *  \param[in] ddepth       desired depth of the destination image
 *  \param[in] kernel       convolution kernel (or rather a correlation
 *                          kernel), a single-channel floating point matrix
 *  \param[in] anchor       anchor of the kernel that indicates the relative
 *                          position of a filtered point within the kernel;
 *                          the anchor should lie within the kernel; default
 *                          value (-1,-1) means that the anchor is at the
 *                          kernel center
 *  \param[in] delta        optional value added to the filtered pixels
 *                          before storing them in dst
 *  \param[in] border_mode  pixel extrapolation method
 *  \param[in] border_value value to use for constant border mode
 */
bool Filter2D(const cv::Mat &src, cv::Mat &dst, const int ddepth,
              cv::Mat &kernel, const cv::Point anchor, const int delta,
              const BorderMode border_mode, uint8_t border_value) {

  // Create destination image
  dst.create(src.size(), ddepth);

  // find the center of the kernel
  float a = (kernel.rows - 1) / 2;
  float b = (kernel.cols - 1) / 2;

  // Iterate through color channels
  for (int chan = 0; chan < 3; chan++) {
    // Iterate through source image rows and columns
    for (int x = a; x < src.rows - a; x++) {
      cv::Vec3b *ptr = dst.ptr<cv::Vec3b>(x);
      for (int y = b; y < src.cols - b; y++) {

        int total = 0;
        // Iterate through kernel rows and columns
        for (int t = 0; t < kernel.cols; t++) {
          float *ptr1 = kernel.ptr<float>(t);
          for (int s = 0; s < kernel.rows; s++) {
            // sum the product of the weights in kernels by DCs in source image
            total += ptr1[s] * src.at<cv::Vec3b>(x + s, y + t)[chan];
          }
        }
        // insert product in destination image
        total = clamp(total, 0, 255);
        ptr[y][chan] = total + delta;
      }
    }
  }
  return true;
}
} // namespace ipcv
