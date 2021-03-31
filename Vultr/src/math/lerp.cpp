#include <math/lerp.h>
#include <tgmath.h>
namespace Vultr
{
namespace Math
{
double Lerp(double a, double b, double f)
{
    return a + f * (b - a);
}
double Lerp(double a, double b, double f, double dt)
{
    return Lerp(a, b, 1 - pow(f, dt));
}
} // namespace Math
} // namespace Vultr
