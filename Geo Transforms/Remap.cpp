/** Implementation file for remapping source values to map locations
 *
 *  \file ipcv/geometric_transformation/Remap.cpp
 *  \author Carl Salvaggio, Ph.D. (salvaggio@cis.rit.edu)
 *  \edited: Andrea Avendano (aa6588@rit.edu)
 *  \date 26 Sept 2020
 */

#include "Remap.h"

#include <iostream>

using namespace std;

namespace ipcv {

/** Remap source values to the destination array at map1, map2 locations
 *
 *  \param[in] src            source cv::Mat of CV_8UC3
 *  \param[out] dst           destination cv::Mat of CV_8UC3 for remapped values
 *  \param[in] map1           cv::Mat of CV_32FC1 (size of the destination map)
 *                            containing the horizontal (x) coordinates at
 *                            which to resample the source data
 *  \param[in] map2           cv::Mat of CV_32FC1 (size of the destination map)
 *                            containing the vertical (y) coordinates at
 *                            which to resample the source data
 *  \param[in] interpolation  interpolation to be used for resampling
 *  \param[in] border_mode    border mode to be used for out of bounds pixels
 *  \param[in] border_value   border value to be used when constant border mode
 *                            is to be used
 */
bool Remap(const cv::Mat &src, cv::Mat &dst, const cv::Mat &map1,
           const cv::Mat &map2, const Interpolation interpolation,
           const BorderMode border_mode, const uint8_t border_value) {
  dst.create(map1.size(), src.type());

  float x_prime;
  float y_prime;

  // Nearest Neighbor Interpolation
  if (interpolation == ipcv::Interpolation::NEAREST) {
    for (int rows = 0; rows < map1.rows; rows++) {
      for (int cols = 0; cols < map2.cols; cols++) {
        x_prime = floor(map1.at<float>(rows, cols) + 0.5);
        y_prime = floor(map2.at<float>(rows, cols) + 0.5);

        // Replicate pixels in border
        if (border_mode == ipcv::BorderMode::REPLICATE) {
          dst.at<cv::Vec3b>(rows, cols) =
              // clamp pixel values to src pixel value range
              src.at<cv::Vec3b>(clamp(x_prime, static_cast<float>(0),
                                      static_cast<float>(src.rows-1)),
                                clamp(y_prime, static_cast<float>(0),
                                      static_cast<float>(src.cols-1)));
          // constant color border
        } else if (border_mode == ipcv::BorderMode::CONSTANT) {
          if (x_prime < 0 || x_prime >= src.rows || y_prime < 0 ||
              y_prime >=
                  src.cols) { // for pixels that fall outside the src border
            dst.at<cv::Vec3b>(rows, cols) = border_value;
          } else {
            dst.at<cv::Vec3b>(rows, cols) = src.at<cv::Vec3b>(x_prime, y_prime);
          }
        }
      }
    }
  }

  // Bilinear Interpolation
  if (interpolation == ipcv::Interpolation::LINEAR) {

    for (int x = 0; x < src.rows; x++) {
      for (int y = 0; y < src.cols; y++) {
        auto DC_x_y = src.at<cv::Vec3b>(x, y);
        auto DC_x_y_1 = src.at<cv::Vec3b>(x, y + 1);
        auto DC_x_1_y = src.at<cv::Vec3b>(x + 1, y);
        auto DC_x_1_y_1 = src.at<cv::Vec3b>(x + 1, y + 1);

        for (int chan = 0; chan < 3; chan++) {
          float DC_y =
              (DC_x_1_y[chan] - DC_x_y[chan]) * (x_prime - x) + DC_x_y[chan];
          float DC_y_1 = (DC_x_1_y_1[chan] - DC_x_y_1[chan]) * (x_prime - x) +
                         DC_x_y_1[chan];

          dst.at<cv::Vec3b>(x, y)[chan] = static_cast<uint8_t>(
              floor((DC_y_1 - DC_y) * (y_prime - y) + DC_y));
        }
     /* 
   } 
    // Replicate pixels in border
    if (border_mode == ipcv::BorderMode::REPLICATE) {
      dst.at<cv::Vec3b>(x, y) =
          // clamp pixel values to src pixel value range
          src.at<cv::Vec3b>(clamp(x_prime, static_cast<float>(0),
                                  static_cast<float>(src.rows-1)),
                            clamp(y_prime, static_cast<float>(0),
                                  static_cast<float>(src.cols-1)));
      // constant color border
    } else if (border_mode == ipcv::BorderMode::CONSTANT) {
      if (x_prime < 0 || x_prime >= src.rows || y_prime < 0 ||
          y_prime >= src.cols) { // for pixels that fall outside the src border
        dst.at<cv::Vec3b>(x, y) = border_value;
      } else {
        dst.at<cv::Vec3b>(x, y) = src.at<cv::Vec3b>(x_prime, y_prime);
      }
    }
  }
*/
}
}
}
  return true;
}
} // namespace ipcv
