#include <iostream>
#include <cassert>

template<char ...charstring>
struct mystr {
    static constexpr size_t size = sizeof...(charstring);

    template <char ...addend> 
    constexpr auto operator+(mystr<addend...> const &augend) const noexcept {
        return mystr<charstring..., addend...>();
    }

    template <char ...model>
    constexpr bool operator==(mystr<model...> const &sample) const noexcept {
        return size == sample.size;
    }
};

template<typename charstring, charstring ...res>
constexpr auto operator"" _s() noexcept
{
    return mystr<res...>();
}


int main() {
  constexpr auto hello = "hello"_s + " world"_s;
  static_assert(hello == "hello world"_s);
}
