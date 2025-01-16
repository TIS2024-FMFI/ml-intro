#pragma once

struct Function
{
public:
    const std::string name = "func";
    virtual double function(double x);
    virtual double derivative(double x);
    virtual double error_term(double x, double error);
};