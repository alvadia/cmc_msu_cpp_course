#include <iostream>
#include <cassert>

template<char c, char...charstring>
struct mystr {
    template <char ...addend> 
    constexpr auto operator+(mystr<addend...> const &augend) const noexcept {
        return mystr<c, charstring..., addend...>();
    }

    constexpr bool operator==(mystr<c> const &sample) const noexcept {
        return true;
    }
    template <char ...model>
    constexpr bool operator==(mystr<c, model...> const &sample) const noexcept {
        return mystr<model...>()==mystr<charstring...>();
    }
    template <char ...model>
    constexpr bool operator==(mystr<model...> const &sample) const noexcept {
        return false;
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
