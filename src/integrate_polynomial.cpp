#include "integration.hpp"
#include "polynomial.hpp"
#include <cstring>
#include <exception>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

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
        << "\t\t\t\tCB: Cooling Balls" << std::endl
        << "\t\t\t\tCG: Cooling Gaussians (default)" << std::endl
        << "walk_type:\t\tType of random walk" << std::endl
        << "\t\t\t\tBW: Billiard Walk" << std::endl
        << "\t\t\t\tABW: Accelerated Billiard Walk (default)" << std::endl
        << "N:\t\t\tNumber of samples (default 1000)" << std::endl
        << "walk_length:\t\tLength of random walk (default 10)" << std::endl;
}

void parse_args(int argc, char* argv[], std::string& ptfile,
                std::string& pnfile, Rational& error, volumetype& vtype,
                walktype& wtype, int& N, int& wlength) {
    if (argc < 4) {
        throw std::invalid_argument("Bad number of parameters");
    }
    ptfile = argv[1];
    pnfile = argv[2];
    error = std::stod(argv[3]);
    if (error <= 0.0L || error >= 1) {
        throw std::invalid_argument("Error must be a a real number in (0, 1)");
    }

    // optional parameters
    vtype = CG;
    wtype = ABW;
    N = 1000;
    wlength = 10;
    if (argc > 4) {
        if (std::strcmp(argv[4], "CB") == 0) {
            vtype = CB;
        } else if (std::strcmp(argv[4], "CG") == 0) {
            vtype = CG;
        } else if (std::strcmp(argv[4], "SOB") == 0) {
            vtype = SOB;
        } else {
            throw std::invalid_argument(
                "Volume type must be one of \"CB\", \"CG\", \"SOB\"");
        }
    }

    if (argc > 5) {
        if (std::strcmp(argv[5], "BW") == 0) {
            wtype = BW;
        } else if (std::strcmp(argv[5], "ABW") == 0) {
            wtype = ABW;
        } else {
            throw std::invalid_argument(
                "Walk type must be one of \"BW\", \"ABW\"");
        }
    }

    if (argc > 6) {
        N = std::stol(argv[6]);
        if (N <= 0) {
            throw std::invalid_argument("N must be a positive integer");
        }
    }

    if (argc > 7) {
        wlength = std::stol(argv[7]);
        if (wlength <= 0) {
            throw std::invalid_argument(
                "walk_length must be a positive integer");
        }
    }
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

int main(int argc, char* argv[]) {
    // called as ./integrate_poly polytope_file polynomial_file error
    // [volumetype walktype N walk_length]
    std::string ptfile, pnfile;
    volumetype vtype;
    walktype wtype;
    double error;
    int N, wlength;
    parse_args(argc, argv, ptfile, pnfile, error, vtype, wtype, N,
                    wlength);
    // std::cerr << "Polytope file " << ptfile << std::endl
    //           << "Polynomial file " << pnfile << std::endl
    //           << "error " << error << std::endl
    //           << "volumetype " << vtype << std::endl
    //           << "walktype " << wtype << std::endl
    //           << "N " << N << std::endl
    //           << "wlength " << wlength << std::endl;
    HPOLYTOPE pt;
    POLYNOMIAL pn;
    read_pt(ptfile, pt);
    read_pn(pnfile, pn); 

    DEBUG("Integrating " << pn << std::endl);
    // std::cerr << "Over polytope" << std::endl;
    // pt.print();

    double res = integrate_polynomial(pn, pt, error, vtype, wtype, N, wlength);
    DEBUG("RES: " << res << std::endl);

    write_res(res);

    return 0;
}
