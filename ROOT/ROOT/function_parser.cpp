#include "function_parser.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <functional>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

FunctionParserBase::FunctionParserBase(std::string function_str) : function_str(std::move(function_str)) {}

inline bool FunctionParserBase::isPolynomial(const std::string& expression) {
    if (expression.empty()) {
        return false;
    }
    return icontains(expression, "x") && !icontains(expression, "sin") && !icontains(expression, "cos");
}

inline bool FunctionParserBase::isTrigonometric(const std::string& expression) {
    if (expression.empty()) {
        return false;
    }
    return icontains(expression, "sin") || icontains(expression, "cos");
}

bool FunctionParserBase::icontains(const std::string& hay, const std::string& needle) {
    std::string hay_lower = hay;
    std::string needle_lower = needle;
    std::transform(hay_lower.begin(), hay_lower.end(), hay_lower.begin(), ::tolower);  // NOLINT(modernize-use-ranges)
    std::transform(needle_lower.begin(), needle_lower.end(), needle_lower.begin(),
                   ::tolower);  // NOLINT(modernize-use-ranges)
    return hay_lower.find(needle_lower) != std::string::npos;
}
std::string FunctionParserBase::removeSpaces(const std::string& function_str) {
    std::string out;
    out.reserve(function_str.size());
    for (char function_char : function_str) {
        if (std::isspace(static_cast<unsigned char>(function_char)) == 0) {
            out.push_back(function_char);
        }
    }
    return out;
}

std::vector<std::string> FunctionParserBase::splitSignTokens(const std::string& expr_no_ws) {
    std::vector<std::string> tokens;
    if (expr_no_ws.empty()) {
        return tokens;
    }

    size_t start = 0;
    for (size_t i = 1; i < expr_no_ws.size(); ++i) {
        char sign = expr_no_ws[i];
        if (sign == '+' || sign == '-') {
            tokens.push_back(expr_no_ws.substr(start, i - start));
            start = i;
        }
    }
    tokens.push_back(expr_no_ws.substr(start));
    return tokens;
}

std::pair<double, std::string> FunctionParserBase::parseOptionalCoefficient(const std::string& token) {
    std::regex coeff_regex(R"(^([+-]?[0-9]*\.?[0-9]+)\*?(.*)$)");
    std::smatch match;

    if (std::regex_match(token, match, coeff_regex)) {
        double coeff = std::stod(match[1]);
        std::string rest = match[2];
        return {coeff, rest};
    }
    return {1.0, token};
}

PolynomialParser ::PolynomialParser(std::string function_str) : FunctionParserBase(function_str) {}

bool PolynomialParser::parseTokenAsPolyTerm(const std::string& raw_token, std::function<double(double)>& out_term) {
    if (raw_token.empty()) {
        return false;
    }

    int sign = 1;
    std::string token = raw_token;

    if (token[0] == '+') {
        sign = 1;
        token = token.substr(1);
    } else if (token[0] == '-') {
        sign = -1;
        token = token.substr(1);
    }

    if (token.empty()) {
        return false;
    }

    std::smatch match;

    // Match 3*x^2, 3x^2, x^2, x, 3x, 3*x
    std::regex poly_regex(R"(^([0-9]*\.?[0-9]+)?\*?x(?:\^([0-9]+))?$)");
    if (std::regex_match(token, match, poly_regex)) {
        double coeff = match[1].matched ? std::stod(match[1]) : 1.0;
        int power = match[2].matched ? std::stoi(match[2]) : 1;

        out_term = [coeff = sign * coeff, power = power](double var) { return coeff * std::pow(var, power); };
        return true;
    }

    // Numeric constant
    std::regex num_regex(R"(^([0-9]*\.?[0-9]+)$)");
    if (std::regex_match(token, match, num_regex)) {
        double constant = std::stod(token);
        out_term = [out = sign * constant](double) { return out; };
        return true;
    }

    // Try coefficient-first: 2*x^2, 4*x
    auto [coeff_guess, rest] = parseOptionalCoefficient(token);
    std::regex power_only(R"(^x(?:\^([0-9]+))?$)");
    if (std::regex_match(rest, match, power_only)) {
        int power = match[1].matched ? std::stoi(match[1]) : 1;
        out_term = [coeff = sign * coeff_guess, power = power](double var) { return coeff * std::pow(var, power); };
        return true;
    }

    return false;
}

