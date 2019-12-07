#pragma once
#include "my_functions.h"

#define _USE_MATH_DEFINES // for C++

TFunction::TFunction(){}

TFunctionIdentity::TFunctionIdentity(const std::any &parameters) {}

inline double TFunctionIdentity::operator()(double x) const {return x;}

inline std::string TFunctionIdentity::ToString() const {return "x";}

inline double TFunctionIdentity::GetDerivative(double x) const {return 1;}

TFunctionConstant::TFunctionConstant(const std::any &parameters) {
    try {
        value = std::any_cast<double>(parameters);
    } catch (const std::bad_any_cast& e) {
        throw std::logic_error("Bad constant cast: can't extract double value.");
    }}

inline double TFunctionConstant::operator()(double x) const {return value;}

inline std::string TFunctionConstant::ToString() const {return std::to_string(value);}

inline double TFunctionConstant::GetDerivative(double x) const {return 0;}

TFunctionPower::TFunctionPower(const std::any &parameters){
    exponent = std::any_cast<double>(parameters);
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


TFunctionExponent::TFunctionExponent(const std::any &parameters) {}

inline double TFunctionExponent::operator()(double x) const {
    return std::exp(x);
}


std::string TFunctionExponent::ToString() const {
    return "e^x";
}

inline double TFunctionExponent::GetDerivative(double x) const {
    return std::exp(x);
}

TFunctionPolynomial::TFunctionPolynomial(const std::any &parameters) {
    try {
        coefficients = std::any_cast<std::vector<double>>(parameters);
	coefficients.shrink_to_fit();
    } catch (const std::bad_any_cast& e) {
	//rethrow;
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
    if (coefficients.size() >= 2) partial_sum += " " + std::to_string(coefficients[1]) + "*x";
    for (int i = 2; i < coefficients.size(); ++i) {
        partial_sum += " " + std::to_string(coefficients[i]) + "*x^" + std::to_string(i);
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
