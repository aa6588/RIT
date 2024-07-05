/** Header file for Optical Music Recognition
 *  \author: Andrea Avendano (aa6588@rit.edu)
 *  \date 9 April 2021
 */


#pragma once

#include <opencv2/core.hpp>

using namespace std;

namespace ipcv {

/** Finds note types and pitch on a sheet of music
 *
 *  \param[in] src          source cv::Mat of CV_8UC1 (sheet music)
 *  \param[out] note        std::vector of note type
 *  \param[out] pitch       std::vector of pitch
 */

void OMR(const cv::Mat& src, cv::Mat binary);
}
