/* Projectile Header File
 */

namespace physics {

class Projectile {
public:

/**constructor for 
 * initial velocity, launch angle, and initial height
 */
  Projectile(double v0, double angle, double y0) { 
  v0_ = v0;
  angle_ = angle;
  y0_ = y0;
}
  ~Projectile(){};

/** Initial velocity getter
 */
  double get_v0() const { return v0_; } 
/** Initial velocity setter
 */
  void set_v0(double v0);
/** Launch angle getter
 */
  double get_angle() const { return angle_; }
/**Launch angle setter
 */
  void set_angle(double angle);
/** initial height getter
 */
  double get_y0() const { return y0_; }
/**initial height setter
 */
  void set_y0(double y0);

/**compute the initial horizontal velocity
 */
  double vx();
/**compute the initial vertical velocity
 */
  double vy(double t);

/**compute the final velocity after t seconds
 * \param[in] time 
 */
  double velocity(double t);
/**compute the final horizontal displacement after t seconds
 * \param[in] time
 */
  double horiz_disp(double t);
/**compute the final vertical displacement after t seconds 
 * \param[in] time
 */
  double vert_disp(double t);
/**compute the magnitude of the displacement after t seconds
 * \param[in] time
 */
  double disp(double t);
/**compute the total time in air 
 */
  double time_flight();
/**compute the maximum height reached above the surface
 */
  double max_height();
/**compute the total horizontal distance traveled
 */
  double total_horiz_dist();

private:
  double v0_;
  double angle_;
  double y0_;
};
} // namespace physics
