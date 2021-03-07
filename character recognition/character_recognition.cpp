/** Application file for character recognition
 *  \author Andrea Avendano (aa6588@rit.edu)
 *  \date 20 Oct 2020
 */

#include "imgs/ipcv/utils/Utils.h"
#include "imgs/plot/plot.h"
#include <algorithm>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <ctime>
#include <filesystem>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <string>

using namespace std;
namespace po = boost::program_options;

int main(int argc, char *argv[]) {
  bool verbose = false;
  string src_filename = "";
  string filter_string = "matching";

  po::options_description options("Options");
  options.add_options()("help,h", "display this message")(
      "verbose,v", po::bool_switch(&verbose), "verbose [default is silent]")(
      "source-filename,i", po::value<string>(&src_filename),
      "source filename")("filter-type,k", po::value<string>(&filter_string),
                         "filter type (matching|angle) [default is matching]");

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
  // read in source image
  cv::Mat src = cv::imread(src_filename, cv::IMREAD_GRAYSCALE);

  if (verbose) {
    cout << "Source filename: " << src_filename << endl;
    cout << "Size: " << src.size() << endl;
    cout << "Channels: " << src.channels() << endl;
    cout << "Filter: " << filter_string << endl;
  }

  // Read in character files
  std::string path = "/cis/faculty/cnspci/public_html/courses/common/images/"
                     "character_recognition/notAntiAliased/characters";
  std::string extension = ".tif";

  const auto &directory_entries = std::filesystem::directory_iterator(path);

  std::vector<std::string> files;
  for (const auto &directory_entry : directory_entries) {
    if (directory_entry.path().extension() == extension) {
      files.push_back(directory_entry.path());
    }
  }
  std::sort(files.begin(), files.end());

  // make a vector of vector hits
  int char_total = static_cast<int>(files.size());
  std::vector<std::vector<int>> hits(2, std::vector<int>(char_total, 0));
  int char_idx = 0;

  // convert src image to floats and invert it
  src.convertTo(src, CV_32FC1, 1.f / 255);
  // for all filter files
  for (const auto &file : files) {
    // Convert file name to character
    hits[0][char_idx] = std::stoi(file.substr(file.length() - 6, 2), nullptr);
    // read in filter and invert it
    cv::Mat filter = cv::imread(file, cv::IMREAD_GRAYSCALE);
    filter.convertTo(filter, CV_32FC1, 1.f / 255);

    // sum of filter
    double filter_sum = cv::sum(filter)[0];
    // Magnitude using dot product
    double filter_mag = 0;
    filter_mag = sqrt(filter.dot(filter));

    cv::Mat neighborhood;

    double src_mag = 0;
    // for the current neighborhood
    for (int rows = 0; rows <= src.rows; rows++) {
      for (int cols = 0; cols <= src.cols; cols++) {
        if (cols + filter.cols < src.cols && rows + filter.rows < src.rows) {
          cv::Mat neighborhood =
              src(cv::Rect(cols, rows, filter.cols, filter.rows));
          double neighborhood_sum = cv::sum(neighborhood)[0];

          if (filter_string == "angle") {

            src_mag = sqrt(neighborhood.dot(neighborhood));
            // cosine between vectors
            double cos = neighborhood.dot(filter) / (src_mag * filter_mag);
            // if cos(angle) is greater than matching threshold
            if (cos > 0.999999) {
              // Adds to histogram count
              hits[1][char_idx] += 1;
            }
          }
          if (filter_string == "matching") {

            // Normalize filter
            filter = filter / filter_sum;

            float sum = 0;

            // Normalize image in current neighborhood
            if (neighborhood_sum != 0) {
              neighborhood = neighborhood / neighborhood_sum;
              // for each row and col in filter
              for (int filter_row = 0; filter_row < filter.rows; filter_row++) {
                for (int filter_col = 0; filter_col < filter.cols;
                     filter_col++) {

                  // Find response
                  sum += filter.at<float>(filter_row, filter_col) *
                         neighborhood.at<float>(filter_row, filter_col);
                }
              }
              // if max response is higher than matching threshold
              if (sum >= 0.97) {
                // Adds to histogram count
                hits[1][char_idx] += 1;
              }
            }
          }
        }
      }
    }
    char_idx++;
  }
  //show counts for each character
  for (int i = 0; i < char_total; i++) {
    char character = hits[0][i];
    int total = hits[1][i];
    std::cout << character << ": " << total << std::endl;
  }
  // Plot histogram
  plot::plot2d::Params params;
  params.set_x_label("Character");
  params.set_y_label("count");
  plot::Plot2d(hits[0], hits[1], params);

  return EXIT_SUCCESS;
}
