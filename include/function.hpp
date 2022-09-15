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

template <typename Point, typename NT> class Function {
  private:
    std::vector<NT> vars;
    exprtk::expression<NT> expression;
    std::string expression_string;
    std::vector<std::string> var_names;
    exprtk::symbol_table<NT> symbol_table;

  public:
    Function() {}
    Function(const std::string& expression_string,
             const std::vector<std::string>& var_names)
        : var_names(var_names), expression_string(expression_string) {
        exprtk::parser<NT> parser;
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

    Function(const Function& other) : Function(other.expression_string, other.var_names) {}

    NT operator()(const Point X) {
        for (int i = 0; i < vars.size(); i++) {
            vars[i] = X[i];
        }
        NT value = expression.value();

#ifndef NDEBUG
        std::cerr << "Value of " << *this << " in (";
        for (auto& v : vars) {
            std::cerr << v << " ";
        }
        std::cerr << ") = " << value << std::endl;
#endif
        return value;
    }

    friend std::ostream& operator<<(std::ostream& out, const Function& fn) {
        out << fn.expression_string;
        return out;
    }
};

/**
 * Input format:
 * <variable_0> <variable_1> ... <variable_n>
 * <expression>
 *
 * Example:
 * x0 x1, x2
 * x1 + x0 * x2
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
    getline(is, line);

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
