#pragma once 
#include <cmath>
#include <string>

// Base Function class with virtual methods
struct Function
{
public:
    virtual ~Function() = default; // Add virtual destructor for proper polymorphism
    virtual std::string name() const = 0; // Pure virtual method for name
    virtual double function(double x) const = 0;
    virtual double derivative(double x) const = 0;
    virtual double error_term(double x, double error) const = 0;
};

// ReLu activation function
struct ReLu : public Function
{
public:
    std::string name() const override { return "ReLu"; }

    double function(double x) const override {
        return (x > 0) ? x : 0;
    }

    double derivative(double x) const override {
        return (x > 0) ? 1 : 0;
    }

    double error_term(double x, double error) const override {
        return derivative(x) * error;
    }
};

// Sigmoid activation function
struct Sigmoid : public Function
{
public:
    std::string name() const override { return "Sigmoid"; }

    double function(double x) const override {
        return 1.0 / (1.0 + exp(-x));
    }

    double derivative(double x) const override {
        double s = function(x);
        return s * (1 - s);
    }

    double error_term(double x, double error) const override {
        return derivative(x) * error;
    }
};

// Tanh activation function
struct Tanh : public Function
{
public:
    std::string name() const override { return "Tanh"; }

    double function(double x) const override {
        return tanh(x);
    }

    double derivative(double x) const override {
        double t = tanh(x);
        return 1 - (t * t);
    }

    double error_term(double x, double error) const override {
        return derivative(x) * error;
    }
};

// SoftMax activation function
struct SoftMax : public Function
{
public:
    std::string name() const override { return "SoftMax"; }

    double function(double x) const override {
        return (x > 0) ? x : 0; // Dummy implementation (SoftMax requires vector context)
    }

    double derivative(double x) const override {
        return (x > 0) ? 1 : 0; // Dummy implementation
    }

    double error_term(double x, double error) const override {
        return derivative(x) * error; // Dummy implementation
    }
};
