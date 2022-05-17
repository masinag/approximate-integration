#include "integration.hpp"
#include "polynomial.hpp"
#include <cstring>
#include <exception>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#define UNDEFINED -1

typedef double Rational;

void help() {
    std::cout
        << "Usage: ./integrate_poly polytope_file polynomial_file error "
           "[volumetype walktype N walk_length]"
        << std::endl
        << std::endl
        << "Required arguments" << std::endl
        << "polytope_file:\t\tSpecifies the polytope to integrate over in .ine "
           "format (see cddlib manual)"
        << std::endl
        << "polynomial_file:\tSpecifies the polynomial to integrate in LattE "
           "format [[coefficient,[exponent vector]],...]"
        << std::endl
        << "error:\t\t\tRelative error acceptable (rational number in (0, 1))"
        << std::endl
        << std::endl
        << "Optional arguments" << std::endl
        << "volume_type:\t\tVolume computation method" << std::endl
        << "\t\t\t\tSOB: Sequence of Balls" << std::endl
        << "\t\t\t\tCB: Cooling Balls (default)" << std::endl
        << "\t\t\t\tCG: Cooling Gaussians" << std::endl
        << "walk_type:\t\tType of random walk" << std::endl
        << "\t\t\tBa: BallWalk" << std::endl
        << "\t\t\tCDHR: CDHRWalk (default)" << std::endl
        << "\t\t\tRDHR: RDHRWalk" << std::endl
        << "\t\t\tBi: BilliardWalk" << std::endl
        << "\t\t\tABi: AcceleratedBilliwardWalk"
        << std::endl
        // << "\t\t\tGBa: GaussianBallWalk" << std::endl
        // << "\t\t\tGCDHR: GaussianCDHRWalk" << std::endl
        // << "\t\t\tGRDHR: GaussianRDHRWalk" << std::endl
        // << "\t\t\tGABi: GaussianAcceleratedBilliwardWalk" << std::endl
        << "N:\t\t\tNumber of samples (default 1000)" << std::endl
        << "walk_length:\t\tLength of random walk (default d^2 where d is the "
           "number of dimensions of the polytope)"
        << std::endl;
}

void read_pt(std::string& ptfile, HPOLYTOPE& pt) {
    std::ifstream inp;
    std::vector<std::vector<Rational>> Pin;
    inp.open(ptfile, std::ifstream::in);
    if (!inp.good())
        throw std::invalid_argument("Polytope file does not exist");
    read_pointset(inp, Pin);
    pt = HPOLYTOPE(Pin);
}

void read_pn(std::string& pnfile, POLYNOMIAL& pn) {
    std::ifstream inp;
    inp.open(pnfile, std::ifstream::in);
    if (!inp.good())
        throw std::invalid_argument("Polynomial file does not exist");
    read_polynomial(inp, pn);
}

void write_res(Rational res) { std::cout << "Decimal: " << res << std::endl; }

void parse_args(int argc, char* argv[], HPOLYTOPE& pt, POLYNOMIAL& pn,
                Rational& error, volumetype& vtype, walktype& wtype, int& N,
                int& wlength) {
    if (argc < 4)
        throw std::invalid_argument("Bad number of parameters");

    std::string ptfile(argv[1]);
    std::string pnfile(argv[2]);
    read_pt(ptfile, pt);
    read_pn(pnfile, pn);

    error = std::stod(argv[3]);
    if (error <= 0.0L || error >= 1)
        throw std::invalid_argument("Error must be a a real number in (0, 1)");

    // optional parameters
    vtype = CB;
    wtype = CDHR;
    N = 1000;
    wlength = pt.dimension() * pt.dimension();
    if (argc > 4) {
        std::string volume(argv[4]);
        if (volume == "CB")
            vtype = CB;
        else if (volume == "CG")
            vtype = CG;
        else if (volume == "SOB")
            vtype = SOB;
        else
            throw std::invalid_argument("Invalid volume method");
    }

    if (argc > 5) {
        std::string walk(argv[5]);
        if (walk == "Ba")
            wtype = Ba;
        else if (walk == "CDHR")
            wtype = CDHR;
        else if (walk == "RDHR")
            wtype = RDHR;
        else if (walk == "Bi")
            wtype = Bi;
        else if (walk == "ABi")
            wtype = ABi;
        // else if (walk == "GBa")
        //     wtype = GBa;
        // else if (walk == "GCDHR")
        //     wtype = GCDHR;
        // else if (walk == "GRDHR")
        //     wtype = GRDHR;
        // else if (walk == "GABi")
        //     wtype = GABi;
        else
            throw std::invalid_argument("Invalid walk type");
    }

    if (argc > 6) {
        N = std::stol(argv[6]);
        if (N <= 0)
            throw std::invalid_argument("N must be a positive integer");
    }

    if (argc > 7) {
        wlength = std::stol(argv[7]);
        if (wlength <= 0)
            throw std::invalid_argument(
                "walk_length must be a positive integer");
    }
}

int main(int argc, char* argv[]) {
    // called as ./integrate_poly polytope_file polynomial_file error
    // [volumetype walktype N walk_length]
    HPOLYTOPE pt;
    POLYNOMIAL pn;
    volumetype vtype;
    walktype wtype;
    double error;
    int N, wlength;
    try {
        parse_args(argc, argv, pt, pn, error, vtype, wtype, N, wlength);
    } catch (const std::invalid_argument& ex) {
        std::cerr << ex.what() << std::endl;
        help();
        exit(1);
    }
    // std::cerr << "Polytope file " << ptfile << std::endl
    //           << "Polynomial file " << pnfile << std::endl
    //           << "error " << error << std::endl
    //           << "volumetype " << vtype << std::endl
    //           << "walktype " << wtype << std::endl
    //           << "N " << N << std::endl
    //           << "wlength " << wlength << std::endl;

    DEBUG("Integrating " << pn << std::endl);
    // std::cerr << "Over polytope" << std::endl;
    // pt.print();

    double res = integrate_polynomial(pn, pt, error, vtype, wtype, N, wlength);
    DEBUG("RES: " << res << std::endl);

    write_res(res);

    return 0;
}
