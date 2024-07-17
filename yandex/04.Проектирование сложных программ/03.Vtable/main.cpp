#include <array>
#include <iostream>
#include <string>
#include "identity_document.h"
#include "passport.h"
#include "driving_licence.h"
#include "international_driving_licence.h"
#include "travel_pack.h"
using namespace std;

void PrintInfo(const IdentityDocument& doc) {
    doc.PrintID();
}

void PrintInfo() {
    IdentityDocument::PrintUniqueIDCount();
}

void PrintInfo(const Passport& pass) {
    pass.PrintVisa("France"s);
}

IdentityDocument PrintInfo(int i) {
    Passport pass;
    cout << "PrintInfo("sv << i << ")"sv << endl;
    pass.PrintID();
    return pass;
}

void PrintDrivingLicence(DrivingLicence dr_lic) {
    dr_lic.PrintID();
}

int main() {
    Passport pass;
    PrintInfo(pass);
    PrintInfo(3).PrintID();

    array<IdentityDocument*, 3> docs = { (IdentityDocument*)(new Passport()), (IdentityDocument*)(new DrivingLicence()), (IdentityDocument*)(new Passport()) };
    for (const auto* doc : docs) {
        doc->PrintID();
    }

    for (size_t i = 0; i < docs.size(); ++i) {
        docs[i]->Delete();
    }

    array<IdentityDocument, 3> docs2 = {Passport(), DrivingLicence(), IdentityDocument()};

    for (size_t i = 0; i < docs2.size(); ++i) {
        docs2[i].PrintID();
    }

    PrintInfo(docs2[0]);

    IdentityDocument::PrintUniqueIDCount();

    Passport pass2;
    pass2.PrintUniqueIDCount();

    array<Passport, 4> docs3;

    InternationalDrivingLicence int_dr_lic;
    PrintDrivingLicence(int_dr_lic);

    TravelPack* travel_pack1 = new TravelPack();
    TravelPack* travel_pack2 = new TravelPack(*travel_pack1);
    travel_pack1->PrintID();
    travel_pack2->PrintID();
    travel_pack1->Delete();
    travel_pack2->PrintID();
    travel_pack2->Delete();
}

/*
IdentityDocument::Ctor() : 1
Passport::Ctor()
Passport::PrintVisa(France) : 1
IdentityDocument::Ctor() : 2
Passport::Ctor()
PrintInfo(3)
Passport::PrintID() : 2 expiration date : 13/11/2033
IdentityDocument::CCtor() : 3
Passport::Dtor()
IdentityDocument::Dtor() : 2
IdentityDocument::PrintID() : 3
IdentityDocument::Dtor() : 3
IdentityDocument::Ctor() : 2
Passport::Ctor()
IdentityDocument::Ctor() : 3
DrivingLicence::Ctor()
IdentityDocument::Ctor() : 4
Passport::Ctor()
Passport::PrintID() : 2 expiration date : 13/11/2033
DrivingLicence::PrintID() : 3
Passport::PrintID() : 4 expiration date : 13/11/2033
Passport::Dtor()
IdentityDocument::Dtor() : 2
DrivingLicence::Dtor()
IdentityDocument::Dtor() : 3
Passport::Dtor()
IdentityDocument::Dtor() : 4
IdentityDocument::Ctor() : 2
Passport::Ctor()
IdentityDocument::CCtor() : 3
Passport::Dtor()
IdentityDocument::Dtor() : 2
IdentityDocument::Ctor() : 3
DrivingLicence::Ctor()
IdentityDocument::CCtor() : 4
DrivingLicence::Dtor()
IdentityDocument::Dtor() : 3
IdentityDocument::Ctor() : 4
IdentityDocument::PrintID() : 3
IdentityDocument::PrintID() : 4
IdentityDocument::PrintID() : 4
IdentityDocument::PrintID() : 3
unique_id_count_ : 4
IdentityDocument::Ctor() : 5
Passport::Ctor()
unique_id_count_ : 5
IdentityDocument::Ctor() : 6
Passport::Ctor()
IdentityDocument::Ctor() : 7
Passport::Ctor()
IdentityDocument::Ctor() : 8
Passport::Ctor()
IdentityDocument::Ctor() : 9
Passport::Ctor()
IdentityDocument::Ctor() : 10
DrivingLicence::Ctor()
InternationalDrivingLicence::Ctor()
IdentityDocument::CCtor() : 11
DrivingLicence::CCtor()
DrivingLicence::PrintID() : 11
DrivingLicence::Dtor()
IdentityDocument::Dtor() : 11
IdentityDocument::Ctor() : 11
IdentityDocument::Ctor() : 12
Passport::Ctor()
IdentityDocument::Ctor() : 13
DrivingLicence::Ctor()
IdentityDocument::Ctor() : 14
Passport::Ctor()
IdentityDocument::Ctor() : 15
DrivingLicence::Ctor()
TravelPack::Ctor()
IdentityDocument::CCtor() : 16
IdentityDocument::CCtor() : 17
Passport::CCtor()
IdentityDocument::CCtor() : 18
DrivingLicence::CCtor()
IdentityDocument::CCtor() : 19
Passport::CCtor()
IdentityDocument::CCtor() : 20
DrivingLicence::CCtor()
TravelPack::CCtor()
Passport::PrintID() : 12 expiration date : 13/11/2033
DrivingLicence::PrintID() : 13
Passport::PrintID() : 14 expiration date : 13/11/2033
DrivingLicence::PrintID() : 15
Passport::PrintID() : 17 expiration date : 13/11/2033
DrivingLicence::PrintID() : 18
Passport::PrintID() : 19 expiration date : 13/11/2033
DrivingLicence::PrintID() : 20
Passport::Dtor()
IdentityDocument::Dtor() : 12
DrivingLicence::Dtor()
IdentityDocument::Dtor() : 13
TravelPack::Dtor()
DrivingLicence::Dtor()
IdentityDocument::Dtor() : 15
Passport::Dtor()
IdentityDocument::Dtor() : 14
IdentityDocument::Dtor() : 11
Passport::PrintID() : 17 expiration date : 13/11/2033
DrivingLicence::PrintID() : 18
Passport::PrintID() : 19 expiration date : 13/11/2033
DrivingLicence::PrintID() : 20
Passport::Dtor()
IdentityDocument::Dtor() : 17
DrivingLicence::Dtor()
IdentityDocument::Dtor() : 18
TravelPack::Dtor()
DrivingLicence::Dtor()
IdentityDocument::Dtor() : 20
Passport::Dtor()
IdentityDocument::Dtor() : 19
IdentityDocument::Dtor() : 16
InternationalDrivingLicence::Dtor()
DrivingLicence::Dtor()
IdentityDocument::Dtor() : 10
Passport::Dtor()
IdentityDocument::Dtor() : 9
Passport::Dtor()
IdentityDocument::Dtor() : 8
Passport::Dtor()
IdentityDocument::Dtor() : 7
Passport::Dtor()
IdentityDocument::Dtor() : 6
Passport::Dtor()
IdentityDocument::Dtor() : 5
IdentityDocument::Dtor() : 4
IdentityDocument::Dtor() : 4
IdentityDocument::Dtor() : 3
Passport::Dtor()
IdentityDocument::Dtor() : 1
*/
