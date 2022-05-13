#ifndef POLYNOMIAL_HPP
#define POLYNOMIAL_HPP

#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#ifndef NDEBUG
#define DEBUG(x) std::cerr << x
#else
#define DEBUG(x)
#endif

template <typename Point, typename NT> class Monomial {
  private:
    NT coefficient;
    std::vector<std::pair<int, int>> var_exp;

  public:
    Monomial(NT coefficient, std::vector<std::pair<int, int>>& var_exp)
        : coefficient(coefficient), var_exp(var_exp) {}

    double operator()(Point X) {
        double total = coefficient;
        for (auto& v : var_exp) {
            total *= std::pow(X[v.first], v.second);
        }
        return total;
    }

    friend std::ostream& operator<<(std::ostream& out,
                                    const Monomial& monomial) {
        out << monomial.coefficient;
        for (auto& v : monomial.var_exp) {
            out << " * x_{" << v.first << "}^{" << v.second << "}";
        }
        return out;
    }

    /**
     * Check if monomial is a constant; if so, store in c its constant value.
     */
    bool is_constant(double& c) {
        c = coefficient;
        return var_exp.empty();
    }
};

template <typename Point, typename NT> class Polynomial {
  private:
    std::vector<Monomial<Point, NT>> monomials;

  public:
    Polynomial& operator+=(Monomial<Point, NT>& monomial) {
        monomials.push_back(monomial);
        return *this;
    }

    double operator()(Point X) {
        NT total = 0.0L;
        for (auto& m : monomials) {
            total += m(X);
        }
        return total;
    }

    friend std::ostream& operator<<(std::ostream& out,
                                    const Polynomial<Point, NT>& poly) {
        out << "(" << poly.monomials[0] << ")";
        for (size_t i = 1; i < poly.monomials.size(); i++) {
            out << " + (" << poly.monomials[i] << ")";
        }
        return out;
    }

    /**
     * Check if polynomial is a constant; if so, store in c its constant value.
     */
    bool is_constant(NT& c) {
        c = 0.0L;
        NT mc;
        for (auto& monomial : monomials) {
            if (!monomial.is_constant(mc))
                return false;
            c += mc;
        }
        return true;
    }
};

/**
 * Parse a rational from the given string.
 *
 * The rational can be expressed in floating point or scientific notation
 * or as a fraction.
 */
template <typename NT>
inline NT parse_rational(const char* str, char** end_ptr) {
    NT num, den = 1.0L;
    num = std::strtod(str, end_ptr);
    for (str = *end_ptr; *str == ' '; ++str)
        ;
    if (*str == '/')
        den = std::strtod(str + 1, end_ptr);
    return num / den;
}

/**
 * Input format:
 * [[coefficient,[exponent vector]],...]
 */
template <typename Point, typename NT>
inline void read_polynomial(std::istream& is, Polynomial<Point, NT>& p) {
    std::string line;
    std::getline(is, line);
    DEBUG("Parsing " << line << std::endl);
    const char* line_c = line.c_str();
    size_t size = line.size();

    std::vector<std::pair<int, int>> var_exp;
    NT coefficient;
    int var_index;
    char* end_ptr; // store the end pointer of a parsed number
    // ignore outermost square brackets
    for (size_t i = 1; i < size - 1; i++) {
        // find start of next monomial
        while (line_c[i] != '[')
            i++;
        i++;
        // read coefficient
        coefficient = parse_rational<NT>(line_c + i, &end_ptr);
        assert(coefficient != 0);
        i = end_ptr - line_c;
        // skip spaces
        while (line_c[i] == ' ' || line_c[i] == ',')
            i++;
        // read exponents
        for (i++, var_index = 0; line_c[i] != ']'; i++, var_index++) {
            int exponent = std::strtol(line_c + i, &end_ptr, 10);
            if (exponent > 0)
                var_exp.push_back(std::make_pair(var_index, exponent));
            i = end_ptr - line_c;
        }
        // add monomial
        Monomial<Point, NT> m(coefficient, var_exp);
        DEBUG("Found monomial: " << m << std::endl);
        p += m;
        var_exp.clear();
    }
}

#endif
