/********************
 * Fast Fourier Transform 
 * author: Andrea Avendano (aa6588@rit.edu)
 * date: Nov 24, 2020
 ********************/

#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "imgs/ipcv/utils/Utils.h"

int main(){
using namespace std;

  // Read image into cv::Mat
  string filename = "../data/images/misc/lenna_grayscale.pgm";
  cv::Mat src;
  src = cv::imread(filename, cv::IMREAD_GRAYSCALE);
  cout << "Image filename:" << endl;
  cout << filename << endl;
  cout << endl;

  // Perform 2-dimensional DFT
  cv::Mat dft_input;
  cv::Mat dft_log;
  src.convertTo(dft_input, CV_64F);

  cv::dft(dft_input, dft_log, cv::DFT_COMPLEX_OUTPUT + cv::DFT_SCALE);

  //show original source and DFT 
  cv::imshow("Source", src);
  cv::imshow("log|DFT|",
             ipcv::DftMagnitude(dft_log,
                                ipcv::DFT_MAGNITUDE_LOG +
                                ipcv::DFT_MAGNITUDE_CENTER +
                                ipcv::DFT_MAGNITUDE_NORMALIZE));


  // Make matrix of magnitudes of DFT 
  cv::Mat dft_mag = ipcv::DftMagnitude(dft_input);
  // Make current component matrix
  cv::Mat current_component = cv::Mat_<complex<double>>::zeros(src.rows, src.cols);
  // Make sinusoid matrix (inverse of the current component matrix)
  cv::Mat current_sinusoid = cv::Mat_<double>::zeros(src.rows, src.cols);
  // Make sum matrix which will sum the current components with each iteration
  cv::Mat summed = cv::Mat_<double>::zeros(src.rows, src.cols);
  // Make DFT matrix which will show the DFT from current components
  cv::Mat current_dft = cv::Mat_<double>::zeros(src.rows, src.cols);

  //find the current maximum value in the magnitude matrix
  cv::Mat_<cv::Point> max; //matrix for storing max coordinates
  double idx;
  double Max;
  idx = 0; //resets each time for a new maximum
  
  //while there is still maximum values (haven't turned them to 0)
  while (Max != 0) {
    cv::Point max_loc;
    // Find current maximum in magnitude matrix
    cv::minMaxLoc(dft_mag, NULL, &Max, NULL, &max_loc);
    //store the max location in max matrix
    max.push_back(max_loc);
    
    //copy current max value to the current component matrix
    current_component.at<complex<double>>(max_loc.y, max_loc.x) =
        dft_log.at<complex<double>>(max_loc.y, max_loc.x);

    //controls
    auto k = cv::waitKey(100);
bool paused = false;
    if (k == 'p' || k == 'P') {
      if (paused == true) {
        cv::waitKey(100);
        paused = false;
      } else if (paused == false) {
        cv::waitKey(0);
        paused = true;
      }
      }else if (k == 'q' || k == 'Q') {
      cv::destroyAllWindows();
      break;
    } 

    // copy current max value to current_dft display, in the middle
    double a = src.rows / 2;
    double b = src.cols / 2;
    current_dft.at<double>(a, b) = dft_log.at<double>(a, b);
    

    //calculate inverse dft of the current component
    if (idx == Max || idx == 0) {
      cv::dft(current_component, current_sinusoid, cv::DFT_REAL_OUTPUT + cv::DFT_INVERSE);
      // Add the resulting values to the summed matrix
      summed += current_sinusoid;
      
      //show windows
      cv::imshow("Current Components (log|DFT|)", current_dft);
      cv::imshow("Summed Components", static_cast<cv::Mat_<uint8_t>>(summed));
      // Repurposing cc_sinusoid for both display conditions
      cv::imshow("Current Component (+ 128)",
                 static_cast<cv::Mat_<uint8_t>>(current_sinusoid + 128));
      // Normalizing from 0 to 1 and then scaling up by 255
      cv::normalize(current_sinusoid, current_sinusoid, 0, 1, cv::NORM_MINMAX);
      cv::imshow("Current Component (Scaled)",
                 static_cast<cv::Mat_<uint8_t>>(current_sinusoid * 255));
      max.release();
      // Resetting the current component
      current_component = 0;
    }
    // set the max value to 0 to go onto the next max value in the next loop
    dft_mag.at<double>(max_loc.y, max_loc.x) = 0;
    // update loop idx
    idx = Max;
  }
}
