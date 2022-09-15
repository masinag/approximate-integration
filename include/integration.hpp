#ifndef INTEGRATION_H
#define INTEGRATION_H
#include "function.hpp"
#include "simple_MC_integration.hpp"
typedef HPolytope<Point> HPOLYTOPE;
typedef Function<Point, NT> FUNCTION;
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


template <typename NT>
NT integrate_function(const FUNCTION& pn, HPOLYTOPE& pt, const NT error,
                      const volumetype vtype, const walktype wtype, const int N,
                      const int wlength) {
    NT res;
    switch (wtype) {
    case Ba: // BallWalk
        res = simple_mc_polytope_integrate<BallWalk, HPOLYTOPE, BallWalk>(
            pn, pt, N, vtype, wlength, error);
        break;
    case CDHR: // CDHRWalk
        res = simple_mc_polytope_integrate<CDHRWalk, HPOLYTOPE, CDHRWalk>(
            pn, pt, N, vtype, wlength, error);
        break;
    case RDHR: // RDHRWalk
        res = simple_mc_polytope_integrate<RDHRWalk, HPOLYTOPE, RDHRWalk>(
            pn, pt, N, vtype, wlength, error);
        break;
    case Bi: // BilliardWalk
        res =
            simple_mc_polytope_integrate<BilliardWalk, HPOLYTOPE, BilliardWalk>(
                pn, pt, N, vtype, wlength, error);
        break;
    case ABi: // AcceleratedBilliwardWalk
        res = simple_mc_polytope_integrate<AcceleratedBilliardWalk, HPOLYTOPE,
                                           AcceleratedBilliardWalk>(
            pn, pt, N, vtype, wlength, error);
        break;
    default:
        std::cerr << "Invalid Walk Type" << std::endl;
        exit(1);
    }
    return res;
}
#endif
