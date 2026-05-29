#pragma once

#include <mutex>
#include <atomic>

#include "../Int_Number/Int_Number.h"


class Quotient_Number {
    Int_Number up;
    Int_Number doun;
    bool sign_plus;

    static std::atomic<long long> fractional_part_output_length;

    mutable std::mutex guard;
    
    void Irreducible_Fraction_Bypass();



public:
    static void Set_Precision(long long i){
        if (i > 0)
            fractional_part_output_length = i;
    }
    
    Quotient_Number();
    Quotient_Number(int u);
    Quotient_Number(const Int_Number& u);
    Quotient_Number(const Int_Number& u1, const Int_Number& u2);
    Quotient_Number(const Int_Number& u1, const Int_Number& u2, bool plus);
    Quotient_Number(const Quotient_Number& q);

    Quotient_Number(Quotient_Number&& q);
    Quotient_Number(Int_Number&& u1, Int_Number&& u2);
    Quotient_Number(Int_Number&& u1, Int_Number&& u2, bool plus);
    
    
    void Make_Irreducible_Fraction();


    int operator <=> (const Quotient_Number& Q) const;

    bool operator >(const Quotient_Number& Q) const{return (*this <=> Q) == 1 ? true : false;}
    bool operator <(const Quotient_Number& Q) const{return (*this <=> Q) == -1 ? true : false;}
    bool operator >= (const Quotient_Number& Q) const{return (*this <=> Q) == -1 ? false : true;}
    bool operator <= (const Quotient_Number& Q) const{return (*this <=> Q) == 1 ? false : true;}
    bool operator == (const Quotient_Number& Q) const{return (*this <=> Q) == 0 ? true : false;}
    bool operator != (const Quotient_Number& Q) const{return (*this <=> Q) == 0 ? false : true;}
    
    Quotient_Number& operator - ();

    Quotient_Number operator+(const Quotient_Number& Q) const;
    Quotient_Number operator-(const Quotient_Number& Q) const;
    Quotient_Number operator*(const Quotient_Number& Q) const;
    Quotient_Number operator/(const Quotient_Number& Q) const;


    void operator=(Quotient_Number&& Q);
    void operator=(const Quotient_Number& Q);
    
    void operator+=(const Quotient_Number& Q);
    void operator-=(const Quotient_Number& Q);
    void operator*=(const Quotient_Number& Q);
    void operator/=(const Quotient_Number& Q);

    
    
    friend std::ostream& operator<<(std::ostream& os, const Quotient_Number& Q);
    friend std::ofstream& operator<<(std::ofstream& of, const Quotient_Number& Q);

    friend std::istream& operator >> (std::istream& is, Quotient_Number& Q);
    friend std::ifstream& operator >> (std::ifstream& is, Quotient_Number& Q);
};



