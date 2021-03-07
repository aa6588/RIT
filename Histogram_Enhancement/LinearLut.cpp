/** Implementation file for image enhancement using linear histogram statistics
 *
 *  \file ipcv/histogram_enhancement/LinearLut.cpp
 *  \author Carl Salvaggio, Ph.D. (salvaggio@cis.rit.edu)
 *  \edited: Andrea Avendano (aa6588@rit.edu)
 *  \date 5 Sep 2020
 */

#include "LinearLut.h"

#include <iostream>

#include "imgs/ipcv/utils/Utils.h"

using namespace std;

namespace ipcv {

/** Create a 3-channel (color) LUT using linear histogram enhancement
 *
 *  \param[in] src          source cv::Mat of CV_8UC3
 *  \param[in] percentage   the total percentage to remove from the tails
 *                          of the histogram to find the extremes of the
 *                          linear enhancemnt function
 *  \param[out] lut         3-channel look up table in cv::Mat(3, 256)
 */
bool LinearLut(const cv::Mat &src, const int percentage, cv::Mat &lut) {

  // create src histogram and cdf
  cv::Mat_<int> src_h;
  cv::Mat srcCdf;
  ipcv::Histogram(src, src_h);
  ipcv::HistogramToCdf(src_h, srcCdf);

//Find and trim low and high dcs
  int dcHigh, dcLow;
  for (int count = 0; count < 256; count++) {
   if (srcCdf.at<double>(count) >= percentage / (100 * (double)(2))){
       dcLow = count;
   break; 
  }
}
  for (int count2 = 255; count2 >= 0; count2--) {
    if (srcCdf.at<double>(count2) <= 1 -  percentage /( 100 * (double)(2) )){
    dcHigh = count2;
    break;
  }
}

//Create LUT
  lut = cv::Mat_<uint8_t>::zeros(3, 256);

//Find slope and y-intercept
  double m = (255 - 0) / (dcHigh - dcLow);
  double b = (-1 * m * dcLow);

  for (int chan = 0; chan < 3; chan++) {
    for (int dc = 0; dc < 256; dc++) {
      int dc_Lut = m * dc + b;
      dc_Lut = std::clamp(dc_Lut, 0, 255);
      lut.at<uint8_t>(chan, dc) = dc_Lut;
    }
  }
  return true;
}
} // namespace ipcv
