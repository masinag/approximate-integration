#ifndef INTEGRATION_H
#define INTEGRATION_H
#include "function.hpp"
#include "simple_MC_integration.hpp"

// Polytope type
typedef HPolytope<Point> HPOLYTOPE;
// Function type
typedef Function<Point, NT> FUNCTION;

// Type of random walk to use to sample points from the polytope
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

/**
 * @brief Integrate a function over a polytope using Monte Carlo integration
 *
 * @tparam NT the floating point representation to use
 * @param fn the function to integrate
 * @param pt the polytope to integrate over
 * @param error the error tolerance for the volume estimate
 * @param vtype the algorithm to use to compute the volume of the polytope
 * @param wtype the type of random walk to use to sample points from the
 * polytope
 * @param N the number of points to sample to estimate the integral
 * @param wlength the length of the random walk to use to sample points from the
 * polytope
 * @return the estimated value of the integral
 */
template <typename NT>
NT integrate_function(const FUNCTION& fn, HPOLYTOPE& pt, const NT error,
                      const volumetype vtype, const walktype wtype, const int N,
                      const int wlength) {
    NT res;
    switch (wtype) {
    case Ba: // BallWalk
        res = simple_mc_polytope_integrate<BallWalk, HPOLYTOPE, BallWalk>(
            fn, pt, N, vtype, wlength, error);
        break;
    case CDHR: // CDHRWalk
        res = simple_mc_polytope_integrate<CDHRWalk, HPOLYTOPE, CDHRWalk>(
            fn, pt, N, vtype, wlength, error);
        break;
    case RDHR: // RDHRWalk
        res = simple_mc_polytope_integrate<RDHRWalk, HPOLYTOPE, RDHRWalk>(
            fn, pt, N, vtype, wlength, error);
        break;
    case Bi: // BilliardWalk
        res =
            simple_mc_polytope_integrate<BilliardWalk, HPOLYTOPE, BilliardWalk>(
                fn, pt, N, vtype, wlength, error);
        break;
    case ABi: // AcceleratedBilliwardWalk
        res = simple_mc_polytope_integrate<AcceleratedBilliardWalk, HPOLYTOPE,
                                           AcceleratedBilliardWalk>(
            fn, pt, N, vtype, wlength, error);
        break;
    default:
        std::cerr << "Invalid Walk Type" << std::endl;
        std::exit(1);
    }
    return res;
}
#endif
