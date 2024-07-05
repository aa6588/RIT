/** Implementation file for Automatic Image Registration with Affine Transforms
 *  \author: Andrea Avendano (aa6588@rit.edu)
 *  \date 12 March 2021
 */

#include "registration.h"

#include <iostream>
#include <math.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <eigen3/Eigen/Dense>
#include "imgs/ipcv/utils/Utils.h"
#include "imgs/ipcv/utils/Dft.h"
using namespace std;

namespace ipcv {


/** Finds scale, rotation, translation points of trans based on source
 *
 *  \param[in] src          source cv::Mat of CV_8UC1
 *  \param[in] trans       translated version of source cv::Mat CV_8UC1
 *  \param[out] src_log      polar log transform of src
 *  \param[out] trans_log    polar log transform of trans
 *  \param[out] dst         registered image
 */

void Register(const cv::Mat& src, cv::Mat& trans, cv::Mat& src_log, cv::Mat& trans_log, cv::Mat& dst){


//FFT
cv::Mat src_fft;
cv::Mat trans_fft;
cv::Mat source;
cv::Mat translated;
src.convertTo(source,CV_64FC2);
trans.convertTo(translated,CV_64FC2);
//cv::resize(translated,trans, src.size());
//cout << translated.size() << endl;

  double r = src.rows;
  double c = src.cols;
  double M = trans.rows;
  double N = trans.cols;
  double b = pow(10,log10(M)/N);

/*unsucessful cartesian to log polar :(
//  float p = 0;
//  float theta = 1.0 * M_PI/r;
// float x = 0;
// float y = 0;
  
Eigen::MatrixXd xy_prime(3,1);
//Eigen::MatrixXD xy(3,1);
cv::Mat map1;
cv::Mat map2;
map1.create(src.size(), CV_32FC1);
  map2.create(src.size(), CV_32FC1);

for (int i = 0; i < src.rows; i++){
  theta = i * theta;
//  double x = 0;
 // double y = 0;
  for (int j = 0; j< src.cols; j++){
    p = pow(b,j)-1;
    x = p*cos(theta) + (c / 2);
    y = p*sin(theta) + (r / 2);

    xy_prime << x, y, 1;
  //  xy << i, j, 1;

 //Fill maps
      map1.at<float>(i, j) = xy_prime(0, 0);
      map2.at<float>(i, j) = xy_prime(1, 0);
}
 x = 0;
 y = 0;
}
cv::remap(src_fft, src_log, map1, map2, cv::INTER_LINEAR, cv::BORDER_CONSTANT,
           cv::Scalar(0, 0, 0) );

*/
//define mats
cv::Mat sobel;
cv::Mat src_abs_fft;
cv::Mat trans_abs_fft;

//fft
cv::dft(source, src_fft, cv::DFT_COMPLEX_OUTPUT + cv::DFT_SCALE);
cv::dft(translated, trans_fft, cv::DFT_COMPLEX_OUTPUT + cv::DFT_SCALE);
//absolute value fft
src_abs_fft = ipcv::DftMagnitude(src_fft);
trans_abs_fft = ipcv::DftMagnitude(trans_fft);

cv::Mat IR;
cv::Mat R;
cv::Mat temp;
//numerator
cv::mulSpectrums(src_fft,trans_fft,temp, 0,true);
//denom
double inv = 1; 
IR = DftMultiply(temp,(inv/src_abs_fft));
R = DftMultiply(IR, (inv/trans_abs_fft));
//compute R
cv::dft(R, R, cv::DFT_REAL_OUTPUT + cv::DFT_INVERSE);
//trans_log = R;
//trans_log.convertTo(trans_log, CV_8UC1);
//trans.convertTo(trans, CV_8UC1);
//find max
double max;
cv::Point max_loc;
cv::minMaxLoc(R, NULL, &max, NULL, &max_loc);
//output coordinates
double tx;
double ty;

tx = fmod(N - max_loc.x , N/2);
ty = fmod(max_loc.y -M ,120/2);

cout << tx << "," << ty << endl;

cv::Mat src_lp;
cv::Mat trans_lp;
//cartesian to log polar for rotation and scale
        cv::Point2f center( (float)src.cols / 2, (float)src.rows / 2 );
        double m = 30;
        cv::logPolar(source,src_lp, center, m, cv::INTER_LINEAR + cv::WARP_FILL_OUTLIERS);

   cv::Point2f center2( (float)trans.cols / 2, (float)trans.rows / 2 );
        double m2 = 30;
        cv::logPolar(translated,trans_lp, center2, m2, cv::INTER_LINEAR + cv::WARP_FILL_OUTLIERS); 
//src_lp.convertTo(src_log, CV_8UC1);
//trans_lp.convertTo(trans_log, CV_8UC1);

cv::Mat src_lp_fft;
cv::Mat trans_lp_fft;
cv::Mat src_lp_abs_fft;
cv::Mat trans_lp_abs_fft;

//fft
//cv::Sobel(trans,sobel,CV_64FC1,1,0,3);
cv::dft(src_lp, src_lp_fft, cv::DFT_COMPLEX_OUTPUT + cv::DFT_SCALE);
cv::dft(trans_lp, trans_lp_fft, cv::DFT_COMPLEX_OUTPUT + cv::DFT_SCALE);
//high pass filter
cv::Laplacian(src_lp_fft,src_lp_fft,CV_64FC2, 3);
cv::Laplacian(trans_lp_fft,trans_lp_fft,CV_64FC2, 3);
//abs fft
src_lp_abs_fft = ipcv::DftMagnitude(src_lp_fft);
trans_lp_abs_fft = ipcv::DftMagnitude(trans_lp_fft);

cv::Mat IR2;
cv::Mat R2;
//numerator
cv::mulSpectrums(src_lp_fft,trans_lp_fft,IR2, 0,true); 
//denom
IR2 = DftMultiply(IR2,(inv/src_lp_abs_fft));
R2 = DftMultiply(IR2, (inv/trans_lp_abs_fft));
//compute R
cv::dft(R2, R2, cv::DFT_REAL_OUTPUT + cv::DFT_INVERSE);
//find max
double max2;
cv::Point max_loc2;
cv::minMaxLoc(R2, NULL, &max2, NULL, &max_loc2);
//output rotation and scale
double theta;
double scale;


//for shift, scale, angle
theta = 2* (180 * max_loc2.y )/N ;
scale = 1/ pow(b, max_loc2.x );

cout << "angle: " << theta << endl;
cout  << "scale: " << scale << endl;

src_lp.convertTo(src_log, CV_8UC1);
trans_lp.convertTo(trans_log, CV_8UC1);
trans.convertTo(trans, CV_8UC1);

};
}


