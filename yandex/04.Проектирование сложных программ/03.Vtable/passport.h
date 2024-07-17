#pragma once

#include "identity_document.h"
#include <iostream>
#include <string>
#include <ctime>

using namespace std::string_view_literals;

class Passport {
public:
    Passport()
    : identity_document_()
    , expiration_date_(GetExpirationDate()) {
        Passport::SetVTable(this);
        std::cout << "Passport::Ctor()"sv << std::endl;
    }

    Passport(const Passport& other)
        : identity_document_(other.identity_document_)
        , expiration_date_(other.expiration_date_)
    {
        Passport::SetVTable(this);
        std::cout << "Passport::CCtor()"sv << std::endl;
    }

    Passport& operator=(const Passport&) = delete;
    
    ~Passport() {
        std::cout << "Passport::Dtor()"sv << std::endl;
        IdentityDocument::SetVTable((IdentityDocument*)this);
    }
    
    //was vrtl!
    void PrintVisa(const std::string& country) const {
        std::cout << "Passport::PrintVisa("sv << country << ") : "sv << GetID() << std::endl;
    }
    
    int GetID() const {
        return identity_document_.GetID();
    }
    
    void PrintUniqueIDCount() {
        identity_document_.PrintUniqueIDCount();
    }
    
    //=========== VTable ===============??
    static void SetVTable(Passport* obj) {
        *(Passport::Vtable**)obj = &Passport::VTABLE;
    }

    using DeleteFunction = void(*)(Passport*);
    using PrintIdFunction = void(*)(const Passport*);

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
        return (const Passport::Vtable*)identity_document_.GetVtable();
    }

    Vtable* GetVtable() {
        return (Passport::Vtable*)identity_document_.GetVtable();
    }

private:
    IdentityDocument identity_document_;
    static Passport::Vtable VTABLE;
    
    static void Delete(Passport* obj) {
        delete obj;
    }
    
    static void PrintID(const Passport* obj) {
        std::cout << "Passport::PrintID() : "sv << obj->GetID();
        std::cout << " expiration date : "sv << obj->expiration_date_.tm_mday << "/"sv << obj->expiration_date_.tm_mon << "/"sv
                  << obj->expiration_date_.tm_year + 1900 << std::endl;
    }
    
    const struct tm expiration_date_;

    tm GetExpirationDate() {
	    time_t t = time(nullptr);
	    tm exp_date = *localtime(&t);
	    exp_date.tm_year += 10;
	    mktime(&exp_date);
	    return exp_date;
	}
};

Passport::Vtable Passport::VTABLE = { Passport::Delete,
    Passport::PrintID };
