#pragma once

#include "identity_document.h"
#include <iostream>
#include <string>

using namespace std::string_view_literals;

class DrivingLicence {
public:
    DrivingLicence() 
    : identity_document_() {
        DrivingLicence::SetVTable(this);
        std::cout << "DrivingLicence::Ctor()"sv << std::endl;
    }

    DrivingLicence(const DrivingLicence& other)
        : identity_document_(other.identity_document_) {
            DrivingLicence::SetVTable(this);
        std::cout << "DrivingLicence::CCtor()"sv << std::endl;
    }
    
    DrivingLicence& operator=(const DrivingLicence&) = delete;

    ~DrivingLicence() {
        std::cout << "DrivingLicence::Dtor()"sv << std::endl;
        IdentityDocument::SetVTable((IdentityDocument*)this);
    }
    
    //?
    int GetID() const {
        return identity_document_.GetID();
    }
    
    //=========== VTable ===============??
    static void SetVTable(DrivingLicence* obj) {
        *(DrivingLicence::Vtable**)obj = &DrivingLicence::VTABLE;
    }

    using DeleteFunction = void(*)(DrivingLicence*);
    using PrintIdFunction = void(*)(const DrivingLicence*);

    struct Vtable {
        DeleteFunction delete_this;
        const PrintIdFunction print_id_this;
    };

    void Delete() {
        GetVtable()->delete_this(this);
    }
    
    void PrintID() const {
        GetVtable()->print_id_this(this);
    }
    
    operator IdentityDocument() {
        return {identity_document_};
    }

    const Vtable* GetVtable() const {
        return (const DrivingLicence::Vtable*)identity_document_.GetVtable();
    }

    Vtable* GetVtable() {
        return (DrivingLicence::Vtable*)identity_document_.GetVtable();
    }
private:
    IdentityDocument identity_document_;
    static DrivingLicence::Vtable VTABLE;
    
    static void Delete(DrivingLicence* obj) {
        delete obj;
    }
    
    static void PrintID(const DrivingLicence* obj) {
        std::cout << "DrivingLicence::PrintID() : "sv << obj->GetID() << std::endl;
    }
};

DrivingLicence::Vtable DrivingLicence::VTABLE = { DrivingLicence::Delete,
    DrivingLicence::PrintID };
