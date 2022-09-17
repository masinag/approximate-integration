#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include "exprtk.hpp"
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

/**
 * @brief Class representing a mathematical function
 * @tparam Point The input type of the function
 * @tparam NT    The output type of the function
 */
template <typename Point, typename NT> class Function {
  private:
    // value of the input variables
    std::vector<NT> vars;
    // expression tree
    exprtk::expression<NT> expression;
    // string representing the expression
    std::string expression_string;
    // the names of the input variables
    std::vector<std::string> var_names;

  public:
    /** @brief Default constructor. */
    Function() = default;

    /**
     * @brief Construct a new Function object
     *
     * @param expression_string The string representing the function
     * @param var_names         The names of the variables
     */
    Function(const std::string& expression_string,
             const std::vector<std::string>& var_names)
        : expression_string(expression_string), var_names(var_names) {
        exprtk::parser<NT> parser;
        exprtk::symbol_table<NT> symbol_table;
        vars.resize(var_names.size());
        for (int i = 0; i < var_names.size(); i++) {
            symbol_table.add_variable(var_names[i], vars[i], false);
        }
        symbol_table.add_constants();
#ifndef NDEBUG
        std::cerr << "Symbol table:" << std::endl;
        std::vector<std::pair<std::string, NT>> symbols;
        symbol_table.get_variable_list(symbols);
        for (auto& p : symbols) {
            std::cerr << p.first << " = " << p.second << std::endl;
        }
#endif
        expression.register_symbol_table(symbol_table);
        parser.compile(expression_string, expression);
    }

    /** @brief Copy constructor
     *
     * @param other The other function to copy
     */
    Function(const Function& other)
        : Function(other.expression_string, other.var_names) {}

    /**
     * @brief Evaluate the function at a given point
     *
     * @param X the point to evaluate the function at
     * @return the value of the function at the point
     */
    NT operator()(const Point X) {
        for (int i = 0; i < vars.size(); i++) {
            vars[i] = X[i];
        }
#ifndef NDEBUG
        std::cerr << "Value of " << *this << " in (";
        for (auto& v : vars) {
            std::cerr << v << " ";
        }
        std::cerr << ") = " << expression.value() << std::endl;
#endif
        return expression.value();
    }

    friend std::ostream& operator<<(std::ostream& out, const Function& fn) {
        return out << fn.expression_string;
    }
};

/**
 * @brief Read a function from a file
 *
 * Input format:
 * <variable_0> <variable_1> ... <variable_n>
 * <expression>
 *
 * Example:
 * x0 x1 x2
 * x1 + x0 * x2
 *
 *
 * @tparam Point The input type of the function
 * @tparam NT    The output type of the function
 * @param is     The input stream to read from
 * @param fn     The function to read into
 */
template <typename Point, typename NT>
void read_function(std::istream& is, Function<Point, NT>& fn) {
    std::string line, variable_name;
    std::getline(is, line);
    std::istringstream iss(line);
    std::vector<std::string> var_names;
    while (iss >> variable_name) {
        var_names.push_back(variable_name);
    }
    std::getline(is, line);

#ifndef NDEBUG
    std::cerr << "Read variables: ";
    for (std::string& name : var_names) {
        std::cerr << "[" << name << "]";
    }
    std::cerr << std::endl << "Expression: " << line << std::endl;
#endif
    fn = {line, var_names};
}

#endif
