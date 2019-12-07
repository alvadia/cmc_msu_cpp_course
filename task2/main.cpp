#include <iostream>
#include <functional>
#include "factory.cpp"
#include "my_functions.cpp"
#include "test.cpp"

using TFunctionPtr = std::shared_ptr<TFunction>;

int main(int argc, char* argv[]){
    TFunctionFactory factory;
    auto id = factory.Create(Type::ident);
    auto constant = factory.Create(Type::constant, 42.0);
    auto power = factory.Create(Type::exponentiation, 2.0);
    auto exp = factory.Create(Type::exp, 1.1);
    auto poly = factory.Create(Type::polynomial, std::vector<double>({1.0, 0.0, 2.0, 3.0}));
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
    
    auto equation = factory.Create(Type::polynomial, std::vector<double>({1.0, -2.0, 1.0}));
    double root = optimize_newton(equation);

    std::cout << equation->ToString() << " root = " << root << std::endl;
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
