#include "people.h"

#include <cassert>
#include <iostream>

int main() {
    using namespace std;

    try {
        Programmer programmer{"Bjarne Stroustrup"s, 69, Gender::MALE};
        programmer.AddProgrammingLanguage(ProgrammingLanguage::CPP);
        assert(programmer.CanProgram(ProgrammingLanguage::CPP));

        Worker worker{"Mario"s, 42, Gender::MALE};
        worker.AddSpeciality(WorkerSpeciality::PLUMBER);
        assert(worker.HasSpeciality(WorkerSpeciality::PLUMBER));
        cerr << worker.GetName() << ' ' << worker.GetAge() << endl;
    } catch (...) {
        cout << "Exception was thrown"s << endl;
    }
    cout << "OK"s << endl;
}
