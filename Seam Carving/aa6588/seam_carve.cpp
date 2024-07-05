#include <ctime>
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "imgs/ipcv/utils/Utils.h"
#include <opencv2/imgproc.hpp>
#include "imgs/ipcv/seam_carving/Seam_carving.h"

using namespace std;

namespace po = boost::program_options;

int main(int argc, char* argv[]) {
  bool verbose = false;
  bool seam_direction = true;
  string src_filename = "";
  string dst_filename = "";

  po::options_description options("Options");
  options.add_options()("help,h", "display this message")(
      "verbose,v", po::bool_switch(&verbose), "verbose [default is silent]")(
      "source-filename,i", po::value<string>(&src_filename), "source filename")(
      "dst-filename,t",po::value<string>(&dst_filename),"resized filename")(
      "seam-direction,d",po::bool_switch(&seam_direction),"default is horizontal");

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

  cv::Mat src = cv::imread(src_filename, cv::IMREAD_COLOR);

  if (verbose) {
    cout << "Source filename: " << src_filename << endl;
    cout << "Size: " << src.size() << endl;
    cout << "Channels: " << src.channels() << endl;
   // cout << "Destination filename: " << dst_filename << endl;
  }

  cv::Mat dst;
  cv::Mat seam;
  dst = src.clone();
  seam.create(src.size(),CV_64F);
cv::imshow(src_filename, src);
  clock_t startTime = clock();
 // ipcv::Seam(src, dst, seam,seam_direction);
//for (int i = 0; i < 15; i ++){
  ipcv::Seam(src, dst, seam,seam_direction);
//dst.copyTo(src);
//}

  clock_t endTime = clock();

  if (verbose) {
    cout << "Elapsed time: "
         << (endTime - startTime) / static_cast<double>(CLOCKS_PER_SEC)
         << " [s]" << endl;
  }

//if (dst_filename.empty()) {
   // cv::imshow(src_filename, src);
  //  cv::imshow("[Rezied]", dst);
  //  cv::imshow("energy", seam);
  //  cv::waitKey(0);
  //} else {
  //  cv::imwrite(dst_filename, dst);
 // }
return EXIT_SUCCESS;
}
