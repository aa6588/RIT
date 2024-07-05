/* Projectile
 * Author: Andrea Avendano (aa6588@rit.edu)
 * Date: February 13, 2020
 */

#include <iostream>
#include "Projectile.h"
#include <cmath>
using namespace std;

int main() {
  double v0;
  double y0;
  double y01;
  double angle;
  double t; 
  
physics::Projectile first(v0,y0,angle);

cout << "Initial velocity [m/s]: " ; //ask initial velocity
cin >> v0;
first.set_v0(v0); 
cout << "Launch angle [deg]: "; //ask launch angle
cin >> angle;
first.set_angle(angle*(M_PI/180)); //change angle to radians
cout << "Initial height [m]: "; //ask initial height
cin >> y0;
first.set_y0(y0);
cout << "Time [s]: "; //ask how much time
cin >> t;

cout << endl;
cout << "For a projectile with the properties: " << endl;
cout << "Initial velocity = " << v0 << " [m/s]" << endl;
cout << "Launch angle = " << angle << " [deg]" << endl;
cout << "Initial height = " << y0 << " [m]" << endl;
cout << endl;
cout << "Velocity at " << t << " [s] is " << first.velocity(t) << " [m/s] (in open space)" << endl;
cout << "Horizontal displacement at " << t << " [s] is " << first.horiz_disp(t) << " [m] (in open space)" << endl;
cout << "Vertical displacement at " << t << " [s] is " << first.vert_disp(t) << " [m] (in open space)" << endl;
cout << "Displacement magnitude at " << t << " [s] is " << first.disp(t) << " [m] (in open space)" << endl;
cout << endl;
cout << "Time of flight is " << first.time_flight() << " with y0 = " << y0 << " [m]" << endl;
cout << "Maximum height is " << first.max_height() << " with y0 = " << y0 << " [m]" << endl;
cout << "Maximum distance is " << first.total_horiz_dist() << " with y0 = " << y0 << " [m]" << endl;
cout << endl;
cout << "Modify height" << endl; //asks to change height 
cout << "Initial height [m]: ";
cin >> y01;
first.set_y0(y01);
cout << "Using the modified projectile properties: " << endl;
cout << "Initial height: = " << first.get_y0() << " [m]" << endl;
cout << endl;
cout << "Time of flight is " << first.time_flight() << " with y0 = " << first.get_y0() << " [m]" << endl;
cout << "Maximum height is " << first.max_height() << " with y0 = " << first.get_y0() << " [m]" << endl;
cout << "Maximum distance is " << first.total_horiz_dist() << " with y0 = " << first.get_y0() << " [m]" << endl;
}
