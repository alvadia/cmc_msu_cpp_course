#pragma once
#define _USE_MATH_DEFINES

#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <memory>
#include <any>

class TFunctionComposition;
enum class Type {none, ident , constant , exponentiation , exp , polynomial, addition, subtraction, multiplication , division};

class TFunction {
public:
    TFunction();
    virtual double operator()(double x) const = 0;
    virtual std::string ToString() const = 0;
    virtual double GetDerivative(double x) const = 0;

    friend std::shared_ptr<TFunctionComposition> operator+(const std::shared_ptr<TFunction> &left, 
                                   const std::shared_ptr<TFunction> &right);
    friend std::shared_ptr<TFunctionComposition> operator-(const std::shared_ptr<TFunction> &left, 
                                    const std::shared_ptr<TFunction> &right);
    friend std::shared_ptr<TFunctionComposition> operator*(const std::shared_ptr<TFunction> &left, 
                                   const std::shared_ptr<TFunction> &right);
    friend std::shared_ptr<TFunctionComposition> operator/(const std::shared_ptr<TFunction> &left, 
                                  const std::shared_ptr<TFunction> &right);
};

class TFunctionIdentity: public TFunction {

public:
    TFunctionIdentity(const std::any &parameters);
    virtual double operator()(double x) const override;
    virtual std::string ToString() const override;
    virtual double GetDerivative(double x) const override;
};

class TFunctionConstant: public TFunction {
    double value;

public:
    TFunctionConstant(const std::any &parameters);
    virtual double operator()(double x) const override;
    virtual std::string ToString() const override;
    virtual double GetDerivative(double x) const override;
};

class TFunctionPower: public TFunction {
    double exponent;

public:
    TFunctionPower(const std::any &parameters);
    virtual double operator()(double x) const override;
    virtual std::string ToString() const override;
    virtual double GetDerivative(double x) const override;
};

class TFunctionExponent: public TFunction {
public:
    TFunctionExponent(const std::any &parameters);
    virtual std::string ToString() const override;
    virtual double operator()(double x) const override;
    virtual double GetDerivative(double x) const override;
};

class TFunctionPolynomial: public TFunction {
    std::vector<double> coefficients;

public:
    TFunctionPolynomial(const std::any &parameters);
    virtual double operator()(double x) const override;
    virtual std::string ToString() const override;
    virtual double GetDerivative(double x) const override;
};

class TFunctionComposition: public TFunction {
    std::shared_ptr<TFunction> augend;
    std::shared_ptr<TFunction> addend;
    Type type;

public:
    TFunctionComposition(const std::shared_ptr<TFunction> &augend,
                 const std::shared_ptr<TFunction> &addend,
		const Type type);
    virtual double operator()(double x) const override;
    virtual std::string ToString() const override;
    virtual double GetDerivative(double x) const override;
};

double optimize_newton(const std::shared_ptr<TFunction> &func,
                               double x0,
                               int maxiter, double tol);

