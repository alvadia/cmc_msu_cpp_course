
#include <ctime>
#include <map>
#include <iostream>
using type = int;
using map = std::map<type, std::string>;
int check_map_with_hint(type cycle, type reps) {
    map testee;
    {   // creating 0..cycle, cycle*2..cycle*3 elements
        for(type i=0; i<cycle;++i) {
            testee.insert(std::make_pair(i,std::to_string(i)));
        }
        for(type i=2*cycle; i<3*cycle;++i) {
            testee.insert(std::make_pair(i,std::to_string(i)));
        }
    }
    std::clock_t start = std::clock();
    for(type i=0; i<reps;i++)
    {   // test previous value hints
        map clone(testee);
        map::iterator iter = clone.lower_bound(cycle-1);
        for(type i=cycle; i<2*cycle;++i) {
            iter = clone.insert(iter,std::make_pair(i,std::to_string(i)));
        }
    }
    std::clock_t end = std::clock();
    std::cout << static_cast<double>((end - start) ) << " ";
    return 0;
}

int check_map_without_hint(type cycle, type reps){
    map testee;
    {   // creating 0..cycle, cycle*2..cycle*3 elements
        for(type i=0; i<cycle;++i) {
            testee.insert(std::make_pair(i,std::to_string(i)));
        }
        for(type i=2*cycle; i<3*cycle;++i) {
            testee.insert(std::make_pair(i,std::to_string(i)));
        }
    }
    {
        std::clock_t start = std::clock();
        for(type i=0; i<reps;++i)
        {   // no hint
            map clone(testee);
            for(type i=cycle; i<2*cycle;++i) {
                clone.insert(std::make_pair(i,std::to_string(i)));
            }
        }
        std::clock_t end = std::clock();
        std::cout << static_cast<double>((end - start)) << " ";
    }
    return 0;
}

int main(int argc, char const *argv[]) {
    int mode = std::stoi(argv[1]);
    if (mode==0) return check_map_with_hint(1<<16, 10); // call with param 0
    else return check_map_without_hint(1<<16, 10); // call with param 1
}
