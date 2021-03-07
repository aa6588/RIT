/** Implementation file for finding map coordinates for an RST transformation
 *
 *  \file ipcv/geometric_transformation/MapRST.cpp
 *  \author Carl Salvaggio, Ph.D. (salvaggio@cis.rit.edu)
 *  \edited: Andrea Avendano (aa6588@rit.edu)
 *  \date 26 Sept 2020
 */

#include "MapRST.h"

#include <iostream>

#include <eigen3/Eigen/Dense>

using namespace std;

namespace ipcv {

/** Find the map coordinates (map1, map2) for an RST transformation
 *
 *  \param[in] src           source cv::Mat of CV_8UC3
 *  \param[in] angle         rotation angle (CCW) [radians]
 *  \param[in] scale_x       horizontal scale
 *  \param[in] scale_y       vertical scale
 *  \param[in] translation_x horizontal translation [+ right]
 *  \param[in] translation_y vertical translation [+ up]
 *  \param[out] map1         cv::Mat of CV_32FC1 (size of the destination map)
 *                           containing the horizontal (x) coordinates at
 *                           which to resample the source data
 *  \param[out] map2         cv::Mat of CV_32FC1 (size of the destination map)
 *                           containing the vertical (y) coordinates at
 *                           which to resample the source data
 */
bool MapRST(const cv::Mat src, const double angle, const double scale_x,
            const double scale_y, const double translation_x,
            const double translation_y, cv::Mat &map1, cv::Mat &map2) {

  // Define function matrices
  Eigen::MatrixXd scale(3, 3);
  Eigen::MatrixXd trans(3, 3);
  Eigen::MatrixXd rotate(3, 3);
  Eigen::MatrixXd rst(3, 3);
  // Scaling function
  scale << (1 / scale_x), 0, 0, 0, (1 / scale_y), 0, 0, 0, 1;
  // Rotating function
  rotate << cos(angle), sin(angle), 0, -sin(angle), cos(angle), 0, 0, 0, 1;
  // Translation function
  trans << 1, 0, translation_y, 0, 1, translation_x, 0, 0, 1;
  // RST function
  rst = rotate * scale * trans;

  // Make Map matrix

  double a = src.cols * abs(sin(angle));
  double b = src.rows * abs(cos(angle));
  double c = src.cols * abs(cos(angle));
  double d = src.rows * abs(sin(angle));
  double map_rows = scale_x * floor(a + b);
  double map_cols = scale_y * floor(c + d);  
  
  map1.create(map_rows, map_cols, CV_32FC1);
  map2.create(map_rows, map_cols, CV_32FC1);

  // Create frame coordinates
  Eigen::MatrixXd xy(3, 1);
  Eigen::MatrixXd xy_prime(3, 1);

  // For entire image
  for (int rows = 0; rows < map1.rows; rows++) {
    for (int cols = 0; cols < map2.cols; cols++) {
      // Shift origin
      xy << rows - (map1.rows / 2),  (map1.cols / 2) - cols, 1;

      // Apply transform
      xy_prime = rst * xy;

      // Shift origin back
      xy_prime(0, 0) = xy_prime(0, 0) + src.rows / 2;
      xy_prime(1, 0) = src.cols / 2 - xy_prime(1, 0);
      
      //Fill maps
      map1.at<float>(rows, cols) = xy_prime(0, 0);
      map2.at<float>(rows, cols) = xy_prime(1, 0);
    }
  }

  return true;
}
} // namespace ipcv
