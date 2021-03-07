/** Implementation file for image enhancement using histogram matching
 *
 *  \file ipcv/histogram_enhancement/MatchingLut.cpp
 *  \author Carl Salvaggio, Ph.D. (salvaggio@cis.rit.edu)
 *  \edited: Andrea Avendano (aa6588@rit.edu)
 *  \date 5 Sept 2020
 */

#include "MatchingLut.h"

#include <iostream>

#include "imgs/ipcv/utils/Utils.h"

using namespace std;

namespace ipcv {

/** Create a 3-channel (color) LUT using histogram matching
 *
 *  \param[in] src   source cv::Mat of CV_8UC3
 *  \param[in] h     the histogram in cv:Mat(3, 256) that the
 *                   source is to be matched to
 *  \param[out] lut  3-channel look up table in cv::Mat(3, 256)
 */
bool MatchingLut(const cv::Mat &src, const cv::Mat &h, cv::Mat &lut) {

  // compute histogram of src
  cv::Mat_<int> src_h;
  ipcv::Histogram(src, src_h);

  // compute  cdf of src
  cv::Mat srcCdf;
  ipcv::HistogramToCdf(src_h, srcCdf);

  // compute cdf of tgt
  cv::Mat tgtCdf;
  ipcv::HistogramToCdf(h, tgtCdf);

  // Create LUT
  lut = cv::Mat_<uint8_t>::zeros(3, 256);
  cv::Mat difference_cdf = cv::Mat_<double>::zeros(1, 256);

  double min, max;
  cv::Point minIdx;
  cv::Point maxIdx;

  for (int chan = 0; chan < 3; chan++) {
    for (int dc = 0; dc < 256; dc++) {
      double C_src = srcCdf.at<double>(chan, dc);
      for (int tgt_col = 0; tgt_col < 256; tgt_col++) {
        difference_cdf.at<double>(tgt_col) =
            abs(tgtCdf.at<double>(chan, tgt_col) - C_src);
      }
      //Find the closest matching cdf value
      cv::minMaxLoc(difference_cdf, &min, &max, &minIdx, &maxIdx);
      lut.at<uint8_t>(chan, dc) = minIdx.x;
    }
  }
  return true;
}
} // namespace ipcv