std::function<double(double)> PolynomialParser::parse() {
    std::string function_str_no_spaces = removeSpaces(this->function_str);

    auto tokens = splitSignTokens(function_str_no_spaces);
    std::vector<std::function<double(double)>> terms;

    for (const auto& token : tokens) {
        std::function<double(double)> term;
        if (!parseTokenAsPolyTerm(token, term)) {
            std::cerr << "Unsupported polynomial token: '" << token << "'\n";
            return [](double) { return 0.0; };
        }
        terms.push_back(term);
    }

    return [terms](double var) {
        double sum = 0;
        for (const auto& term : terms) {
            sum += term(var);
        }
        return sum;
    };
}

TrigonometricParser ::TrigonometricParser(std::string function_str) : FunctionParserBase(function_str) {}

bool TrigonometricParser::parseTokenAsTrigTerm(const std::string& raw_token, std::function<double(double)>& out_term) {
    if (raw_token.empty()) {
        return false;
    }

    int sign = 1;
    std::string token = raw_token;

    if (token[0] == '+') {
        sign = 1;
        token = token.substr(1);
    } else if (token[0] == '-') {
        sign = -1;
        token = token.substr(1);
    }

    if (token.empty()) {
        return false;
    }

    std::smatch match;

    // Match: 3*sin(x), sin(x), 2cos(x), cos(x), etc.
    std::regex trig_regex(R"(^([0-9]*\.?[0-9]+)?\*?(sin|cos)\(x\)$)");
    if (std::regex_match(token, match, trig_regex)) {
        double coeff = match[1].matched ? std::stod(match[1]) : 1.0;
        std::string type = match[2];

        if (type == "sin") {
            out_term = [coeff = sign * coeff](double var) { return coeff * std::sin(var); };
        } else {
            out_term = [coeff = sign * coeff](double var) { return coeff * std::cos(var); };
        }

        return true;
    }

    // Try coefficient-first: 2sin(x)
    auto [coeff_guess, rest] = parseOptionalCoefficient(token);
    std::regex core(R"(^sin\(x\)$|^cos\(x\)$)");
    if (std::regex_match(rest, core)) {
        if (rest.find("sin") != std::string::npos) {
            out_term = [coeff = sign * coeff_guess](double var) { return coeff * std::sin(var); };
        } else {
            out_term = [coeff = sign * coeff_guess](double var) { return coeff * std::cos(var); };
        }
        return true;
    }

    return false;
}

std::function<double(double)> TrigonometricParser::parse() {
    std::string function_str_no_spaces = removeSpaces(function_str);

    auto tokens = splitSignTokens(function_str_no_spaces);
    std::vector<std::function<double(double)>> terms;

    for (const auto& token : tokens) {
        std::function<double(double)> term;
        if (!parseTokenAsTrigTerm(token, term)) {
            std::cerr << "Unsupported trig token: '" << token << "'\n";
            return [](double) { return 0.0; };
        }
        terms.push_back(term);
    }

    return [terms](double var) {
        double sum = 0;
        for (const auto& term : terms) {
            sum += term(var);
        }
        return sum;
    };
}

std::function<double(double)> FunctionParserBase::parseFunction(const std::string& function_str) {
    if (isPolynomial(function_str)) {
        PolynomialParser parser(function_str);
        return parser.parse();
    }
    if (isTrigonometric(function_str)) {
        TrigonometricParser parser(function_str);
        return parser.parse();
    }

    std::cerr << "Unsupported function type: '" << function_str << "'\n";
    return [](double) { return 0.0; };
}
