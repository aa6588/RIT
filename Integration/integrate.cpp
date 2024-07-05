/* Integration
 * Author: Andrea Avendano (aa6588@rit.edu)
 * Date: March 16, 2020
 */

#include <iostream>
#include <vector>
#include <cmath>
#include "imgs/numerical/integration/Integral.h"

using namespace std;

double F(double x) {return cos(x);}

int main() {
vector<double> x;
vector<double> y;
double a;
double b;
double n;

cout << "For the defined function ..." << endl;
cout << "a: "; 
cin >> a;
cout << "b: ";
cin >> b;
cout << "n: ";
cin >> n;

x.reserve(n); //reserves number of points
for (double i = 0; i < n; i++) { //create independent x vector
   x.push_back(((i) * M_PI)/(2*(n-1)));
}
for (double i = 0; i < n; i++) { //create dependent y vector
   y.push_back(F(x[i]));
}
cout << "Area = " << Integral(x,y) << " (using vector interface)" << endl;
cout << "Area = " << Integral(F, a, b, n) << " (using function/limits/number of points interface)" << endl;
cout << "Area = " << Integral([](double x) { return cos(x); }, a , b, n ) << " (using lambda function/limits/number of points interface)" << endl;
}







