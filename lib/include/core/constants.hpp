#if !defined(CONSTANTS_8_31)
#define CONSTANTS_8_31

namespace core
{
    namespace constants
    {
        constexpr double EPS0 = 8.85418782e-12;
        constexpr double Kb = 1.3806503e-23;
        constexpr double Qe = 1.60217646e-19;
        constexpr double h = 6.626070e-34;
        constexpr double AMU = 1.66053886e-27;
        constexpr double Me = 9.10938e-31;
        constexpr double FLT_EPS = 1.0e-7;
        constexpr double PI = 3.14;
        constexpr double KtoEV = Kb / Qe;
        constexpr double EVtoK = 1.0 / KtoEV;
        constexpr double EVtoJ = Qe;
        constexpr double JtoEV = 1.0 / EVtoJ;
        constexpr double SQRT2 = 1.41;
        constexpr double A0 = 5.29e-11;
    }
}

#endif