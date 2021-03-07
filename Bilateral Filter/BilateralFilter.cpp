/** Implementation file for bilateral filtering
 *
 *  \file ipcv/bilateral_filtering/BilateralFilter.cpp
 *  \author Carl Salvaggio, Ph.D. (salvaggio@cis.rit.edu)
 *  \edited: Andrea Avendano (aa6588@rit.edu)
 *  \date 27 Oct 2020
 */

#include "BilateralFilter.h"

#include <iostream>
#include <math.h>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;

namespace ipcv {

/** Bilateral filter an image
 *
 *  \param[in] src             source cv::Mat of CV_8UC3
 *  \param[out] dst            destination cv::Mat of ddepth type
 *  \param[in] sigma_distance  standard deviation of distance/closeness filter
 *  \param[in] sigma_range     standard deviation of range/similarity filter
 *  \param[in] radius          radius of the bilateral filter (if negative, use
 *                             twice the standard deviation of the distance/
 *                             closeness filter)
 *  \param[in] border_mode     pixel extrapolation method
 *  \param[in] border_value    value to use for constant border mode
 */
bool BilateralFilter(const cv::Mat &src, cv::Mat &dst,
                     const double sigma_distance, const double sigma_range,
                     const int radius, const BorderMode border_mode,
                     uint8_t border_value) {
  dst.create(src.size(), src.type());

  cv::Mat src_lab = src.clone();
  cv::Mat src_gray = src.clone();
  cv::Mat src_lab_range = src.clone();
  cv::Mat dst_lab;
  dst_lab.create(src.size(), src.type());

  /*Grayscale images take around 1 min
   *Color images take around 4-5 mins
   *It works pls be patient qwq
   */

  // for each color channel
  for (int chan = 0; chan < 3; chan++) {
    // for source rows and cols
    for (int rows = 2; rows < src.rows - 2; rows++) {
      for (int cols = 2; cols < src.cols - 2; cols++) {
        // check if the image is grayscale
        if (src_gray.at<cv::Vec3b>(rows, cols)[0] ==
            src_gray.at<cv::Vec3b>(rows, cols)[1]) {

          double filtered = 0;
          double FilterMax = 0;
          int neighbor_x = 0;
          int neighbor_y = 0;
          float y = 0;
          int size = 0;
          // if radius is negative, it is twice the distance standard deviation
          if (radius < 0) {
            size = sigma_distance * 2;
            // else, the radius is the filter size
          } else {
            size = radius;
          }
          // For the AOI
          for (int i = 0; i < size * 2; i++) {
            for (int j = 0; j < size * 2; j++) {
              neighbor_x = rows - (size - i);
              neighbor_y = cols - (size - j);
              // compute range filter
              float x = src_gray.at<cv::Vec3b>(neighbor_x, neighbor_y)[0] -
                        src_gray.at<cv::Vec3b>(rows, cols)[0];
              double range = exp(-(pow(x, 2)) / (2 * pow(sigma_range, 2)));
              // compute spatial filter
              y = float(
                  sqrt(pow(rows - neighbor_x, 2) + pow(cols - neighbor_y, 2)));
              double spatial = exp(-(pow(y, 2)) / (2 * pow(sigma_distance, 2)));
              // multiply range and spatial to get bilateral filter
              double bilateral = range * spatial;
              filtered =
                  filtered +
                  src_gray.at<cv::Vec3b>(neighbor_x, neighbor_y)[0] * bilateral;
              FilterMax = FilterMax + bilateral;
            }
          }
          // normalize and apply filter
          filtered = filtered / FilterMax;
          dst.at<cv::Vec3b>(rows, cols)[0] = filtered;
          dst.at<cv::Vec3b>(rows, cols)[1] = filtered;
          dst.at<cv::Vec3b>(rows, cols)[2] = filtered;

        }

        // For color images, use CIE Lab space and only filter on the L channel
        // for similarity filter
        else {
          double filtered = 0;
          double FilterMax = 0;
          int neighbor_x = 0;
          int neighbor_y = 0;
          float y = 0;
          int size = 0;
          // if the radius is negative, the value is twice the distance standard
          // deviation
          if (radius < 0) {
            size = sigma_distance * 2;
            // else, the radius is the size of the filter
          } else {
            size = radius;
          }
          // For range filter (similarity) convert to Lab space
          cv::cvtColor(src_lab, src_lab_range, cv::COLOR_BGR2Lab);
          // in the AOI
          for (int i = 0; i < size * 2; i++) {
            for (int j = 0; j < size * 2; j++) {
              neighbor_x = rows - (size - i);
              neighbor_y = cols - (size - j);
              // compute range filter in lab space
              float x = src_lab.at<cv::Vec3b>(neighbor_x, neighbor_y)[chan] -
                        src_lab.at<cv::Vec3b>(rows, cols)[chan];
              double range = exp(-(pow(x, 2)) / (2 * pow(sigma_range, 2)));
              // compute spatial filter in DC space
              y = float(
                  sqrt(pow(rows - neighbor_x, 2) + pow(cols - neighbor_y, 2)));
              double spatial = exp(-(pow(y, 2)) / (2 * pow(sigma_distance, 2)));
              // multiply the filters to get bilateral filter
              double bilateral = range * spatial;
              filtered = filtered +
                         src_lab.at<cv::Vec3b>(neighbor_x, neighbor_y)[chan] *
                             bilateral;

              FilterMax = FilterMax + bilateral;
            }
          }
          // normalize and apply the filter
          filtered = filtered / FilterMax;
          dst.at<cv::Vec3b>(rows, cols)[chan] = filtered;
        }
      }
    }

    // border mode replicate
    if (border_mode == ipcv::BorderMode::REPLICATE) {
      int border = 2;
      cv::copyMakeBorder(dst, dst, border, border, border, border,
                         cv::BORDER_REPLICATE);
    }
}
    return true;
  }
} // namespace ipcv
