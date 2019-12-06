#include <unordered_map>
#include <any>
#include <memory>

#include "factory.h"
#include "my_functions.h"

class TFunctionFactory::PImpl {
    class Fabric {
    public:
        virtual ~Fabric(){}
        virtual std::shared_ptr<TFunction> Create(const std::any &parameters) const = 0;
    };

    using TFabricPtr = std::shared_ptr<Fabric>;
    using TRegisteredFabrics = std::unordered_map<std::string, TFabricPtr>;
    
    TRegisteredFabrics all_fabrics;

public:

    template <typename TObject>
    class ProductionLine: public Fabric {
        std::shared_ptr<TFunction> Create(const std::any &parameters ) const override {
            return std::make_shared<TObject>(parameters);
        }
    };

    PImpl() { 
        register_all();
    }

    template <typename T>
    void register_one(const Type type) {
        TRegisteredFabrics[type] = std::make_shared<ProductionLine<T>>();
    }

    void register_all() {
        register_one<TFunctionIdentity>(Type::ident);
        register_one<TFunctionConstant>(Type::constant);
        register_one<TFunctionPower>(Type::exponentiation);
        register_one<TFunctionExponent>(Type::exp);
        register_one<TFunctionPolynomial>(Type::polynomial);
    }

    std::shared_ptr<TFunction> CreateFunction(Type key, const std::any &params) const {
        auto fabric = TRegisteredFabrics.find(key);
        if (fabric == TRegisteredFabrics.end()) {
            throw std::domain_error;
        } else {
            return fabric->second->Create(params);
        }
    }

    std::vector<Type> get_all_fabrics () const {
        std::vector<Type> result;
        for (auto& tuple : TRegisteredFabrics) {
            result.push_back(tuple.first);
        }
        return result;
    }

};

std::shared_ptr<TFunction> TFactory::CreateFunction(Type type, const std::any &parameters) {
    return Implementation->CreateFunction(type, parameters);
}

TFactory::TFactory() 
    : Implementation(std::make_shared<TFactory::PImpl>()) {}

TFactory::~TFactory(){}
