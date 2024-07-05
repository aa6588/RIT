/** Interface file for seam carving
 *  \author Andrea Avendano (aa6588@rit.edu)
 *  \date 30 April  2021
 */

#pragma once

#include <opencv2/core.hpp>

namespace ipcv {

/** Finds and removes seams of image
 *  \param[in] src          source cv::Mat of CV_8UC1
 *  \param[out] resized     new resized image
 *  \param[out] seam        cv::Mat with seam locations
 */


void Seam(cv::Mat src, cv::Mat dst, cv::Mat energy_image ,bool seam_direction);
}
