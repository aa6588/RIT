/** Quadratic Formula
 * \author Andrea Avendano (aa6588@rit.edu)
 * \date January 28 2020
 */

#include <iostream>
#include <string>
#include <cmath>
#include <complex>

  int main(){
  double a;
  double b;
  double c;

  std::cout << "a: "; //asks to input variables (ax^2 + bx + c)
  std::cin >> a;
  std::cout << "b: ";
  std::cin >> b;
  std::cout << "c: ";
  std::cin >> c;

double discriminant = (b*b)-(4*a*c);

if (discriminant > 0){ //if discriminant is greater than 0 then it has 2 real roots
  double root1 = (-b+sqrt(discriminant))/(2*a);
  double root2 = (-b-sqrt(discriminant))/(2*a);
  std::cout << "The roots are {" << root1 << ", " << root2 << "}" << std::endl;
}
  else if (discriminant == 0){ //if discriminant is equal to 0 then it has 1 real root
  double root1 = (-b+sqrt(discriminant))/(2*a);
  std::cout << "The root is {" << root1 << "}" << std::endl;
  } 
  else { //if discriminant is less than 0 then it has 2 imaginary roots
  std::complex<double> disc=sqrt(std::complex<double>(b*b-4*a*c));
  std::complex<double> realroot = (-b - disc)/(2*a);
  std::complex<double> iroot = (-b + disc)/(2*a);
  std::cout << "The roots are {" << iroot <<", " << realroot << "}" << std::endl;
  }
}
