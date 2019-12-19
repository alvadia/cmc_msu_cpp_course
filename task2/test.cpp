#include "my_functions.cpp"
#include <gtest/gtest.h>
#include <random>
#include <cmath>
TEST(Test, TFunctionIdentity) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> distribution(-2<<16, 2<<16);
    auto testee = TFunctionIdentity(Type::none);
    for (int i = 0; i < 1000; ++i){
	double arg = distribution(gen);
    	EXPECT_DOUBLE_EQ(arg, testee(arg));
        EXPECT_DOUBLE_EQ(1.0, testee.GetDerivative(arg));
    }
}
TEST(Test, TFunctionConstant) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> distribution(-2<<16, 2<<16);
    for (int i = 0; i < 1000; ++i){
        double constant = distribution(gen);
        auto testee = TFunctionConstant(constant);
	double arg = distribution(gen);
    	EXPECT_DOUBLE_EQ(constant, testee(arg));
        EXPECT_DOUBLE_EQ(0.0, testee.GetDerivative(arg));
    }
}
TEST(Test, TFunctionPower) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> distribution(0, 10);
    for (int i = 0; i < 1000; ++i){
        double param = distribution(gen);
        auto testee = TFunctionPower(param);
	double arg = distribution(gen);
    	EXPECT_DOUBLE_EQ(std::pow(arg, param), testee(arg));
        EXPECT_DOUBLE_EQ(param * std::pow(arg, param - 1.0), testee.GetDerivative(arg));
    }
}
TEST(Test, TFunctionExponent) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> distribution(-708,708);
    for (int i = 0; i < 1000; ++i){
        auto testee = TFunctionExponent(Type::none);
	double arg = distribution(gen);
    	EXPECT_DOUBLE_EQ(std::exp(arg), testee(arg));
        EXPECT_DOUBLE_EQ(std::exp(arg), testee.GetDerivative(arg));
    }
}
TEST(Test, TFunctionPolynomial) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> distribution(-708.0, 708.0);
    std::uniform_int_distribution<int> int_generator(1, 20);
    for (int i = 0; i < 10; ++i){
	std::vector<double> params;
	for (int j = 0; j < int_generator(gen); ++j)
		params.push_back(distribution(gen));
        auto testee = TFunctionPolynomial(params);
        for (int j = 0; j < 10; ++j){
		double arg = distribution(gen);
		double f = params[0];
		double f_d = 0.0;
		for (int j = 1; j < params.size(); ++j) {
			f += params[j] * std::pow(arg, j);
			f_d += params[j] * j * std::pow(arg, j-1);
		}
	    	EXPECT_DOUBLE_EQ(f, testee(arg));
		EXPECT_DOUBLE_EQ(f_d, testee.GetDerivative(arg));
	}
    }
}
TEST(Test, FactoryBuilding){
    TFunctionFactory factory;
    auto id = factory.Create(Type::ident, Type::none);
    auto constant = factory.Create(Type::constant, 42.0);
    auto power = factory.Create(Type::exponentiation, 2.0);
    auto exp = factory.Create(Type::exp, 1.1);
    auto poly = factory.Create(Type::polynomial, std::vector<double>({1.0, 0.0, 2.0, 3.0}));
EXPECT_EQ("x: for x = 10 is 10.000000", id->ToString() + ": for x = 10 is " + std::to_string((*id)(10)));
EXPECT_EQ("42.000000: for x = 10 is 42.000000", constant->ToString() + ": for x = 10 is " + std::to_string((*constant)(10)));
EXPECT_EQ("x^2.000000: for x = 10 is 100.000000", power->ToString() + ": for x = 10 is " + std::to_string((*power)(10)));
EXPECT_EQ("e^x: for x = 10 is 22026.465795", exp->ToString() + ": for x = 10 is " + std::to_string((*exp)(10)));
    EXPECT_EQ("1.000000 + 0.000000*x + 2.000000*x^2 + 3.000000*x^3: for x = 10 is 3201.000000", poly->ToString() + ": for x = 10 is " + std::to_string((*poly)(10)));
}
