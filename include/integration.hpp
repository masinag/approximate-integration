#include "polynomial.hpp"
#include "simple_MC_integration.hpp"
typedef HPolytope<Point> HPOLYTOPE;
typedef Polynomial<Point, NT> POLYNOMIAL;
enum walktype { BW, ABW };

template <typename VolumeWalkType = BallWalk, typename RNG = RandomNumberGenerator, typename NT>
NT compute_volume(HPOLYTOPE& pt, NT error, volumetype vtype, int wlength) {
    DEBUG("Computing volume instead" << std::endl);
    NT res;
    switch (vtype) {
    case CB:
        res = volume_cooling_balls<VolumeWalkType, RNG, HPOLYTOPE>(pt, error,
                                                                   wlength)
                  .second;
        break;
    case CG:
        res = volume_cooling_gaussians<GaussianBallWalk, RNG, HPOLYTOPE>(
            pt, error, wlength);
        break;
    case SOB:
        res = volume_sequence_of_balls<VolumeWalkType, RNG, HPOLYTOPE>(
            pt, error, wlength);
        break;
    default:
        std::cerr << "Invalid Volume Type" << std::endl;
        exit(1);
    }
    return res;
}

template <typename NT>
NT integrate_polynomial(POLYNOMIAL& pn, HPOLYTOPE& pt, NT error,
                        volumetype vtype, walktype wtype, int N, int wlength) {
    NT c;
    if (pn.is_constant(c)) {
        DEBUG("Constant " << c << std::endl);
        return c * compute_volume(pt, error, vtype, wlength);
    }
    NT res;
    switch (wtype) {
    case BW:
        res = simple_mc_polytope_integrate<BilliardWalk, HPOLYTOPE>(
            pn, pt, N, vtype, wlength, error);
        break;
    case ABW:
        res = simple_mc_polytope_integrate<AcceleratedBilliardWalk, HPOLYTOPE>(
            pn, pt, N, vtype, wlength, error);
        break;
    default:
        std::cerr << "Invalid Walk Type" << std::endl;
        exit(1);
    }
    return res;
}
