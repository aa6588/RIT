/* BlackbodyFit Header File
 * Author: Andrea Avendano (aa6588@rit.edu)
 * Date: April 03, 2020
 */

#include <vector>

namespace radiometry {

using namespace std;

/* Returns the derived material temperature when fitting a blackbody in
 * a wavelength interval to  an acceptable error.
 * and passes back the derived emissivity spectrum through the 
 * parameter list.
 */
 
double BlackbodyFit(vector<double> &x, vector<double> &y, double lower,
                    double upper, double t_error, vector<double> &emissivity);

} // namespace radiometry
