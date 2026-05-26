#pragma once

#include <vector>
#include <string>
#include <mutex>
#include <thread>



#include "Library/Service_Names/Service_Names.h"
#include "Library/Error/Error.h"
#include "Library/Thread_Pool/Thread_Pool.h"






class Int_Number {
    int* num;
    int ptr_head;
    int plt_tail;
    int len_dedicated;
    long len_num;
    bool sign_plus;

    mutable std::mutex guard;

    static Thread_Pool thread_pool;

    static void Division_NOT_Bypass(Int_Number& R, Int_Number& Nu2, Int_Number& Q){
        if(R.len_num < Nu2.len_num)
            return;
        
        if(R < Nu2)
            return;

        bool plus_flag = true;

        while(Abs(R) >= Nu2){
            if(!plus_flag)
                -R;

            long long q;
            int move_flag = 0;


            if (R.num[R.ptr_head] >= Nu2.num[Nu2.ptr_head])
                q = (R.num[R.ptr_head] / Nu2.num[Nu2.ptr_head]);
            
            else{
                move_flag = 1;
                q = (((long long)R.num[R.ptr_head] * (long long)1000000000 + (long long)R.num[R.ptr_head-1]) / (Nu2.num[Nu2.ptr_head] + 1));
            }
            
            int go = ((R.len_num - 1) / 9 + 1) - ((Nu2.len_num - 1) / 9 + 1) - move_flag;

            if(plus_flag){
                R -= (Nu2 * q) << go;
                Q += Int_Number(q) << go;
            }
            else{
                R += (Nu2 * q) << go;
                Q -= Int_Number(q) << go;
            }


            if(R.Is_plus())
                plus_flag = true;
            else
                plus_flag = false;
        }
    }
    friend std::ostream& operator<<(std::ostream& os, const Quotient_Number& Q);
    friend std::ofstream& operator<<(std::ofstream& of, const Quotient_Number& Q);




    void Relocation_Bypass(int count);

    Int_Number Copy_Bypass() const;


    int Comparator_Bypass(const Int_Number& Nu) const;
    int ABS_Comparator_Bypass(const Int_Number& Nu) const;


    void Only_Add_With_Shift_Bypass(const Int_Number& Nu, int shift = 0);

    void Add_Bypass(const Int_Number& Nu);
    void Subtract_Bypass(const Int_Number& Nu);



    //transition_block, Nu.num, this.num, poz_start_mas_block, poz_start_mas_in, poz_start_mas_out, count_block
    static void __local__Add_In_Bypass_Paralel(void** mas){
        long sum;

        int* transition_block = static_cast<int*>(mas[0]);
        int* Nu_num = static_cast<int*>(mas[1]);
        int* this_num = static_cast<int*>(mas[2]);

        int poz_start_mas_block = *static_cast<int*>(mas[3]);
        int poz_start_mas_in = *static_cast<int*>(mas[4]);
        int poz_start_mas_out = *static_cast<int*>(mas[5]);
        int count_block = *static_cast<int*>(mas[6]);

        for(int i = 0; i < count_block; i++){
            sum = this_num[poz_start_mas_in + i] + Nu_num[poz_start_mas_out + i];
            transition_block[poz_start_mas_block + i] = sum / 1000000000;
            this_num[poz_start_mas_in + i] = sum % 1000000000;
        }
        
        for(int i = 0; i < count_block-1; i++){
            sum = this_num[poz_start_mas_in + i + 1] + transition_block[poz_start_mas_block + i];
            transition_block[poz_start_mas_block + i + 1] += sum / 1000000000;
            this_num[poz_start_mas_in + i + 1] = sum % 1000000000;
            transition_block[poz_start_mas_block + i] = 0;
        }
    };
    void Add_In_Bypass_Paralel(const Int_Number& Nu);


