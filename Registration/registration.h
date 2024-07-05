/** Interface file for image registration
 *  \author Andrea Avendano (aa6588@rit.edu)
 *  \date 12 March 2021
 */

#pragma once

#include <opencv2/core.hpp>

namespace ipcv {

/** Finds scale, rotation, translation points of trans based on source
 *
 *  \param[in] src          source cv::Mat of CV_8UC1
 *  \param[in] trans       translated version of source cv::Mat CV_8UC1
 *  \param[out] src_log      polar log transform of src
 *  \param[out] trans_log    polar log transform of trans
 *  \param[out] dst          registered image
 */

void Register(const cv::Mat& src, cv::Mat& trans, cv::Mat& src_log, cv::Mat& trans_log, cv::Mat& dst);
}
