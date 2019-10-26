#include <iostream>
#include <vector>

// File had english comments, so I decided not to change its style.

template <typename T>
class Array {
private:
    T* data;
    size_t size;

public:
    Array(size_t n)
        : size(n)
    {
        data = new T[n];
    }

    T& operator[](size_t i) {
        return data[i];
    }

    const T& operator[](size_t i) const {
        return data[i];
    }

    Array(const Array &array) {
      // good copy constructor,
      // without any mess with data pointers after copying
        size = array.size;
        data = new T[size];
	for (size_t i = 0; i != size; ++i) {
	    data[i] = array.data[i];
	}
    }

    const Array& operator=(const Array&array){
        return array;
    }
  
    const size_t GetSize() const {
        return size;
    }

    ~Array() {
        delete[] data;
    }
};

template <typename T>
Array<T> GetArray(size_t n, T x) {
  Array<T> A(n); //constructor temporary object
    for (size_t i = 0; i != n; ++i) {
      A[i] = x;//nonconstant []
    }
    return A;//return non-const object
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const Array<T>& A) {
    for (size_t i = 0; i != A.GetSize(); ++i) {//getter
      out << A[i] << " ";//const []
      //print 0
    } //loop n times
    //I don't know why, but 380...0 was printed last in first try.
    out << "\n";
    return out;
}

int main() {
    size_t n;
    std::cin >> n;
    Array<int> A(n); //constructor
    A = GetArray<int>(n, 0); //getarray
    //operator= corrected code - two objects with one data pointer were here previously, but now one object / one pointer
    //destructor of internal temporary copy
    std::cout << A << "\n";// operator <<
    //one can destoy A here, but it will live for the end of main
    std::vector<Array<int>> vec;
    Array<int> B(n); // constructor
    // fill vector of 100 arrays
    for (size_t i = 0; i < 100; ++i) {
      B = GetArray<int>(n, i);//situation is similar to A=...
      vec.push_back(B);//copy constuctor
    }
}