    //transition_block, Nu.num, this.num, poz_start_mas_block, poz_start_mas_in, poz_start_mas_out, count_block
    static void __local_first__Subtract_In_Bypass_Paralel(void** mas){
        long subt;

        int* transition_block = static_cast<int*>(mas[0]);
        int* Nu_num = static_cast<int*>(mas[1]);
        int* this_num = static_cast<int*>(mas[2]);

        int poz_start_mas_block = *static_cast<int*>(mas[3]);
        int poz_start_mas_in = *static_cast<int*>(mas[4]);
        int poz_start_mas_out = *static_cast<int*>(mas[5]);
        int count_block = *static_cast<int*>(mas[6]);


        for(int i = 0; i < count_block; i++){
            subt = this_num[poz_start_mas_in + i] - Nu_num[poz_start_mas_out + i];
            
            if(subt < 0){
                transition_block[poz_start_mas_block + i] = -1;
                this_num[poz_start_mas_in + i] = 1000000000 + subt;
            }
            else{
                transition_block[poz_start_mas_block + i] = 0;
                this_num[poz_start_mas_in + i] = subt;
            }
        }
        
        for(int i = 0; i < count_block-1; i++){
            subt = this_num[poz_start_mas_in + i + 1] + transition_block[poz_start_mas_block + i];
            
            if(subt < 0){
                transition_block[poz_start_mas_block + i + 1] = -1;
                transition_block[poz_start_mas_block + i] = 0;
                this_num[poz_start_mas_in + i + 1] = 1000000000 + subt;
            }
            else{
                transition_block[poz_start_mas_block + i] = 0;
                this_num[poz_start_mas_in + i + 1] = subt;
            }
        }
    };
    static void __local_second__Subtract_In_Bypass_Paralel(void** mas){
        long subt;

        int* transition_block = static_cast<int*>(mas[0]);
        int* Nu_num = static_cast<int*>(mas[1]);
        int* this_num = static_cast<int*>(mas[2]);

        int poz_start_mas_block = *static_cast<int*>(mas[3]);
        int poz_start_mas_in = *static_cast<int*>(mas[4]);
        int poz_start_mas_out = *static_cast<int*>(mas[5]);
        int count_block = *static_cast<int*>(mas[6]);


        for(int i = 0; i < count_block; i++){
            subt = -(this_num[poz_start_mas_in + i]) + Nu_num[poz_start_mas_out + i];
            
            if(subt < 0){
                transition_block[poz_start_mas_block + i] = -1;
                this_num[poz_start_mas_in + i] = 1000000000 + subt;
            }
            else{
                transition_block[poz_start_mas_block + i] = 0;
                this_num[poz_start_mas_in + i] = subt;
            }
        }
        
        for(int i = 0; i < count_block-1; i++){
            subt = this_num[poz_start_mas_in + i + 1] + transition_block[poz_start_mas_block + i];
            
            if(subt < 0){
                transition_block[poz_start_mas_block + i + 1] = -1;
                transition_block[poz_start_mas_block + i] = 0;
                this_num[poz_start_mas_in + i + 1] = 1000000000 + subt;
            }
            else{
                transition_block[poz_start_mas_block + i] = 0;
                this_num[poz_start_mas_in + i + 1] = subt;
            }
        }
    };
    void Subtract_In_Bypass_Paralel(const Int_Number& Nu);


    Int_Number& Shift_Right_Bypass(int i);
    Int_Number& Shift_Left_Bypass(int i);


    void Multiplication_Bypass(int q);
    void Multiplication_Bypass(const Int_Number& Nu);
    //В НЕЕ НЕЛЬЗЯ ПИХАТЬ ОДИН И ТОТ ЖЕ ОБЬЕКТ(ЧИСЛО)

    Int_Number Multiplication_NTT_Outward_Bypass(const Int_Number& Nu) const;

public:
    
    Int_Number();
    Int_Number(int len_dedicated1, int plt_tail1);
    Int_Number(int n);
    Int_Number(long long int n);
    Int_Number(Int_Number&& Nu);
    Int_Number(const Int_Number& Nu);
    Int_Number(const std::string& line_num);

    
    
    void All_Print(int count = 10) const;
    void Technical_Print() const;
    void Relocation(int count = 3);
    bool Is_plus() const;
    int Length_Number() const;
    friend Int_Number& Abs(Int_Number& Nu);

    std::string To_String() const;
    std::string To_Clear_String() const;

    

    void operator = (Int_Number&& Nu);
    void operator = (const Int_Number& Nu);
    
    
    int operator <=> (const Int_Number& Nu) const;
    
    bool operator >(const Int_Number& Nu) const{return (*this <=> Nu) == 1 ? true : false;}
    bool operator <(const Int_Number& Nu) const{return (*this <=> Nu) == -1 ? true : false;}
    bool operator >= (const Int_Number& Nu) const{return (*this <=> Nu) == -1 ? false : true;}
    bool operator <= (const Int_Number& Nu) const{return (*this <=> Nu) == 1 ? false : true;}
    bool operator == (const Int_Number& Nu) const{return (*this <=> Nu) == 0 ? true : false;}
    bool operator != (const Int_Number& Nu) const{return (*this <=> Nu) == 0 ? false : true;}
    
    
    Int_Number& operator - ();
    void operator += (const Int_Number& Nu);
    void operator -= (const Int_Number& Nu);
    Int_Number operator +(const Int_Number& Nu) const;
    Int_Number operator -(const Int_Number& Nu) const;
    
    
    Int_Number& operator << (int i);
    Int_Number& operator >> (int i);
    
    
    void operator *= (int q);
    void operator *= (const Int_Number& Nu);
    Int_Number operator * (int q) const;
    Int_Number operator * (const Int_Number& Nu) const;
    
    
    void operator /= (const Int_Number& Nu);
    void operator %= (const Int_Number& Nu);
    Int_Number operator / (const Int_Number& Nu) const;
    Int_Number operator % (const Int_Number& Nu) const;
    
    Quotient_Number operator | (const Int_Number& Nu1) const;

    friend Int_Number NOD(const Int_Number& Nu1, const Int_Number& Nu2);

    
    friend std::ostream& operator << (std::ostream& os, const Int_Number& Nu);
    friend std::ofstream& operator << (std::ofstream& of, const Int_Number& Nu);

    friend std::istream& operator >> (std::istream& is, Int_Number& Nu);
    friend std::ifstream& operator >> (std::ifstream& is, Int_Number& Nu);
    
    
    ~Int_Number();
};


