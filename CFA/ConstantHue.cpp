/*CFA Interpolation By Interpolation in the Chrominance Domain
 * Author: Andrea Avendano (aa6588@rit.edu)
 * Date: February 26, 2021
 */

/*The algorithm
 * - interpolate `G`
 * - compute hue for `R`,`B` channels at subsampled locations
 * - interpolate hue for all pixels in `R`,`B` channels
 * - determine chrominance `R`,`B` from hue 
 */

#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include "ConstantHue.h"

using namespace std;
using namespace cv;

namespace ipcv {

cv::Mat ConstantHue(const cv::Mat& src, string pattern) {


	// Create blue (B) and red (R) filter location masks
  int b_row;
  int b_col;
  int r_row;
  int r_col;
  if (pattern == "GBRG") {
    b_row = 0;
    b_col = 1;
    r_row = 1;
    r_col = 0;
  } else if (pattern == "GRBG") {
    b_row = 1;
    b_col = 0;
    r_row = 0;
    r_col = 1;
  } else if (pattern == "BGGR") {
    b_row = 0;
    b_col = 0;
    r_row = 1;
    r_col = 1;
  } else if (pattern == "RGGB") {
    b_row = 1;
    b_col = 1;
    r_row = 0;
    r_col = 0;
  } else {
    cerr << "Invalid CFA pattern provided: " << pattern << endl;
  }
cv::Mat b_mask = cv::Mat::zeros(src.size(), CV_16UC1);
  for (int r = b_row; r < src.rows; r += 2) {
    for (int c = b_col; c < src.cols; c += 2) {
      b_mask.at<uint16_t>(r, c) = 1;
    }
  }
  cv::Mat r_mask = cv::Mat::zeros(src.size(), CV_16UC1);
  for (int r = r_row; r < src.rows; r += 2) {
    for (int c = r_col; c < src.cols; c += 2) {
      r_mask.at<uint16_t>(r, c) = 1;
    }
  }

  // Create green (G) filter location mask
  cv::Mat g_mask = cv::Mat::ones(src.size(), CV_16UC1);
  cv::subtract(g_mask, b_mask, g_mask);
  cv::subtract(g_mask, r_mask, g_mask);

  // Initialize interpolated B, G, and R channels
  cv::Mat B;
  cv::multiply(src, b_mask, B, 1, CV_32F);
  cv::Mat G;
  cv::multiply(src, g_mask, G, 1, CV_32F);
  cv::Mat R;
  cv::multiply(src, r_mask, R,1,CV_32F); 

	//kernels
	Mat K_G =( Mat_<float>(3,3) << 0, 1, 0, 1, 4, 1, 0, 1, 0); 
	K_G *= (1.0/4.0);
	Mat K_B = (Mat_<float>(3,3) << 1, 2, 1, 2, 4, 2, 1, 2, 1);
	K_B *= (1.0/4.0);
	Mat K_R = K_B;

	// interpolate luminance G using bilinear
  cv::Mat G_total;
	filter2D(G, G_total, CV_32FC1, K_G);

//compute hue (B-G), (R-G)
	
  cv::Mat HR;
  cv::Mat HB;
  cv::Mat GR;
  cv::Mat GB;

r_mask.convertTo(r_mask, CV_32F);
b_mask.convertTo(b_mask, CV_32F);

  cv::multiply(G_total,r_mask,GR);
  cv::multiply(G_total,b_mask,GB);


  // Hue Ratio

 for ( int r = 0;r < src.rows; r++){
  for (int c = 0; c < src.cols; c++){
   R.at<float>(r,c) = R.at<float>(r,c) - GR.at<float>(r,c);

   B.at<float>(r,c) = B.at<float>(r,c) - GB.at<float>(r,c);


}
}

//interpolate hue
cv::Mat HR_interp;
cv::Mat HB_interp;

filter2D(R,HR_interp, CV_32F, K_R);
filter2D(B,HB_interp, CV_32F, K_B);

//compute chrominance
cv::Mat R_total;
cv::Mat B_total;
R_total = HR_interp + G_total;
B_total = HB_interp + G_total;

//clamp values to convert to 16-bit
float min_dc = 0.0;
  float max_dc = static_cast<float>(255);
  for (int r = 0; r < src.rows; r++) {
    for (int c = 0; c < src.cols; c++) {
      B_total.at<float>(r, c) = clamp(B_total.at<float>(r, c), min_dc, max_dc);
      G_total.at<float>(r, c) = clamp(G_total.at<float>(r, c), min_dc, max_dc);
      R_total.at<float>(r, c) = clamp(R_total.at<float>(r, c), min_dc, max_dc);
    }
  }

	cv::Mat channels[3];
 B_total.convertTo(channels[0], CV_16U);
 R_total.convertTo(channels[2], CV_16U);
 G_total.convertTo(channels[1], CV_16U);

  cv::Mat dst;
  cv::merge(channels, 3, dst);

	return dst;
}





}
