#include <math/lerp.h>
#include <tgmath.h>

namespace Vultr::Math
{
    double lerp(double a, double b, double f)
    {
        return a + f * (b - a);
    }
    double lerp(double a, double b, double f, double dt)
    {
        return lerp(a, b, 1 - pow(f, dt));
    }
} // namespace Vultr::Math
