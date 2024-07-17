#pragma once

#include "identity_document.h"
#include <iostream>
#include <string>

using namespace std::string_view_literals;

class TravelPack {
public:
    TravelPack()
    : identity_document_()
    , identity_doc1_((IdentityDocument*)new Passport())
    , identity_doc2_((IdentityDocument*)new DrivingLicence()) {
        TravelPack::SetVTable(this);
        std::cout << "TravelPack::Ctor()"sv << std::endl;
    }
    
    TravelPack(const TravelPack& other)
    : identity_document_(other.identity_document_)
    , identity_doc1_((IdentityDocument*)(new Passport(*(Passport*)other.identity_doc1_)))
    , identity_doc2_((IdentityDocument*)(new DrivingLicence(*(DrivingLicence*)other.identity_doc2_)))
    , additional_pass_(other.additional_pass_)
    , additional_dr_licence_(other.additional_dr_licence_) {
        TravelPack::SetVTable(this);
        std::cout << "TravelPack::CCtor()"sv << std::endl;
    }
    
    TravelPack& operator=(const TravelPack&) = delete;
    
    ~TravelPack() {
        identity_doc1_->Delete();
        identity_doc2_->Delete();
        std::cout << "TravelPack::Dtor()"sv << std::endl;
        IdentityDocument::SetVTable((IdentityDocument*)this);
    }
    
    //=========== VTable ===============??
    static void SetVTable(TravelPack* obj) {
        *(TravelPack::Vtable**)obj = &TravelPack::VTABLE;
    }
    
    using DeleteFunction = void(*)(TravelPack*);
    using PrintIdFunction = void(*)(const TravelPack*);
    
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
        return (const TravelPack::Vtable*)identity_document_.GetVtable();
    }
    
    Vtable* GetVtable() {
        return (TravelPack::Vtable*)identity_document_.GetVtable();
    }
    
private:
    IdentityDocument identity_document_;
    static TravelPack::Vtable VTABLE;
    
    
    IdentityDocument* identity_doc1_;
    IdentityDocument* identity_doc2_;
    Passport additional_pass_;
    DrivingLicence additional_dr_licence_;
    
    static void Delete(TravelPack* obj) {
        delete obj;
    }
    
    static void PrintID(const TravelPack* obj) {
        obj->identity_doc1_->PrintID();
        obj->identity_doc2_->PrintID();
        obj->additional_pass_.PrintID();
        obj->additional_dr_licence_.PrintID();
    }
};

TravelPack::Vtable TravelPack::VTABLE = { TravelPack::Delete,
    TravelPack::PrintID };
