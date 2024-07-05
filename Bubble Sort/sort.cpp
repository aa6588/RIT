/* Bubble Sorter
 * Author: Andrea Avendano (aa6588@rit.edu)
 * Date: March 3, 2020
 */

#include "Sorting.h"
#include <iostream>
#include <vector>

using namespace std;

int main() {

  vector<double> test = {11, 4, 1, 8, 3,
                         3,  7, 0, 9, 5}; // input vector of any datatype
  cout << "Original vector ... " << endl;
  print_vector(test); // print original vector
  cout << endl;
  cout << "Sorted vector (ascending) ..." << endl;
  Bubble(test, true); // when ascending order is true
  print_vector(test); // print sorted vector in ascending order
  cout << endl;
  cout << "Sorted vector (descending) ..." << endl;
  Bubble(test, false); // when ascending order is false
  print_vector(test);  // print sorted vector in descending order
}
