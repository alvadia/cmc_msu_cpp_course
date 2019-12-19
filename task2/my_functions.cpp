#pragma once
#include "my_functions.h"

#define _USE_MATH_DEFINES // for C++

TFunction::TFunction(){}

template<typename T>
TFunctionIdentity::TFunctionIdentity(const T &parameters) {
    T x;
    check_param(x);
}

inline double TFunctionIdentity::operator()(double x) const {return x;}

inline std::string TFunctionIdentity::ToString() const {return "x";}

inline double TFunctionIdentity::GetDerivative(double x) const {return 1;}

template<typename T>
inline double check_param(T x) {
    try {
       return (double) x;
    } catch (std::logic_error) {
       throw std::logic_error("Convert Exception Parameter");
    }
}

template<typename T>
TFunctionConstant::TFunctionConstant(const T &parameters) {
        value = check_param(parameters);
}

inline double TFunctionConstant::operator()(double x) const {return value;}

inline std::string TFunctionConstant::ToString() const {return std::to_string(value);}

inline double TFunctionConstant::GetDerivative(double x) const {return 0;}

template<typename T>
TFunctionPower::TFunctionPower(const T &parameters){
    exponent = check_param(parameters);
}

inline double TFunctionPower::operator()(double x) const {
    return std::pow(x, exponent);
}

inline std::string TFunctionPower::ToString() const {
    if (exponent==0.0) return "1";
    if (exponent==1.0) return "x";
    return "x^" + std::to_string(exponent);
}

inline double TFunctionPower::GetDerivative(double x) const {
    return exponent * std::pow(x, exponent - 1);
}

template<typename T>
TFunctionExponent::TFunctionExponent(const T &parameters) {
    T x;
    check_param(x);
}

inline double TFunctionExponent::operator()(double x) const {
    return std::exp(x);
}


std::string TFunctionExponent::ToString() const {
    return "e^x";
}

inline double TFunctionExponent::GetDerivative(double x) const {
    return std::exp(x);
}

template<typename T>
TFunctionPolynomial::TFunctionPolynomial(const T &parameters) {
        try {
            for (auto x : parameters ) {coefficients.push_back(check_param(x));}
            coefficients.shrink_to_fit();
        } catch (std::logic_error) {
          throw std::logic_error("Can't convert to iterable.");
        }
}

double TFunctionPolynomial::operator()(double x) const {
    double partial_sum = 0;
    for (int i = 0; i < coefficients.size(); ++i) {
        partial_sum += coefficients[i] * std::pow(x, i);
    }
    return partial_sum;
}

std::string TFunctionPolynomial::ToString() const {
    std::string partial_sum = "";
    if (coefficients.size() >= 1) partial_sum += std::to_string(coefficients[0]);
    if (coefficients.size() >= 2) partial_sum += " + " + std::to_string(coefficients[1]) + "*x";
    for (int i = 2; i < coefficients.size(); ++i) {
        partial_sum += " + " + std::to_string(coefficients[i]) + "*x^" + std::to_string(i);
    }
    return partial_sum;
}

double TFunctionPolynomial::GetDerivative(double x) const {
    double partial_sum = 0;
    for (int i = 1; i < coefficients.size(); ++i) {
        partial_sum += coefficients[i] * i * std::pow(x, i - 1);
    }
    return partial_sum;
}

TFunctionComposition::TFunctionComposition(const std::shared_ptr<TFunction> &augend,
                             const std::shared_ptr<TFunction> &addend,
				Type type) 
        : augend(augend)
        , addend(addend)
	, type(type) {}

inline double TFunctionComposition::operator()(double x) const {
	switch (type) {
	case Type::addition:
    		return (*augend)(x) + (*addend)(x);
	case Type::subtraction:
		return (*augend)(x) - (*addend)(x);
	case Type::multiplication:
		return (*augend)(x) * (*addend)(x);
	case Type::division:
		return (*augend)(x) / (*addend)(x);
	default:
		return 0.0;
	}
}

inline std::string TFunctionComposition::ToString() const {
	std::string op0 = "(" + augend->ToString() + ")";
	std::string op1 = "(" + addend->ToString() + ")";
	switch (type) {
	case Type::addition:
    		return op0 + " + " + op1;
	case Type::subtraction:
		return op0 + " - " + op1;
	case Type::multiplication:
		return op0 + " * " + op1;
	case Type::division:
		return op0 + " / " + op1;
	default:
		return "ERROR";
	}
}

inline double TFunctionComposition::GetDerivative(double x) const {
	double u_p = augend->GetDerivative(x);
	double v_p = addend->GetDerivative(x);
	switch (type) {
	case Type::addition:
    		return u_p + v_p;
	case Type::subtraction:
		return u_p - v_p;
	case Type::multiplication: {
		double u = (*augend)(x);
		double v = (*addend)(x);
		return u_p * v + u * v_p;
		}
	case Type::division: {
		double u = (*augend)(x);
		double v = (*addend)(x);
		return (u_p * v - u * v_p) / (v*v);
		}
	default:
		return 0.0;
	}
}

std::shared_ptr<TFunctionComposition> operator+(const std::shared_ptr<TFunction> &arg0, 
                        const std::shared_ptr<TFunction> &arg1) {
    return std::make_shared<TFunctionComposition>(arg0, arg1, Type::addition);
}

std::shared_ptr<TFunctionComposition> operator-(const std::shared_ptr<TFunction> &arg0, 
                        const std::shared_ptr<TFunction> &arg1) {
    return std::make_shared<TFunctionComposition>(arg0, arg1, Type::subtraction);
}

std::shared_ptr<TFunctionComposition> operator*(const std::shared_ptr<TFunction> &arg0, 
                        const std::shared_ptr<TFunction> &arg1) {
    return std::make_shared<TFunctionComposition>(arg0, arg1, Type::multiplication);
}

std::shared_ptr<TFunctionComposition> operator/(const std::shared_ptr<TFunction> &arg0, 
                        const std::shared_ptr<TFunction> &arg1) {
    return std::make_shared<TFunctionComposition>(arg0, arg1, Type::division);
}

double optimize_newton(const std::shared_ptr<TFunction> &func,
                               double x0 = 0.0,
                               int maxiter = 1024, double tol = 0.0000001) {

    double p0 = x0;
    int funcalls = 0;
    for (int i=0; i < maxiter; ++i){
            double fval = (*func)(p0);
            funcalls++;
            if (fval==0)
                return p0;
            double fder = func->GetDerivative(p0);
            funcalls++;
            if (fder == 0) {
                std::cerr << "Derivative was zero.";
                std::string msg = " Failed to converge after " + std::to_string(i+1) + " iterations, value is " + std::to_string(p0) + ".";
		}
            double newton_step = fval / fder;
            if (std::abs(newton_step) < tol)
                return p0;
            p0 = p0 - newton_step;
	}
    return 0.0;
}
