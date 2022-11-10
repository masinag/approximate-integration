#include "function.hpp"
#include "integration.hpp"
#include <argparse/argparse.hpp>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

void read_pt(const std::string& ptfile, HPOLYTOPE& pt) {
    std::ifstream inp;
    std::vector<std::vector<NT>> Pin;
    inp.open(ptfile, std::ifstream::in);
    if (!inp.good())
        throw std::invalid_argument("Polytope file does not exist");
    read_pointset(inp, Pin);
    pt = HPOLYTOPE(Pin);
}

void read_fn(const std::string& fnfile, FUNCTION& fn) {
    std::ifstream inp;
    inp.open(fnfile, std::ifstream::in);
    if (!inp.good())
        throw std::invalid_argument("Integrand file does not exist");
    read_function(inp, fn);
}

void write_res(NT res) { std::cout << "Decimal: " << res << std::endl; }

void parse_args(int argc, char* argv[], HPOLYTOPE& pt, FUNCTION& fn, NT& error,
                volumetype& vtype, walktype& wtype,
                unsigned int& wlength, unsigned int& seed, unsigned int& N) {
    argparse::ArgumentParser parser("./volesti_integrate");
    parser.add_description(
        "Approximate integration of functions over polytopes");

    // polytope file
    parser.add_argument("polytope_file")
        .help("The file containing the polytope to integrate over in .hrep "
              "format");
    // polynomial file
    parser.add_argument("integrand_file")
        .help("The file containing the function to integrate in exprtk "
              "format");
    // relative error
    parser.add_argument("error")
        .help("The relative error tolerated")
        .scan<'g', NT>();
    // volume algorithm
    parser.add_argument("--volume")
        .help("The method used to compute the volume. The available methods "
              "are:\n"
              "\t\t\t\tCB      Cooling Balls\n"
              "\t\t\t\tCG      Cooling Gaussians\n"
              "\t\t\t\tSOB     Sequence Of Balls")
        .default_value(std::string("CB"))
        .action([](const std::string& value) {
            if (volumes.find(value) != volumes.end())
                return value;
            throw std::runtime_error("Invalid volume method");
        });
    // random walk type
    parser.add_argument("--walk")
        .help("The type of random walk to sample points. The available types "
              "are:\n"
              "\t\t\t\tBa      Ball Walk\n"
              "\t\t\t\tRDHR    Random Direction Hit and Run\n"
              "\t\t\t\tCDHR    Coordinate Direction Hit and Run\n"
              "\t\t\t\tBi      Billiard Walk\n"
              "\t\t\t\tABi     Accelerated Billiard Walk")
        .default_value(std::string("CDHR"))
        .action([](const std::string& value) {
            if (walks.find(value) != walks.end())
                return value;
            throw std::runtime_error("Invalid walk type");
        });

    // random walk length
    std::unordered_map<std::string, double> default_wlength_exp = {
        {"RDHR", 3}, {"CDHR", 3}, {"Ba", 2.5}, {"Bi", 2}, {"ABi", 2},
    };
    unsigned int min_wlength = 10;
    std::ostringstream ss;
    ss << "The length of the random walk to sample random points. Default "
          "value is set to: \n";
    for (const auto& entry : default_wlength_exp) {
        ss << "\t\t\t\t" << std::left << std::setw(8) << entry.first << "max("
           << min_wlength << ", d^" << entry.second << ")" << std::endl;
    }
    ss << "where d is the number of dimensions of the polytope";
    parser.add_argument("--wlength").help(ss.str()).scan<'u', unsigned int>();

    // random seed
    parser.add_argument("--seed")
        .help("The seed used to initialize the random number generator")
        .scan<'u', unsigned int>();

    // number of points for MC integral estimation
    parser.add_argument("--N")
        .help("The number of points used to estimate the integral")
        .default_value(1000U)
        .scan<'u', unsigned int>();

    // parse arguments
    parser.parse_args(argc, argv);

    std::string ptfile = parser.get<std::string>("polytope_file");
    std::string fnfile = parser.get<std::string>("integrand_file");

    read_pt(ptfile, pt);
    read_fn(fnfile, fn);

    error = parser.get<NT>("error");
    if (error <= 0.0L || error >= 1)
        throw std::runtime_error("Error must be a a real number in (0, 1)");

    vtype = volumes.at(parser.get<std::string>("--volume"));
    std::string wtype_str = parser.get<std::string>("--walk");
    wtype = walks.at(wtype_str);
    N = parser.get<unsigned int>("--N");

    if (auto opt_wlength = parser.present<unsigned int>("--wlength")) {
        wlength = *opt_wlength;
    } else {
        wlength =
            std::max(min_wlength,
                     static_cast<unsigned int>(std::pow(
                         pt.dimension(), default_wlength_exp.at(wtype_str))));
    }

    if (auto opt_seed = parser.present<unsigned int>("--seed")) {
        seed = *opt_seed;
    } else {
        seed = std::chrono::system_clock::now().time_since_epoch().count();
    }
}

int main(int argc, char* argv[]) {
    // called as ./integrate_poly polytope_file polynomial_file error
    // [volumetype walktype N walk_length]
    HPOLYTOPE pt;
    FUNCTION fn;
    volumetype vtype;
    walktype wtype;
    double error;
    unsigned int wlength, seed, N;
    try {
        parse_args(argc, argv, pt, fn, error, vtype, wtype, wlength, seed, N);
    } catch (const std::runtime_error& ex) {
        std::cerr << ex.what() << std::endl;
        std::cerr << "Try '--help' for more information." << std::endl;
        exit(1);
    } catch (const std::invalid_argument& ex) {
        std::cerr << ex.what() << std::endl;
        std::cerr << "Try '--help' for more information." << std::endl;
        exit(1);
    }
#ifndef NDEBUG
    std::cerr << "error " << error << std::endl
              << "volumetype " << vtype << std::endl
              << "walktype " << wtype << std::endl
              << "N " << N << std::endl
              << "wlength " << wlength << std::endl;

    std::cerr << "Integrating " << fn << std::endl;
    // DEBUG("Over polytope" << std::endl);
    // pt.print();
#endif

    double res =
        integrate_function(fn, pt, error, vtype, wtype, wlength, seed, N);

#ifndef NDEBUG
    std::cerr << "RES: " << res << std::endl;
#endif

    write_res(res);

    return 0;
}
