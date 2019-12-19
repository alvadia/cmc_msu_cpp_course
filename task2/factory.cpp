#pragma once
#include <unordered_map>
#include <any>
#include <memory>
#include <exception>

#include "factory.h"
#include "my_functions.h"

class TFunctionFactory::PImpl {
    class Fabric {
    public:
        virtual ~Fabric(){}
        template <typename A>
        std::shared_ptr<TFunction> Create(const A &parameters) const;
    };

    using TFabricPtr = std::shared_ptr<Fabric>;
    
    std::unordered_map<Type, TFabricPtr> all_fabrics;

public:

    template <typename T>
    class ProductionLine: public Fabric {
        template <typename A>
        std::shared_ptr<TFunction> Create(const A &parameters) const {
            return std::make_shared<T>(parameters);
        }
    };

    PImpl() { 
        register_all();
    }

    template <typename T>
    void register_one(Type type) {
        all_fabrics[type] = std::make_shared<ProductionLine<T>>();
    }

    void register_all() {
        register_one<TFunctionIdentity>(Type::ident);
        register_one<TFunctionConstant>(Type::constant);
        register_one<TFunctionPower>(Type::exponentiation);
        register_one<TFunctionExponent>(Type::exp);
        register_one<TFunctionPolynomial>(Type::polynomial);
    }
    template <typename A>
    std::shared_ptr<TFunction> CreateFunction(Type key, const A &params) const {
        auto fabric = all_fabrics.find(key);
        if (fabric == all_fabrics.end()) {
            throw std::domain_error("0");
        } else {
            return fabric->second->Create<A>(params);
        }
    }

    std::vector<Type> get_all_fabrics () const {
        std::vector<Type> result;
        for (auto& tuple : all_fabrics) {
            result.push_back(tuple.first);
        }
        return result;
    }

};

template <typename A>
std::shared_ptr<TFunction> TFunctionFactory::Create(Type type, const A &parameters) {
    return Implementation->CreateFunction<A>(type, parameters);
}

TFunctionFactory::TFunctionFactory() 
    : Implementation(std::make_shared<TFunctionFactory::PImpl>()) {
}

TFunctionFactory::~TFunctionFactory(){}
