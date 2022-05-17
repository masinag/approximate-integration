#include "integration.hpp"
#include <argparse/argparse.hpp>
#include <exception>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

typedef double Rational;

void read_pt(const std::string& ptfile, HPOLYTOPE& pt) {
    std::ifstream inp;
    std::vector<std::vector<Rational>> Pin;
    inp.open(ptfile, std::ifstream::in);
    if (!inp.good())
        throw std::invalid_argument("Polytope file does not exist");
    read_pointset(inp, Pin);
    pt = HPOLYTOPE(Pin);
}

void read_pn(const std::string& pnfile, POLYNOMIAL& pn) {
    std::ifstream inp;
    inp.open(pnfile, std::ifstream::in);
    if (!inp.good())
        throw std::invalid_argument("Polynomial file does not exist");
    read_polynomial(inp, pn);
}

void write_res(Rational res) { std::cout << "Decimal: " << res << std::endl; }

void parse_args(int argc, char* argv[], HPOLYTOPE& pt, POLYNOMIAL& pn,
                Rational& error, volumetype& vtype, walktype& wtype,
                unsigned int& N, unsigned int& wlength) {
    argparse::ArgumentParser parser("./volesti_integrate_polynomial");
    parser.add_description(
        "Approximate integration of polynomial over polytopes");
    parser.add_argument("polytope_file")
        .help("The file containing the polytope to integrate over in .hrep "
              "format");
    parser.add_argument("polynomial_file")
        .help("The file containing the polynomial to integrate in LattE "
              "format [[coefficient,[exponent vector]],...]");
    parser.add_argument("error")
        .help("The relative error tolerated")
        .scan<'g', Rational>();
    parser.add_argument("--volume")
        .help("The method used to compute the volume. The available methods "
              "are Cooling Balls (CB), Cooling Gaussians (CG) and Sequence Of "
              "Balls (SOB)")
        .default_value(std::string("CB"))
        .action([](const std::string& value) {
            if (volumes.find(value) != volumes.end())
                return value;
            throw std::runtime_error("Invalid volume method");
        });
    parser.add_argument("--walk")
        .help("The type of random walk to sample points. The available types "
              "are Ball Walk (Ba), (CDHR), (RDHR), Billiard Walk (Bi), "
              "Accelerated Billiard Walk (ABi)")
        .default_value(std::string("CDHR"))
        .action([](const std::string& value) {
            if (walks.find(value) != walks.end())
                return value;
            throw std::runtime_error("Invalid walk type");
        });
    parser.add_argument("--N")
        .help("The number of points used to estimate the integral")
        .default_value(1000U)
        .scan<'u', unsigned int>();
    parser.add_argument("--wlength")
        .help("The length of the random walk to sample random points. If 0, a "
              "default value is set to d^2, where d is the number of "
              "dimensions of the polytope")
        .default_value(0U)
        .scan<'u', unsigned int>();
    parser.parse_args(argc, argv);

    std::string ptfile = parser.get<std::string>("polytope_file");
    std::string pnfile = parser.get<std::string>("polynomial_file");

    read_pt(ptfile, pt);
    read_pn(pnfile, pn);

    error = parser.get<Rational>("error");
    if (error <= 0.0L || error >= 1)
        throw std::runtime_error("Error must be a a real number in (0, 1)");

    vtype = volumes.at(parser.get<std::string>("--volume"));
    wtype = walks.at(parser.get<std::string>("--walk"));
    N = parser.get<unsigned int>("--N");
    wlength = parser.get<unsigned int>("--wlength");
    if (wlength == 0U)
        wlength = pt.dimension() * pt.dimension();
}

int main(int argc, char* argv[]) {
    // called as ./integrate_poly polytope_file polynomial_file error
    // [volumetype walktype N walk_length]
    HPOLYTOPE pt;
    POLYNOMIAL pn;
    volumetype vtype;
    walktype wtype;
    double error;
    unsigned int N, wlength;
    try {
        parse_args(argc, argv, pt, pn, error, vtype, wtype, N, wlength);
    } catch (const std::runtime_error& ex) {
        std::cerr << ex.what() << std::endl;
        std::cerr << "Try '--help' for more information." << std::endl;
        exit(1);
    } catch (const std::invalid_argument& ex) {
        std::cerr << ex.what() << std::endl;
        std::cerr << "Try '--help' for more information." << std::endl;
        exit(1);
    }
    DEBUG("Polytope file " << ptfile << std::endl "Polynomial file " << pnfile
                           << std::endl "error " << error
                           << std::endl "volumetype " << vtype
                           << std::endl "walktype " << wtype << std::endl "N "
                           << N << std::endl "wlength " << wlength
                           << std::endl);

    DEBUG("Integrating " << pn << std::endl);
    // DEBUG("Over polytope" << std::endl);
    // pt.print();

    double res = integrate_polynomial(pn, pt, error, vtype, wtype, N, wlength);
    DEBUG("RES: " << res << std::endl);

    write_res(res);

    return 0;
}
