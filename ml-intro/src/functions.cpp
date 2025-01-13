#include <cmath>

class Function
{
public:
    double function(double x) {};
    double derivative(double x) {};
    double error_term(double x, double error) {};
};

class ReLu : public Function
{
public:
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

class Sigmoid : public Function
{
public:
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

class Tanh : public Function
{
public:
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