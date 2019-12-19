#pragma once

#include <cmath>
#include <optional>
#include <iterator>
#include <memory>
#include <unordered_set>

class TRange {
public:
    using type = int_fast32_t;
    
private:
    type start;
    type final;
    type step;
    type size;

public:
    class const_iterator {
    private:
        type p;
        int step;
        type start;
    	type final;

        const_iterator(type, type, type, type);

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = type;
        using difference_type = type;
        using pointer = type*;
        using reference = type&;
    public:
        const_iterator(const const_iterator& it);
        bool operator!=(const const_iterator& other) const;
        bool operator==(const const_iterator& other) const;
        type operator*() const;
    	const_iterator& operator++();
    	const_iterator operator++(int);

    	friend TRange;
    };

    TRange(type start, type final, type step);
    
    bool operator==(const TRange& operand) const;
    std::optional<type> operator[](size_t i) const;
    type GetSize() const;
    type GetStart() const;
    type GetFinal() const;
    type GetStep() const;
    void GetAllIntegers() const;
    const_iterator begin() const;
    const_iterator end() const;
};

class TRangeHash {
public:
    size_t operator()(const TRange& tR) const;
};
