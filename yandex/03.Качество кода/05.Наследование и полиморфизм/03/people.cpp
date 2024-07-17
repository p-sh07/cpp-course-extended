#include "people.h"

#include <stdexcept>

using namespace std;


//===================== Person =====================//
Person::Person(const std::string& name, int age, Gender gender)
: name_(name), age_(age), gender_(gender) {}


const string& Person::GetName() const {
    return name_;
}

int Person::GetAge() const {
    return age_;
}

Gender Person::GetGender() const {
    return gender_;
}

//===================== Person <- Programmer =====================//
Programmer::Programmer(const string& name, int age, Gender gender)
: Person(name, age, gender) {}

void Programmer::AddProgrammingLanguage(ProgrammingLanguage language) {
    langs_.insert(language);
}

bool Programmer::CanProgram(ProgrammingLanguage language) const {
    if(langs_.empty()) {
        return false;
    }
    else return (langs_.count(language) > 0);
}

//===================== Person <- Worker =====================//
Worker::Worker(const string& name, int age, Gender gender)
: Person(name, age, gender) {}

void Worker::AddSpeciality(WorkerSpeciality speciality) {
    specs_.insert(speciality);
}

bool Worker::HasSpeciality(WorkerSpeciality speciality) const {
    if(specs_.empty()) {
        return false;
    }
    else return (specs_.count(speciality) > 0);
}
