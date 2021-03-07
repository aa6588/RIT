/* Constant Hue Interpolation Method App
 * Author: andrea avendano (aa6588@rit.edu)
 * Date: Feb 26, 2021
 */
#include <ctime>
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "imgs/ipcv/demosaicing/Demosaic.h"
#include "imgs/ipcv/utils/Utils.h"


using namespace std;

namespace po = boost::program_options;

int main(int argc, char* argv[]) {
  bool verbose = false;
  string src_filename = "";
  string dst_filename = "";
  string pattern = "GBRG";

  po::options_description options("Options");
  options.add_options()("help,h", "display this message")(
      "verbose,v", po::bool_switch(&verbose), "verbose [default is silent]")(
      "source-filename,i", po::value<string>(&src_filename), "source filename")(
      "destination-filename,o", po::value<string>(&dst_filename),
      "destination filename")("pattern,p", po::value<string>(&pattern),
                              "pattern [default is GBRG]");

  po::positional_options_description positional_options;
  positional_options.add("source-filename", -1);

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv)
                .options(options)
                .positional(positional_options)
                .run(),
            vm);
  po::notify(vm);

  if (vm.count("help")) {
    cout << "Usage: " << argv[0] << " [options] source-filename" << endl;
    cout << options << endl;
    return EXIT_SUCCESS;
  }

  if (!boost::filesystem::exists(src_filename)) {
    cerr << "Provided source file does not exists" << endl;
    return EXIT_FAILURE;
  }

  cv::Mat src = cv::imread(src_filename, cv::IMREAD_UNCHANGED);
  cv::Mat truth = cv::imread("/cis/otherstu/aa6588/src/cpp/rit/data/images/raw/simulated/truth.ppm", cv::IMREAD_UNCHANGED);
cv::Mat bilinear = cv::imread("/cis/otherstu/aa6588/src/cpp/rit/data/images/raw/simulated/bayer_bilinear.ppm",cv::IMREAD_UNCHANGED);
cv::Mat laroche = cv::imread("/cis/otherstu/aa6588/src/cpp/rit/data/images/raw/simulated/bayer_laroche_and_prescott.ppm",cv::IMREAD_UNCHANGED);

std::vector<double> rmse;
std::vector<double> rmse1;
std::vector<double> rmse2;
std::vector<double> psnr;
std::vector<double> psnr1;
std::vector<double> psnr2;
  if (verbose) {
    cout << "Source filename: " << src_filename << endl;
    cout << "Size: " << src.size() << endl;
    cout << "Channels: " << src.channels() << endl;
    cout << "Pattern: " << pattern << endl;
    cout << "Destination filename: " << dst_filename << endl;
  }
  cv::Mat sobel = src.clone();
  clock_t startTime = clock();

  cv::Mat dst = ipcv::ConstantHue(src, pattern);
  cv::Sobel(src,sobel,CV_8UC1,1,0,3);
  clock_t endTime = clock();

  if (verbose) {
    cout << "Elapsed time: "
         << (endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC)
         << " [s]" << endl;
  }

  if (dst_filename.empty()) {
    cv::imshow(src_filename, src);
    cv::imshow(src_filename + " [CFA Interpolated]", dst);
    cv::waitKey(0);
  } else {
    cv::imwrite(dst_filename, dst);
  }

cv::Mat dE;
cv::Mat dE1;
cv::Mat dE2;

  truth.convertTo(truth, CV_8UC3);
  dst.convertTo(dst, CV_8UC3);
  ipcv::Rmse(truth,dst,sobel,rmse);
  ipcv::Psnr(truth,dst,255,sobel,psnr);
  ipcv::DeltaE(truth,dst,255,sobel,dE);

cout << "RMSE (Hue): " << endl; 
    for (auto i = rmse.begin(); i != rmse.end(); ++i){ 
        cout << *i << " " << endl; }
cout << "PSNR (Hue): " << endl;
    for (auto i = psnr.begin(); i != psnr.end(); ++i){
        cout << *i << " " << endl; }
dE.convertTo(dE,CV_8UC3);
cv::imwrite("/cis/otherstu/aa6588/src/cpp/rit/data/images/raw/simulated/deltaE.png",dE);

cout << endl;
  bilinear.convertTo(bilinear, CV_8UC3);
  ipcv::Rmse(truth,bilinear,sobel,rmse1);
  ipcv::Psnr(truth,bilinear,255,sobel,psnr1);
  ipcv::DeltaE(truth,bilinear,255,sobel,dE1);

cout << "RMSE (bilinear): " << endl;
    for (auto i = rmse1.begin(); i != rmse1.end(); ++i){
        cout << *i << " " << endl;}
cout << "PSNR (bilinear): " << endl;
    for (auto i = psnr1.begin(); i != psnr1.end(); ++i){
        cout << *i << " " << endl;}
dE1.convertTo(dE1,CV_8UC3);
cv::imwrite("/cis/otherstu/aa6588/src/cpp/rit/data/images/raw/simulated/deltaE_bilinear.png",dE1);

cout<<endl;
  laroche.convertTo(laroche, CV_8UC3);
  ipcv::Rmse(truth,laroche,sobel,rmse2);  
  ipcv::Psnr(truth,laroche,255,sobel,psnr2);
  ipcv::DeltaE(truth,laroche,255,sobel,dE2);

cout << "RMSE (laroche): " << endl;
    for (auto i = rmse2.begin(); i != rmse2.end(); ++i){
        cout << *i << " " << endl;}
cout << "PSNR (laroche): " << endl;
    for (auto i = psnr2.begin(); i != psnr2.end(); ++i){
        cout << *i << " " << endl;}
dE2.convertTo(dE2,CV_8UC3);
cv::imwrite("/cis/otherstu/aa6588/src/cpp/rit/data/images/raw/simulated/deltaE_laroche.png",dE2);




  return EXIT_SUCCESS;
}
