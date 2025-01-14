#pragma once
#include <cmath>
#include <string>

struct Function
{
public:
    const std::string name = "func";
    double function(double x) { return 0; };
    double derivative(double x) { return 0; };
    double error_term(double x, double error) { return 0; };
};

struct ReLu : public Function
{
public:
    const std::string name = "ReLu";

    double function(double x) {
        return (x > 0) ? x : 0;
    }

    double derivative(double x) {
        return (x > 0) ? 1 : 0;
    }

    double error_term(double x, double error) {
        return derivative(x) * error;
    }
};

struct Sigmoid : public Function
{
public:
    const std::string name = "Sigmoid";

    double function(double x) {
        return 1.0 / (1.0 + exp(-x));
    }

    double derivative(double x) {
        double s = function(x);
        return s * (1 - s);
    }

    double error_term(double x, double error) {
        return derivative(x) * error;
    }
};

struct Tanh : public Function
{
public:
    const std::string name = "Tanh";

    double function(double x) {
        return tanh(x);
    }

    double derivative(double x) {
        double t = tanh(x);
        return 1 - (t * t);
    }

    double error_term(double x, double error) {
        return derivative(x) * error;
    }
};