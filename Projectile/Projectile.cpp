//Projectile implementation file
#include "Projectile.h"
#include <cmath>

namespace physics {

const double g = 9.80665;
double t;

void Projectile::set_v0(double v0) { v0_ = v0; }

void Projectile::set_angle(double angle) { angle_ = angle; }

void Projectile::set_y0(double y0) { y0_ = y0; }

double Projectile::vx() { return v0_ * cos(angle_); }
double Projectile::vy(double t) { return (v0_ * sin(angle_) - (g * t)); }

double Projectile::velocity(double t) {
  return sqrt((this->vx() * this->vx()) + (this->vy(t) * this->vy(t)));
}
double Projectile::horiz_disp(double t) { return (v0_ * cos(angle_) * t); }
double Projectile::vert_disp(double t) {
  return ((v0_ * sin(angle_) * t) - (0.5 * g * t * t) + y0_);
}
double Projectile::disp(double t) {
  return sqrt(this->horiz_disp(t) * this->horiz_disp(t) +
              this->vert_disp(t) * this->vert_disp(t));
}
double Projectile::time_flight() {
  return ((v0_ * sin(angle_) +
           sqrt(v0_ * v0_ * sin(angle_) * sin(angle_) + (2 * g * y0_))) /
          g);
}
double Projectile::max_height() {
  return (((v0_ * v0_ * sin(angle_) * sin(angle_)) / (2 * g)) + y0_);
}
double Projectile::total_horiz_dist() { return (v0_ * cos(angle_) * this->time_flight()); }

} // namespace physics
