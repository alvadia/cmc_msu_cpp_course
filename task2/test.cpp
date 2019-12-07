#include "my_functions.cpp"
#include <gtest/gtest.h>
#include <random>
#include <cmath>
TEST(Test, TFunctionIdentity) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> distribution(-2<<16, 2<<16);
    auto testee = TFunctionIdentity(std::any(Type::none));
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
        auto testee = TFunctionConstant(std::any(constant));
	double arg = distribution(gen);
    	EXPECT_DOUBLE_EQ(constant, testee(arg));
        EXPECT_DOUBLE_EQ(0.0, testee.GetDerivative(arg));
    }
}
TEST(Test, TFunctionPower) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> distribution(-2<<16, 2<<16);
    for (int i = 0; i < 1000; ++i){
        double param = distribution(gen);
        auto testee = TFunctionPower(std::any(param));
	double arg = distribution(gen);
    	EXPECT_DOUBLE_EQ(std::pow(arg, param), testee(arg));
        EXPECT_DOUBLE_EQ(param * std::pow(arg, param - 1), testee.GetDerivative(arg));
    }
}
TEST(Test, TFunctionExponent) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> distribution(-2<<16,2<<16);
    for (int i = 0; i < 1000; ++i){
        auto testee = TFunctionPower(std::any(Type::none));
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
        auto testee = TFunctionPolynomial(std::any(params));
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
