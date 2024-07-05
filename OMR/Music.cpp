/** Implementation file for Optical Music Recognition
 *  \author: Andrea Avendano (aa6588@rit.edu)
 *  \date 9 April 2021
 */

#include "Music.h"

#include <iostream>
#include <math.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <filesystem>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <string>
using namespace std;

namespace ipcv {

/** Finds note types and pitch on a sheet of music
 *
 *  \param[in] src          source cv::Mat of CV_8UC1 (sheet music)
 *  \param[out] note        std::vector of note type
 *  \param[out] pitch       std::vector of pitch 
 */

void OMR(const cv::Mat& src, cv::Mat binary){

cv::Mat sheet;
sheet = src.clone();
cv::Mat bw;
//apply otsus thresholding and inverting
cv::threshold(sheet,bw,0,255,cv::THRESH_BINARY_INV+cv::THRESH_OTSU);

// Create the images that will use to extract the horizonta and vertical lines
cv::Mat horizontal;
    // Specify size on horizontal axis
    int horizontalsize = bw.cols / 30;

    // Create structure element for extracting horizontal lines through morphology operations
   cv::Mat horizontalStructure = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(horizontalsize,1));

    // Apply morphology operations
    cv::erode(bw, horizontal, horizontalStructure, cv::Point(-1, -1));
    cv::dilate(horizontal, horizontal, horizontalStructure, cv::Point(-1, -1));
//cv::imshow("horizontal",horizontal);
//cv::waitKey(0);
   horizontal.convertTo(horizontal, CV_32FC1);

//find row sum
int sum = 0;

cv::Mat h = cv::Mat_<float>::zeros(horizontal.rows,1);
for (int j = 0; j < horizontal.rows; j++) {
for (int i = 0; i < horizontal.cols; i++) {
      sum += horizontal.at<float>(j, i);
} 
h.at<float>(j,0) = sum;
sum = 0;
}

//find peaks in sum vector
std::vector<int> peaks;
for (int i=1; i<h.total()-1; ++i)
{
       float left  = h.at<float>(i-1);
       float cent  = h.at<float>(i);
       float right = h.at<float>(i+1);
       // we have to set a boundary condition for 'plateaus', 
       // so just decide to have the 'cutoff' on the left side
       if ( left < cent  && right <= cent )
       {
         peaks.push_back(i);      // peak !
       }
}

cout << "Staff: " << endl;
for(int i=0; i < peaks.size()-1; i++){
   if (peaks.at(i+1) - peaks.at(i) < 15 && peaks.at(i+1) - peaks.at(i) > 5){
      //they are in the same group of staves
      //v.push_back(peaks.at(i));}
      cout << peaks[i] << endl;}
  else if (peaks.at(i+1) - peaks.at(i) <= 5){
     i++;}
  else   {

    cout << peaks[i]  << " ";
    cout << endl;
    cout << "Staff: " << endl;
     // v.push_back(peaks.at(i));
     // vecs.push_back(v);
     // v.clear();
}
}

//Note recognition (template matching)
cv::cvtColor(sheet,sheet,cv::COLOR_GRAY2BGR);
// Read in note files
  std::string path = "/cis/otherstu/aa6588/src/cpp/rit/data/images/misc/notes/half";
  std::string extension = ".png";

  const auto &directory_entries = std::filesystem::directory_iterator(path);

  std::vector<std::string> files;
  for (const auto &directory_entry : directory_entries) {
    if (directory_entry.path().extension() == extension) {
      files.push_back(directory_entry.path());
    }
  }
  std::sort(files.begin(), files.end());

    cv::Mat result;
  cv::Point maxLoc;
  cv::bitwise_not(bw,bw);
  // for all filter files
for(int i = 0; i < 10; i++){
  for (const auto &file : files) {

    // read in filter and invert it
    cv::Mat filter = cv::imread(file, cv::IMREAD_GRAYSCALE);
    
    //match tempalte
    cv::matchTemplate(bw,filter,result, cv::TM_CCOEFF_NORMED);
    cv::minMaxLoc(result,NULL,NULL,NULL,&maxLoc);
   
    cout << file << ": " << maxLoc << endl;

cv::ellipse(bw, maxLoc,cv::Size(5,40),0, 0,360, cv::Scalar( 255, 255, 255 ), 15, cv::FILLED );
  
}

i++;
}

bw.copyTo(binary);

}
}
