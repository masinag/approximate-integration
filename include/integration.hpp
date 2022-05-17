#ifndef INTEGRATION_H
#define INTEGRATION_H
#include "polynomial.hpp"
#include "simple_MC_integration.hpp"
typedef HPolytope<Point> HPOLYTOPE;
typedef Polynomial<Point, NT> POLYNOMIAL;
enum walktype {
    Ba,   // BallWalk
    CDHR, // CDHRWalk
    RDHR, // RDHRWalk
    Bi,   // BilliardWalk
    ABi,  // AcceleratedBilliwardWalk
};

static const std::unordered_map<std::string, volumetype> volumes = {
    {"CB", CB}, {"CG", CG}, {"SOB", SOB}};

static const std::unordered_map<std::string, walktype> walks = {
    {"Ba", Ba}, {"RDHR", RDHR}, {"CDHR", CDHR}, {"Bi", Bi}, {"ABi", ABi}};

template <typename VolumeWalkType = BallWalk,
          typename RNG = RandomNumberGenerator, typename NT>
NT compute_volume(HPOLYTOPE& pt, const NT error, const volumetype vtype,
                  const int wlength) {
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
NT integrate_polynomial(const POLYNOMIAL& pn, HPOLYTOPE& pt, const NT error,
                        const volumetype vtype, const walktype wtype,
                        const int N, const int wlength) {
    NT c;
    if (pn.is_constant(c)) {
        DEBUG("Constant " << c << std::endl);
        return c * compute_volume(pt, error, vtype, wlength);
    }
    NT res;
    switch (wtype) {
    case Ba: // BallWalk
        res = simple_mc_polytope_integrate<BallWalk, HPOLYTOPE>(
            pn, pt, N, vtype, wlength, error);
        break;
    case CDHR: // CDHRWalk
        res = simple_mc_polytope_integrate<CDHRWalk, HPOLYTOPE>(
            pn, pt, N, vtype, wlength, error);
        break;
    case RDHR: // RDHRWalk
        res = simple_mc_polytope_integrate<RDHRWalk, HPOLYTOPE>(
            pn, pt, N, vtype, wlength, error);
        break;
    case Bi: // BilliardWalk
        res = simple_mc_polytope_integrate<BilliardWalk, HPOLYTOPE>(
            pn, pt, N, vtype, wlength, error);
        break;
    case ABi: // AcceleratedBilliwardWalk
        res = simple_mc_polytope_integrate<AcceleratedBilliardWalk, HPOLYTOPE>(
            pn, pt, N, vtype, wlength, error);
        break;
    default:
        std::cerr << "Invalid Walk Type" << std::endl;
        exit(1);
    }
    return res;
}
#endif
