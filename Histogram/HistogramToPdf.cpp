/** Implementation file for computing the PDF from a histogram
 *
 *  \file ipcv/utils/HistogramToPdf.cpp
 *  \author Carl Salvaggio, Ph.D. (salvaggio@cis.rit.edu)
 *  \edited Andrea Avendano (aa6588@rit.edu)
 *  \date 27 April 2020
 */

#include "HistogramToPdf.h"

namespace ipcv {

void HistogramToPdf(const cv::Mat &h, cv::Mat &pdf) {
  pdf = cv::Mat::zeros(h.size(), CV_64F);

  for (double chan = 0; chan < 3; chan++) {
    for (double i = 0; i < h.cols; i++) {
      double pixel_sum = cv::sum(h)[0] / 3; // sum of all pixel for each channel
      pdf.at<double>(chan, i) = static_cast<double>(h.at<int>(chan, i)) /
                                (pixel_sum * 2); // digital count /  pixel count for 
    }
  }
}
} // namespace ipcv
