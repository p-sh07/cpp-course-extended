#include <cassert>
#include <iostream>
#include <string>

using namespace std;

class House {
public:
    House(int length, int width, int height)
    : length_(length), width_(width), height_(height) {}
    
    int GetLength() const {
        return length_;
    }
    int GetWidth() const {
        return width_;
    }
    int GetHeight() const {
        return height_;
    }
private:
    int length_, width_, height_;
};

class Resources {
public:
    Resources(int brick_count)
    : brick_count_(brick_count) {}
    
    void TakeBricks(int count) {
        if (count < 0) {
            throw std::out_of_range("negative num. of bricks passed to TakeBricks()");
        } else if (brick_count_ < count) {
            throw std::out_of_range("Not enough bricks in Resources");
        }
        brick_count_ -= count;
    }
    
    int GetBrickCount() const {
        return brick_count_;
    }
    
private:
    int brick_count_;
};

struct HouseSpecification {
    int length = 0;
    int width = 0;
    int height = 0;
};

class Builder {
public:
    Builder(Resources& res)
    : resources_(res) {}
    
    House BuildHouse(HouseSpecification spec) {
        int required_bricks = ComputeNumBricks(spec);
        
        if(resources_.GetBrickCount() < required_bricks) {
            throw std::runtime_error("Not enough bricks for Builder to build house");
        }
        
        resources_.TakeBricks(required_bricks);
        
        return House(spec.length, spec.width, spec.height);
    }
    
private:
    static const int BRICKS_PER_METER_SQUARED = 32;
    Resources& resources_;
    int ComputeNumBricks(const HouseSpecification& spec) const {
        //total area = w * h * 2 + l * h * 2 = 2h(w+l);
        int wall_area = 2 * spec.height * (spec.length + spec.width);
        return wall_area * BRICKS_PER_METER_SQUARED;
    }
};

int main() {
    Resources resources{10000};
    Builder builder1{resources};
    Builder builder2{resources};

    House house1 = builder1.BuildHouse(HouseSpecification{12, 9, 3});
    assert(house1.GetLength() == 12);
    assert(house1.GetWidth() == 9);
    assert(house1.GetHeight() == 3);
    cout << resources.GetBrickCount() << " bricks left"s << endl;

    House house2 = builder2.BuildHouse(HouseSpecification{8, 6, 3});
    assert(house2.GetLength() == 8);
    cout << resources.GetBrickCount() << " bricks left"s << endl;

    House banya = builder1.BuildHouse(HouseSpecification{4, 3, 2});
    assert(banya.GetHeight() == 2);
    cout << resources.GetBrickCount() << " bricks left"s << endl;
}
