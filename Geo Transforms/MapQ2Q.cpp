/** Implementation file for mapping a source quad on to a target quad
 *
 *  \file ipcv/geometric_transformation/MapQ2Q.cpp
 *  \author Carl Salvaggio, Ph.D. (salvaggio@cis.rit.edu)
 *  \edited: Andrea Avendano (aa6588@rit.edu)
 *  \date 3 October 
 */

#include "MapQ2Q.h"

#include <iostream>

#include <eigen3/Eigen/Dense>
#include <opencv2/core.hpp>

using namespace std;

namespace ipcv {

/** Find the source coordinates (map1, map2) for a quad to quad mapping
 *
 *  \param[in] src       source cv::Mat of CV_8UC3
 *  \param[in] tgt       target cv::Mat of CV_8UC3
 *  \param[in] src_vertices
 *                       vertices cv:Point of the source quadrilateral (CW)
 *                       which is to be mapped to the target quadrilateral
 *  \param[in] tgt_vertices
 *                       vertices cv:Point of the target quadrilateral (CW)
 *                       into which the source quadrilateral is to be mapped
 *  \param[out] map1     cv::Mat of CV_32FC1 (size of the destination map)
 *                       containing the horizontal (x) coordinates at
 *                       which to resample the source data
 *  \param[out] map2     cv::Mat of CV_32FC1 (size of the destination map)
 *                       containing the vertical (y) coordinates at
 *                       which to resample the source data
 */
bool MapQ2Q(const cv::Mat src, const cv::Mat tgt,
            const vector<cv::Point> src_vertices,
             vector<cv::Point> tgt_vertices, cv::Mat &map1,
            cv::Mat &map2) {

  // Create src matrix from points
  Eigen::MatrixXf weights(3, 1);
  Eigen::MatrixXf src_inverse(3, 3);
  int src_size = src_vertices.size();

  for (int i = 0; i < src_size - 1; i++) {
    src_inverse(0, i) = src_vertices[i].x;
    src_inverse(1, i) = src_vertices[i].y;
    src_inverse(2, i) = 1;
  }
  //right hand side of equation
  Eigen::MatrixXf src_rhs(3, 1);
  src_rhs << src_vertices[3].x, src_vertices[3].y, 1;

  //create scaling values for src
  weights = src_inverse.inverse() * src_rhs;

  // make B matrix (src to vector space)
  Eigen::MatrixXf B(3, 3);
  B << weights(0) * src_vertices[0].y, weights(1) * src_vertices[1].y, weights(2) * src_vertices[2].y, 
      weights(0) * src_vertices[0].x, weights(1) * src_vertices[1].x, weights(2) * src_vertices[2].x,
      weights(0), weights(1), weights(2);

  // Create map matrix from tgt points
  Eigen::MatrixXf weights_prime(3, 1);
  Eigen::MatrixXf map_inverse(3, 3);
  int map_size = tgt_vertices.size();

  for (int ii = 0; ii < map_size - 1; ii++) {
    map_inverse(0, ii) = tgt_vertices[ii].x;
    map_inverse(1, ii) = tgt_vertices[ii].y;
    map_inverse(2, ii) = 1;
  }

  //right hand side of equation
  Eigen::MatrixXf map_rhs(3, 1);
  map_rhs << tgt_vertices[3].x, tgt_vertices[3].y, 1;

  //Create scaling values for map
  weights_prime = map_inverse.inverse() * map_rhs;

  // make A matrix (map to vector space)
  Eigen::MatrixXf A(3, 3);
  A <<  weights_prime(0) * tgt_vertices[0].x, weights_prime(1) * tgt_vertices[1].x, weights_prime(2) * tgt_vertices[2].x,
        weights_prime(0) * tgt_vertices[0].y, weights_prime(1) * tgt_vertices[1].y, weights_prime(2) * tgt_vertices[2].y, 
        weights_prime(0), weights_prime(1), weights_prime(2);

  //Create perspective transformation matrix
  Eigen::MatrixXf Pms = B * A.inverse();

  //Create maps 
  map1.create(tgt.size(), CV_32FC1);
  map2.create(tgt.size(), CV_32FC1);

  //Apply perspective transformation matrix to the image
  Eigen::MatrixXf src_points(3, 1);
  Eigen::MatrixXf map_points(3, 1);

  for (int r = 0; r < map1.rows; r++) {
    for (int c = 0; c < map1.cols; c++) {
      map_points(0, 0) = c;
      map_points(1, 0) = r;
      map_points(2, 0) = 1;
      src_points = Pms * map_points;
      map1.at<float>(r, c) = src_points(1) / src_points(2);
      map2.at<float>(r, c) = src_points(0) / src_points(2);
    }
  }

  return true;
}
} // namespace ipcv
