/** Implementation file for Seam Carving: content aware image resizing
 *  \author: Andrea Avendano (aa6588@rit.edu)
 *  \date 30 April 2021
 */

#include "Seam_carving.h"

#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include "imgs/ipcv/utils/Utils.h"
#include "imgs/ipcv/seam_carving/Seam_carving.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

namespace ipcv {


/** Finds and removes seams of image 
 *  \param[in] src          source cv::Mat of CV_8UC1
 *  \param[out] resized     new resized image
 *  \param[out] seam        cv::Mat with seam locations
 */

void Seam(cv::Mat src, cv::Mat dst, cv::Mat energy_image, bool seam_direction){
cv::Mat image;
cv::Mat image_gray;
dst = src.clone();

//i = number of column/rows taken out
for (int i = 0; i < 500; i++){

cvtColor(dst,image_gray,COLOR_BGR2GRAY);
cv::Mat gradient;

//calculate gradient (energy matrix)
    Mat dx;
    Mat dy;
    Sobel(image_gray, dx, CV_64F, 1, 0);
    Sobel(image_gray, dy, CV_64F, 0, 1);
    // add gradients
    addWeighted( dx, 0.5, dy, 0.5, 0, gradient );
    // convert and normalize
    gradient.convertTo(energy_image, CV_64F, 1.0/255.0);

double a,b,c;
    
    // get the numbers of rows and columns in the image
    int rowsize = energy_image.rows;
    int colsize = energy_image.cols;
    
    // initialize the map with zeros
    Mat energy_map = Mat_<double>::zeros(rowsize,colsize);
    
    // copy the first row or column depending on direction
    if (seam_direction == true) energy_image.row(0).copyTo(energy_map.row(0));
    else if (seam_direction == false) energy_image.col(0).copyTo(energy_map.col(0));
    
    // take the minimum of the three neighbors and add to total energy
    if (seam_direction == true) //vertical
{
        for (int row = 1; row < rowsize; row++) {
            for (int col = 0; col < colsize; col++) {
                a = energy_map.at<double>(row - 1, max(col - 1, 0));
                b = energy_map.at<double>(row - 1, col);
                c = energy_map.at<double>(row - 1, min(col + 1, colsize - 1));
                
                energy_map.at<double>(row, col) = energy_image.at<double>(row, col) + min(a, min(b, c));
            }
        }
    }
    else if (seam_direction == false) //horiztonal
{
        for (int col = 1; col < colsize; col++) {
            for (int row = 0; row < rowsize; row++) {
                a = energy_map.at<double>(max(row - 1, 0), col - 1);
                b = energy_map.at<double>(row, col - 1);
                c = energy_map.at<double>(min(row + 1, rowsize - 1), col - 1);
                
                energy_map.at<double>(row, col) = energy_image.at<double>(row, col) + min(a, min(b, c));
            }
        }
    }
    
    
//find optimal seams
double d,e,f;
    int offset = 0;
    vector<int> seam;
    //double min_val, max_val;
    Point min_loc, max_loc;
    
    if (seam_direction == true) {
        // copy the data from the last row of the energy map
        Mat row = energy_map.row(rowsize - 1);
    
        // get min and max values and locations
        minMaxLoc(row, NULL, NULL, &min_loc, &max_loc);
        
        // initialize the seam vector
        seam.resize(rowsize);
        int min_index = min_loc.x;
        int current_min = 0;
        seam[rowsize - 1] = min_index;
   //bottom to top
        for (int i = rowsize - 2; i >= 0; i--) {
            d = energy_map.at<double>(i, min_index - 1);
            e = energy_map.at<double>(i, min_index);
            f = energy_map.at<double>(i, min_index + 1);
            current_min = min(d,min(e,f));
            if (current_min == f) {
                offset = 1;
            }
            else if (current_min == e) {
                offset = 0;
            }
            else if (current_min == d) {
                offset = -1;
            }
            
            min_index += offset;
            min_index = min(max(min_index, 0), colsize - 1); // take care of edge cases
            seam[i] = min_index;
        }
    }
    
    else if (seam_direction == false) {
        // copy the data from the last column of the cumulative energy map
        Mat col = energy_map.col(colsize - 1);
        
        // get min and max values and locations
        minMaxLoc(col, NULL,NULL, &min_loc, &max_loc);
        
        // initialize the path vector
        seam.resize(colsize);
        int min_index = min_loc.y;
        seam[colsize - 1] = min_index;
       int current_min =0; 
        // starting from the right, look at the three adjacent pixels to the left of current pixel, choose the minimum of those and add to the path
        for (int i = colsize - 2; i >= 0; i--) {
            d = energy_map.at<double>(min_index - 1, i);
            e = energy_map.at<double>(min_index, i);
            f = energy_map.at<double>(min_index + 1, i);
            //right
            if (current_min == f) {
                offset = 1;
            }
            //middle
            else if (current_min == e) {
                offset = 0;
            }
            //left
            else if (current_min == d) {
                offset = -1;
            }
            
            min_index += offset;
           // min_index = min(max(min_index, 0), rowsize - 1); // take care of edge cases
            seam[i] = min_index;
        }
    }

Mat dummy(1, 1, CV_8UC3, Vec3b(0, 0, 0));
    if (seam_direction == true) { // reduce the width
        for (int i = 0; i < rowsize; i++) {
            // take all pixels to the left and right of marked pixel and store them 
            Mat new_row;
            Mat lower = dst.rowRange(i, i + 1).colRange(0, seam[i]);
            Mat upper = dst.rowRange(i, i + 1).colRange(seam[i] + 1, colsize);
            
            if (!lower.empty() && !upper.empty()) {
                hconcat(lower, upper, new_row);
                hconcat(new_row, dummy, new_row);
            }
            else {
                if (lower.empty()) {
                    hconcat(upper, dummy, new_row);
                }
                else if (upper.empty()) {
                    hconcat(lower, dummy, new_row);
                }
            }
            new_row.copyTo(dst.row(i));
        }
        // clip the last col of image
        dst = dst.colRange(0, colsize - 1);
    }
    else if (seam_direction == false) { // reduce the height
        for (int i = 0; i < colsize; i++) {
            // take all pixels to the top and bottom of marked pixel and store
            Mat new_col;
            Mat lower = dst.colRange(i, i + 1).rowRange(0, seam[i]);
            Mat upper = dst.colRange(i, i + 1).rowRange(seam[i] + 1, rowsize);
            
            if (!lower.empty() && !upper.empty()) {
                vconcat(lower, upper, new_col);
                vconcat(new_col, dummy, new_col);
            }
            else {
                if (lower.empty()) {
                    vconcat(upper, dummy, new_col);
                }
                else if (upper.empty()) {
                    vconcat(lower, dummy, new_col);
                }
            }
            // take the newly formed column and place it into the original image
            new_col.copyTo(dst.col(i));
        }
        // clip the bottom row
        dst = dst.rowRange(0, rowsize - 1);
    }

 if (seam_direction == true) {
        for (int i = 0; i < energy_image.rows; i++) {
            energy_image.at<double>(i,seam[i]) = 1;
        }
    }
    else if (seam_direction == false) {
        for (int i = 0; i < energy_image.cols; i++) {
            energy_image.at<double>(seam[i],i) = 1;
        }
    }

        imshow("Reduced Image", dst);
    imshow ("energy", energy_image);
cv::waitKey(0);
}
}
}
