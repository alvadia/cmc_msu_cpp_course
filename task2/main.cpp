#include <iostream>
#include <functional>
#include "factory.h"
#include "factory.cpp"
#include "my_functions.h"
#include "my_functions.cpp"
#//include <gmock/gmock.h>
#include <gtest/gtest.h>

using TFunctionPtr = std::shared_ptr<TFunction>;

int main(){
    
    TFunctionFactory factory;
    std::vector<TFunctionPtr> cont;

    auto id = factory.Create(Type::ident);
cont.push_back(id);
    auto constant = factory.Create(Type::constant, 42);
cont.push_back(constant);
    auto power = factory.Create(Type::exponentiation, 2);
cont.push_back(power);
    auto exp = factory.Create(Type::exp, 1.1);
cont.push_back(exp);
    auto poly = factory.Create(Type::polynomial, std::vector<int>({1, 0, 2, 3}));
cont.push_back(poly);
    for (const auto &ptr: cont) {
        std::cout << ptr->ToString() << ": for x = 2 is " << (*ptr)(10) << std::endl;
    }
    std::vector<TFunctionPtr> testee;
    auto f = id + constant;
testee.push_back(f);
    auto p = power - exp;
testee.push_back(p);
    auto g = poly / exp;
testee.push_back(g);
    for (const auto &ptr: testee) {
            std::cout << ptr->ToString() << std::endl;
	    std::cout << (*ptr)(2) << std::endl;
	    std::cout << ptr->GetDerivative(2) << std::endl;
    }
    
    auto poly_func2 = factory.Create(Type::polynomial, std::vector<double>({1.0, -2.0, 1.0}));
    double root = optimize_newton(poly_func2, 3.0);

    std::cout << "root = " << root << std::endl;

    return 0;
}
