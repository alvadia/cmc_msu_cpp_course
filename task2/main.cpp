#include <iostream>
#include <functional>
#include "factory.h"
#include "my_functions.h"

using TFunctionPtr = std::shared_ptr<TFunction>;

int main() {
    
    TFunctionFactory factory;
    //std::initializer_list<int> list = {1, 2, 3};
    std::vector<TFunctionPtr> cont;

    auto id = factory.Create(Type::ident);
cont.push_back(id);
    auto constant = factory.Create(Type::constant, 42);
cont.push_back(constant);
    auto power = factory.Create(Type::exponentiation, 2);
cont.push_back(power);
    auto exp = factory.Create(Type::exp);
cont.push_back(exp);
    auto poly = factory.Create(Type::polynomial, std::vector<int>({1, 0, 2, 3}));
cont.push_back(poly);
    for (const auto &ptr: cont) {
        std::cout << ptr->ToString() << ": for x = 10 is " << (*ptr)(10) << std::endl;
    }
    std::vector<TFunctionPtr> testee;
    auto f = id + constant;
cont.push_back(f);
    auto p = power - exp;
cont.push_back(p);
    auto g = poly / exp;
cont.push_back(g);
    auto null = factory.Create(Type::constant, 0);
cont.push_back(null);
    for (const auto &ptr: testee) {
            std::cout << ptr->ToString() << std::endl;
	    std::cout << (*ptr)(2) << std::endl;
	    std::cout << ptr->GetDerivative(2) << std::endl;
    }

    //double Calculate_function_root(const std::shared_ptr<TFunction> &func,
    //                           double initial_value,
     //                          int iter_number);

    
    //auto poly_func2 = factory.Create("poly function", std::vector<int>({1, -2, 1}));    
    
    //double root = Calculate_function_root(poly_func2, -2, 10);

    //std::cout << "root = " << root << std::endl;

    return 0;
}
