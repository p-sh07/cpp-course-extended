//
//  random_generator.cpp
//  cpp-practice
//
//  Modern C++ random generator wrapper for convenient use in testing
//
//  Created by ps on 02.05.2024.
//

#include <random>
#include <limits>

class RandomGen {
    RandomGen() {
        
    }
    
    void SetIntLimits();
    inline int GenInt();
    
    SetDoubleLimits();
    inline double GenDbl();
    
private:
    //use default values as max:
    int min_int_ = std::numeric_limits<int>::min()
    int max_int_ = std::numeric_limits<int>::min()
    std::random_device rd_;  // a seed source for the random number engine
    std::mt19937 gen_(rd_()); // mersenne_twister_engine seeded with rd()
    //std::uniform_int_distribution<> distrib(1, 6);
    std::uniform_real_distribution<> distr_(, std::numeric_limits<RealType>::max());
}
