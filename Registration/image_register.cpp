#include <ctime>
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "imgs/ipcv/utils/Utils.h"
#include <opencv2/imgproc.hpp>
#include "imgs/ipcv/image_registration/registration.h"

using namespace std;

namespace po = boost::program_options;

int main(int argc, char* argv[]) {
  bool verbose = false;
  string src_filename = "";
  string trans_filename = "";
 // string dst_filename = "";

  po::options_description options("Options");
  options.add_options()("help,h", "display this message")(
      "verbose,v", po::bool_switch(&verbose), "verbose [default is silent]")(
      "source-filename,i", po::value<string>(&src_filename), "source filename")(
      "trans-filename,t",po::value<string>(&trans_filename),"translated filename");

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

  cv::Mat src = cv::imread(src_filename, cv::IMREAD_GRAYSCALE);
  cv::Mat trans = cv::imread(trans_filename, cv::IMREAD_GRAYSCALE);

  if (verbose) {
    cout << "Source filename: " << src_filename << endl;
    cout << "Size: " << src.size() << endl;
    cout << "Channels: " << src.channels() << endl;
   // cout << "Destination filename: " << dst_filename << endl;
  }

  cv::Mat src_log;
  src_log.create(src.size(), CV_8UC1);
  cv::Mat trans_log;
  trans_log.create(trans.size(), CV_8UC1);
  cv::Mat dst;
  dst.create(src.size(),CV_8UC1);
  clock_t startTime = clock();

  ipcv::Register(src, trans,src_log, trans_log,dst);

  clock_t endTime = clock();

  if (verbose) {
    cout << "Elapsed time: "
         << (endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC)
         << " [s]" << endl;
  }

  cv::Mat newImage = cv::imread("/cis/otherstu/aa6588/src/cpp/rit/data/images/misc/letter/letter1.png", cv::IMREAD_GRAYSCALE);

cv::Mat R;
R = cv::Mat(src.size(), CV_8UC1,cv::Scalar(0));
dst = cv::Mat(src.size(), CV_8UC3, cv::Scalar(0));
    std::vector<cv::Mat> channels;
    channels.push_back(newImage);
    channels.push_back(trans);
    channels.push_back(R);
    cv::merge(channels, dst);

    cv::imshow("Source", src);
    cv::imshow("Translated", trans);
   // cv::imshow("Source [Polar Coordinates]",
     //        ipcv::DftMagnitude(src_log,
       //                         ipcv::DFT_MAGNITUDE_LOG +
         //                       ipcv::DFT_MAGNITUDE_CENTER +
           //                     ipcv::DFT_MAGNITUDE_NORMALIZE));
    cv::imshow("Source [Polar Coordinates]", src_log);
    cv::imshow("Translated [Polar Coordinates]", trans_log);
    cv::imshow("new image",dst);
    cv::imwrite("/cis/otherstu/aa6588/src/cpp/rit/data/images/misc/letter/letter1.tif",dst);
    cv::waitKey(0);
 
cv::Mat sobel = trans.clone();
std::vector<double> rmse;
cv::Sobel(trans,sobel,CV_8UC1,1,0,3);
cv::cvtColor(trans, trans, cv::COLOR_GRAY2RGB);
cv::cvtColor(newImage, newImage, cv::COLOR_GRAY2RGB);

ipcv::Rmse(trans,newImage,sobel,rmse);
cout << "RMSE: " << endl;
    for (auto i = rmse.begin(); i != rmse.end(); ++i){
        cout << *i << " " << endl; }

  return EXIT_SUCCESS;
}

