/* Constant Hue Interpolation Method
 * Author: andrea avendano (aa6588@rit.edu)
 * Date: Feb 26, 2021
 */

#pragma once

#include <string>

#include <opencv2/core.hpp>

using namespace std;

namespace ipcv {

/** Interpolate CFA using constant hue interpolation
 *
 *  \param[in] src       source cv::Mat of CV_16UC1 containing CFA
 *  \param[in] pattern   a string defining the CFA layout:
 *                         'GBRG'  -  G B  Raspberry Pi (OmniVision OV5647)
 *                                    R G
 *                         'GRBG'  -  G R
 *                                    B G
 *                         'BGGR'  -  B G
 *                                    G R
 *                         'RGGB'  -  R G
 *                                    G B
 *
 *  \return              destination cv::Mat of CV_16UC3 for interpolated
 *                       3-channel (color) image
 */
cv::Mat ConstantHue(const cv::Mat& src, string pattern = "GBRG");
}
   
