/* Bubble Sort Header File
 */

#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

/* Funtion to print out vector of any datatype
 */
template <typename T> void print_vector(vector<T> &vector) {
  for (auto element : vector) {
    cout << element << endl;
  }
}

/*Funtion to bubble sort vector of any datatype, ascending when boolean
 * parameter is true
 */
template <typename T> void Bubble(vector<T> &vector, bool ascending) {
  for (size_t i = 0; i < vector.size(); i++) { // repeats for n times
    for (size_t j = 0; j < vector.size() - 1;
         j++) {                      // compares for n-p pairs of numbers
      if (ascending) {               // if ascending = true
        if (vector[j] > vector[i]) { // if left number is bigger than the right
          std::swap(vector[j], vector[i]); // swap the bigger number to the
                                           // right
        }
      } else if (vector[j] < vector[i]) { // if ascending = false, and left
                                          // number is smaller than right
        std::swap(vector[j], vector[i]); // swap the bigger number to the left
      }
    }
  }
}
