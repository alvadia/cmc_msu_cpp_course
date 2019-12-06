#include "my_functions.h"

#define _USE_MATH_DEFINES // for C++

TFunctionIdentity::TFunctionIdentity(const std::any &parameters) {}

inline double TFunctionIdentity::operator()(double x) const noexcept {}

inline const std::string TFunctionIdentity::ToString() const noexcept {return "x";}

inline const double IdentFunc::GetDerivative(double x) const noexcept {return 1;}

TFunctionConstant::TFunctionConstant(const std::any &parameters) {
    try {
        value = std::any_cast<double>(parameters);
    } catch (const std::bad_any_cast& e) {
        int int_value = std::any_cast<int>(parameters);
        value = static_cast<double>(int_value);
    }}

inline double TFunctionConstant::operator()(double x) const {return value;}

inline std::string TFunctionConstant::ToString() const {return std::to_string(value);}

inline const double TFunctionConstant::GetDerivative(double x) const noexcept {return 0;}

TFunctionPower::TFunctionPower(const std::any &parameters){
    try {
        exponent = std::any_cast<double>(parameters);
    } catch (const std::bad_any_cast& e) {
        int int_value = std::any_cast<int>(parameters);
        exponent = static_cast<double>(int_value);
    }
}

inline double TFunctionPower::operator()(double x) const {
    return std::pow(x, power);
}

inline std::string TFunctionPower::ToString() const {
    switch (exponent) {
    case 0.0:
	return "1";
	case 1.0:
	return "x";
	default:
	return "x^" + std::to_string(exponent);
    }
}

inline double TFunctionPower::GetDerivative(double x) const {
    return power * std::pow(x, power - 1);
}


TFunctionExponent::TFunctionExponent(const std::any &parameters) 
        : TFunctionPower(std::any(M_E)) {}

std::string TFunctionExponent::ToString() const {
    return "e^x";
}

TFunctionPolynomial::TFunctionPolynomial(const std::any &parameters) {
    try {
        coefficients = std::any_cast<std::vector<double>>(parameters);
	coefficients.shrink_to_fit()
    } catch (const std::bad_any_cast& e) {
	coefficients = std::any_cast<std::vector<int>>(parameters);
	coefficients.shrink_to_fit();
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

inline double TFunctionComposition::operator()(double x) const noexcept {
	switch (type) {
	case Type::addition:
    		return (*augend)(x) + (*addend)(x);
	case Type::subtraction:
		return (*augend)(x) - (*addend)(x);
	case Type::multiplication:
		return (*augend)(x) * (*addend)(x);
	case Type::division:
		return (*augend)(x) / (*addend)(x);
	}
}

inline std::string TFunctionComposition::ToString() const noexcept {
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
	case Type::multiplication:
		double u = (*augend)(x);
		double v = (*addend)(x);
		return u_p * v + u * v_p;
	case Type::division:
		double u = (*augend)(x);
		double v = (*addend)(x);
		return (u_p * v - u * v_p) / (v*v);
	}
}

TFunctionComposition operator+(const std::shared_ptr<TFunction> &arg0, 
                        const std::shared_ptr<TFunction> &arg1) {
    return TFunctionComposition(arg0, arg1, Type::addition);
}

TFunctionComposition operator-(const std::shared_ptr<TFunction> &arg0, 
                        const std::shared_ptr<TFunction> &arg1) {
    return TFunctionComposition(arg0, arg1, Type::subtraction);
}

TFunctionComposition operator*(const std::shared_ptr<TFunction> &arg0, 
                        const std::shared_ptr<TFunction> &arg1) {
    return TFunctionComposition(arg0, arg1, Type::multiplication);
}

TFunctionComposition operator/(const std::shared_ptr<TFunction> &arg0, 
                        const std::shared_ptr<TFunction> &arg1) {
    return TFunctionComposition(arg0, arg1, Type::division);
}

double optimize_newton(const std::shared_ptr<TFunction> &func,
                               double x0,
                               int maxiter = 1024, double tol = 0.0000001) {

    double p0 = x0;
    int funcalls = 0;
    for (int i=0; i < maxiter; ++i){
            double fval = func(p0);
            funcalls++;
            if (fval==0)
                return p0;
            double fder = func->GetDerivative(p0);
            funcalls++;
            if (fder == 0) {
                std::cerr << "Derivative was zero.";
                std::string msg = " Failed to converge after " + std::to_string(itr+1) + " iterations, value is " + std::to_string(p0) + ".";
            double newton_step = fval / fder;
            if (std::abs(newton_step) < tol)
                return p0;
            p0 = p0 - newton_step;
	}
}
