/* Blackbody Header file
 */
#include <vector>
namespace radiometry {

using namespace std;

class Blackbody {
public:
  /* Constructor for blackbody temperature
   */
  Blackbody(double T) { temperature_ = T; }

  /*Temperature getter
   */
  double get_T() const { return temperature_; }
  /*Temperature setter
   */
  void set_T(double T);

  /*Computes the radiance of a blackbody with certain temperature
   * param[in] wavelength
   */
  double exitance(double y);
  /*Computes the exitance of a blackbody with certain temperature
   * param[in] wavelength
   */
  double radiance(double y);
  /*Computes the peak wavelength of a blackbody with certain temperature
   * param[in] wavelength
   */
  double peakwavelength();

  /*creates vector for exitance values from a vector of wavelengths
   * param[in]  exitance vector , wavelength value
   */
  void vector_exitance(vector<double> &z, double y);

  /* creates vector for radiance values from a vector of wavelengths
   * param[in] radiance vector, wavelength value 
   */
  void vector_radiance(vector<double> &r, double y);

private:
  double temperature_;
};
} // namespace radiometry
