/** Implementation file for image quantization
 *
 *  \file ipcv/quantize/quantize.cpp
 *  \author Carl Salvaggio, Ph.D. (salvaggio@cis.rit.edu)
 *  \edited by Andrea Avendano (aa6588@rit.edu)
 *  \date 29 Aug 2020
 */

#include "Quantize.h"

#include <iostream>

using namespace std;

/** Perform uniform grey-level quantization on a color image
 *
 *  \param[in] src                 source cv::Mat of CV_8UC3
 *  \param[in] quantization_levels the number of levels to which to quantize
 *                                 the image
 *  \param[out] dst                destination cv:Mat of CV_8UC3
 */
void Uniform(const cv::Mat &src, const int quantization_levels, cv::Mat &dst) {

  dst.create(src.size(), src.type()); // create a matrix the same size as src
  for (int row = 0; row < src.rows; row++) {         // iterate through rows
    for (int col = 0; col < src.cols; col++) {       // iterate through cols
      for (int chan = 0; chan < 3; chan++) {         // iterate through channels
        const int scale = 256 / quantization_levels; // define # of bins
        const auto &src_dc = src.at<cv::Vec3b>(row, col); // define src dc
        auto &dst_dc = dst.at<cv::Vec3b>(row, col);       // define dst dc
        dst_dc[chan] =
            (src_dc[chan] / scale); // scales dc to destination matrix
      }
    }
  }
}
/** Perform improved grey scale quantization on a color image
 *
 *  \param[in] src                 source cv::Mat of CV_8UC3
 *  \param[in] quantization_levels the number of levels to which to quantize
 *                                 the image
 *  \param[out] dst                destination cv:Mat of CV_8UC3
 */
void Igs(const cv::Mat &src, const int quantization_levels, cv::Mat &dst) {

  dst.create(src.size(),
             src.type()); // creates a dst matrix same size and type as src
  int remainder = 0;      // define remainder
  for (int chan = 0; chan < 3; chan++) {         // iterates through channels
    for (int row = 0; row < src.rows; row++) {   // iterantes through rows
      for (int col = 0; col < src.cols; col++) { // iterates through cols
        const int scale = 256 / quantization_levels; // define # of bins
        auto &dst_dc = dst.at<cv::Vec3b>(row, col);  // define destination dc
        auto new_dc = src.at<cv::Vec3b>(row, col); // define modified source dc
        if (new_dc[chan] + remainder <=
            255) { // if the modified source dc is within threshold
          new_dc[chan] +=
              remainder; // add remainder of the previous dc to current dc
        }
        remainder =
            new_dc[chan] % scale; // calculate the remainder for the next dc

        dst_dc[chan] =
            floor(new_dc[chan] / scale); // scales the dc to destination matrix
      }
    }
  }
}

namespace ipcv {

bool Quantize(const cv::Mat &src, const int quantization_levels,
              const QuantizationType quantization_type, cv::Mat &dst) {
  dst.create(src.size(), src.type());

  switch (quantization_type) {
  case QuantizationType::uniform:
    Uniform(src, quantization_levels, dst);
    break;
  case QuantizationType::igs:
    Igs(src, quantization_levels, dst);
    break;
  default:
    cerr << "Specified quantization type is unsupported" << endl;
    return false;
  }

  return true;
}
} // namespace ipcv
