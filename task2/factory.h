#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <cmath>
#include "my_functions.h"

class TFunctionFactory {
	class PImpl;
    std::shared_ptr<const PImpl> Implementation;

public:
    TFunctionFactory();
    std::shared_ptr<TFunction> Create(const Type type, const std::any &params);
    std::vector<Type> get_all_fabrics();
    ~TFunctionFactory();
};
