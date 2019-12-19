#pragma once

#include <iostream>
#include <variant>
#include <optional>
#include <memory>
#include <functional>

#include "TRange.h"
using type = TRange::type;

inline auto next(type p, type step) {
    return p + step;
}

TRange::const_iterator::const_iterator(type p, type N, type M, type s) 
        : step(s)
        , p(p)
        , start(N)
        , final(M) {}

TRange::const_iterator::const_iterator(const const_iterator &iterator)
        : p(iterator.p) 
        , step(iterator.step) {}

bool TRange::const_iterator::operator!=(const const_iterator& operand) const {
    return !(*this == operand);
}

bool TRange::const_iterator::operator==(const const_iterator& operand) const {
    return (p == operand.p);
}

type TRange::const_iterator::operator*() const {
    return p;
}

TRange::const_iterator& TRange::const_iterator::operator++() {
    p = next(p, step);
    if (p < std::min(start, final))
        throw std::out_of_range("Low threshold was trespassed!");
    if (p > std::max(start, final))
        throw std::out_of_range("High threshold was trespassed!");
    return *this;
}

TRange::const_iterator TRange::const_iterator::operator++(int) {
    return ++(*this);
}

TRange::TRange(type start, type final, type step)
    : start(start)
    , final(final)
    , step(step) {
    if (step == 0)
        throw std::logic_error("ERROR: step is equal to 0.");
    if (start==final)
        throw std::logic_error("ERROR: start is equal to final.");
    if ((step > 0) && (final < start))
        throw std::logic_error("Wrong increment direction.");
    if ((step < 0) && (final > start))
        throw std::logic_error("Wrong decrement direction.");
    {
        if (std::abs(final - start) % std::abs(step)) {
            size = std::abs(final - start) / std::abs(step);
        } else {
            size = std::abs(final - start) / std::abs(step) + 1;
        }
    }
}

bool TRange::operator==(const TRange& operand) const {
    return
           (start == operand.start)
        && (final == operand.final)
        && ( step == operand.step );
}

std::optional<type> TRange::operator[](size_t i) const {
    if ((i >= 0) && (i <= size)) {
        return start + step * i;
    } else {
        return std::nullopt;
    }
}

type TRange::GetSize() const {
    return size + 1;
}

type TRange::GetStart() const {
    return start;
}

type TRange::GetFinal() const {
    return final;
}

type TRange::GetStep() const {
    return step;
}

std::ostream& operator<<(std::ostream& out, const TRange& object) {
    for (type i = 0; i < object.GetSize(); ++i) {
        out << *(object[i]) << ' ';
    }
    out << std::endl;
    return out;
}

TRange::const_iterator TRange::begin() const {
    return TRange::const_iterator(start, start, final, step);
}

TRange::const_iterator TRange::end() const {
    return TRange::const_iterator(final + step, start, final, step);
}

template<> struct std::hash<TRange> {
    using temporary = unsigned long long;
    std::size_t operator()(TRange const& object) const noexcept {
        std::hash<temporary> seed0, seed1, seed2;
        return
              seed0(
                (temporary) object.GetSize())
            * seed1(
                (temporary) object.GetStart())
            + seed2(
                (temporary) object.GetStep());
    }
};
