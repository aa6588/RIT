/** Implementation file for computing a CDF from a histogram
 *
 *  \file ipcv/utils/HistogramToCdf.cpp
 *  \author Carl Salvaggio, Ph.D. (salvaggio@cis.rit.edu)
 *  \edited Andrea Avendano (aa6588@rit.edu)
 *  \date 27 April 2020
 */

#include "HistogramToCdf.h"

#include "imgs/ipcv/utils/HistogramToPdf.h"

namespace ipcv {

void HistogramToCdf(const cv::Mat &h, cv::Mat &cdf) {
  cv::Mat pdf;
  HistogramToPdf(h, pdf);

  cdf = cv::Mat::zeros(h.size(), CV_64F);
  for (double j = 0; j < h.rows; j++) {
    for (double i = 0; i < h.cols; i++) {
      cv::Mat range = pdf(cv::Range(j, j + 1),
                          cv::Range(0, i)); // range which to sum up pdf values
      cdf.at<double>(j, i) = 2 * cv::sum(range)[0]; // sum pdf values
    }
  }
}
} // namespace ipcv
