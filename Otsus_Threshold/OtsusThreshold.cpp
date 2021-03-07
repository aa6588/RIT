/** Implementation file for finding Otsu's threshold
 *
 *  \file ipcv/otsus_threshold/OtsusThreshold.cpp
 *  \author Carl Salvaggio, Ph.D. (salvaggio@cis.rit.edu)
 *  \edited: Andrea Avendano (aa6588@rit.edu)
 *  \date 12 Sep 2020
 */

#include "OtsusThreshold.h"

#include <iostream>

#include "imgs/ipcv/utils/Utils.h"

using namespace std;

namespace ipcv {

/** Find Otsu's threshold for each channel of a 3-channel (color) image
 *
 *  \param[in] src          source cv::Mat of CV_8UC3
 *  \param[out] threshold   threshold values for each channel of a 3-channel
 *                          color image in cv::Vec3b
 */
bool OtsusThreshold(const cv::Mat &src, cv::Vec3b &threshold) {
  threshold = cv::Vec3b();
  cv::Mat array = cv::Mat_<double>::zeros(3, 256);

  // Find histogram of src
  cv::Mat_<int> src_h;
  ipcv::Histogram(src, src_h);

  // Compute PDF for src histogram
  cv::Mat src_pdf;
  ipcv::HistogramToPdf(src_h, src_pdf);

  // Find the min and max digital counts
  int DCmin = 0;
  int DCmax = 0;
  for (int chan = 0; chan < 3; chan++) {
    for (int count = 0; count < 256; count++) {
      if (src_pdf.at<double>(chan, count) != 0) {
        DCmin = count;
        break;
      }
    }
    for (int count2 = 255; count2 >= 0; count2--) {
      if (src_pdf.at<double>(chan, count2) != 0) {
        DCmax = count2;
        break;
      }
    }

    // compute the mean of the pdf
    double mean = 0;
    for (int dc = DCmin; dc <= DCmax; dc++) {
      mean += ((dc + 1) * src_pdf.at<double>(chan, dc));
    }
    // compute the zeroth and first order cummulative moments of the histogram
    // up to the kth term
    for (int k = DCmin; k <= DCmax; k++) {
      double mu_k = 0;
      double w_k = 0;
      for (int DC = DCmin; DC <= k; DC++) {
        mu_k += ((DC + 1) * src_pdf.at<double>(chan, DC));
        w_k += (src_pdf.at<double>(chan, DC));
      }
      // compute the means of class C0 and C1
      double mu0 = 0;
      double mu1 = 0;
      mu0 = mu_k / w_k;
      mu1 = (mean - mu_k) / (1 - w_k);

      // create an array of the between class variances
      double variance = 0;
      variance = (w_k * (1 - w_k)) * (mu1 - mu0) * (mu1 - mu0);
      array.at<double>(chan, k) = variance;
    }
  }
  // separate the array into 3 color channels
  cv::Mat blue = cv::Mat_<double>::zeros(1, 256);
  cv::Mat green = cv::Mat_<double>::zeros(1, 256);
  cv::Mat red = cv::Mat_<double>::zeros(1, 256);

  array.row(0).copyTo(blue.row(0));
  array.row(1).copyTo(green.row(0));
  array.row(2).copyTo(red.row(0));
  // Find the corresponding k value of the maximum for each array
  cv::Point maxIdx_b;
  cv::Point maxIdx_g;
  cv::Point maxIdx_r;
  cv::minMaxLoc(blue, NULL, NULL, NULL, &maxIdx_b);
  cv::minMaxLoc(green, NULL, NULL, NULL, &maxIdx_g);
  cv::minMaxLoc(red, NULL, NULL, NULL, &maxIdx_r);

  // input the optimal threshold for each array into theshold vector
  threshold[0] = maxIdx_b.x;
  threshold[1] = maxIdx_g.x;
  threshold[2] = maxIdx_r.x;

  return true;
}
} // namespace ipcv
