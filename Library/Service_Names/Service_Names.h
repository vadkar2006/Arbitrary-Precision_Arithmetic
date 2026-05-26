#pragma once

#include <iostream>
#include <cmath>

#include "Library/Error/Error.h"
#include "Library/Thread_Pool/Thread_Pool.h"


#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define UNDERLINE   "\033[4m"
#define INVERT      "\033[7m"


#define FG_BLACK    "\033[30m"
#define FG_RED      "\033[31m"
#define FG_GREEN    "\033[32m"
#define FG_YELLOW   "\033[33m"
#define FG_BLUE     "\033[34m"
#define FG_MAGENTA  "\033[35m"
#define FG_CYAN     "\033[36m"
#define FG_WHITE    "\033[37m"


#define FG_BRIGHT_BLACK    "\033[90m" 
#define FG_BRIGHT_RED      "\033[91m"
#define FG_BRIGHT_GREEN    "\033[92m"
#define FG_BRIGHT_YELLOW   "\033[93m"
#define FG_BRIGHT_BLUE     "\033[94m"
#define FG_BRIGHT_MAGENTA  "\033[95m"
#define FG_BRIGHT_CYAN     "\033[96m"
#define FG_BRIGHT_WHITE    "\033[97m"


#define BG_BLACK    "\033[40m"
#define BG_RED      "\033[41m"
#define BG_GREEN    "\033[42m"
#define BG_YELLOW   "\033[43m"
#define BG_BLUE     "\033[44m"
#define BG_MAGENTA  "\033[45m"
#define BG_CYAN     "\033[46m"
#define BG_WHITE    "\033[47m"


#define BG_BRIGHT_BLACK    "\033[100m"
#define BG_BRIGHT_RED      "\033[101m"
#define BG_BRIGHT_GREEN    "\033[102m"
#define BG_BRIGHT_YELLOW   "\033[103m"
#define BG_BRIGHT_BLUE     "\033[104m"
#define BG_BRIGHT_MAGENTA  "\033[105m"
#define BG_BRIGHT_CYAN     "\033[106m"
#define BG_BRIGHT_WHITE    "\033[107m"







class Error;
class Int_Number;
class Quotient_Number;



template<typename T>
int Length_Int(T num) {
    if (num == 0) return 1;
    
    T n = num < 0 ? -num : num;
    
    if (n < 10) return 1;
    if (n < 100) return 2;
    if (n < 1000) return 3;
    if (n < 10000) return 4;
    
    return (long)log10(n) + 1;
}







struct Mod998244353 {
    static constexpr int MOD = 998244353;
    static constexpr int ROOT = 3;
    static constexpr int MAX_LOG = 23;
};

struct Mod1004535809 {
    static constexpr int MOD = 1004535809;
    static constexpr int ROOT = 3;
    static constexpr int MAX_LOG = 21;
};

struct Mod469762049 {
    static constexpr int MOD = 469762049;
    static constexpr int ROOT = 3;
    static constexpr int MAX_LOG = 26;
};

struct Mod167772161 {
    static constexpr int MOD = 167772161;
    static constexpr int ROOT = 3;
    static constexpr int MAX_LOG = 25;
};

struct Mod104857601 {
    static constexpr int MOD = 104857601;
    static constexpr int ROOT = 3;
    static constexpr int MAX_LOG = 22;
};







template<class MOD_, int POWER_TWO>
class NTT {
    static constexpr int MAX_N = 1 << MOD_::MAX_LOG;

    static std::vector<int> roots;
    static std::vector<int> roots_inv;
    static std::vector<int> rev;

    static bool initialized;

    static std::mutex init_mutex; 

    
    static void init_impl() {
        if (initialized && (roots.size() >= (1 << POWER_TWO)) && (rev.size() == (1 << POWER_TWO))) 
             return;
        
        std::lock_guard<std::mutex> lock(init_mutex);

        
        if (initialized && (roots.size() >= (1 << POWER_TWO)) && (rev.size() == (1 << POWER_TWO))) 
             return;
        
        int LEN_ARRAY = 1 << POWER_TWO;
        
        roots.resize(LEN_ARRAY);
        roots_inv.resize(LEN_ARRAY);
        rev.resize(LEN_ARRAY);


        for (int i = 0; i < LEN_ARRAY; ++i)
            rev[i] = (rev[i >> 1] >> 1) | ((i & 1) ? (LEN_ARRAY >> 1) : 0);
        
        
        int base = modpow(MOD_::ROOT, (MOD_::MOD - 1) / LEN_ARRAY, MOD_::MOD);
        int base_inv = modpow(base, MOD_::MOD - 2, MOD_::MOD);

        roots[0] = 1;
        roots_inv[0] = 1;
        for (int i = 1; i < LEN_ARRAY; ++i) {
            roots[i] = (1LL * roots[i-1] * base) % MOD_::MOD;
            roots_inv[i] = (1LL * roots_inv[i-1] * base_inv) % MOD_::MOD;
        }

        initialized = true;
    }

public:
    
    static void ensure_initialized(){init_impl();}

    static int modpow(int a, int n, int mod) {
        int res = 1;
        while (n) {
            if (n & 1) res = (1LL * res * a) % mod;
            a = (1LL * a * a) % mod;
            n >>= 1;
        }
        return res;
    }

    static void transform(std::vector<int>& a, bool inverse) {
        ensure_initialized();


        if ((1 << POWER_TWO) != static_cast<int>(a.size()))
            throw Error_Size_Array();
        

        int n = 1 << POWER_TWO;
        
        for (int i = 0; i < n; i++) {
            int j = rev[i];
            if (i < j) 
                std::swap(a[i], a[j]);
        }

        const std::vector<int>& w = inverse ? roots_inv : roots;

        for (int len = 2; len <= n; len <<= 1) {
            int step = n / len;
            for (int i = 0; i < n; i += len) {
                for (int j = 0; j < len/2; j++) {
                    int u = a[i + j];
                    int v = (1LL * a[i + j + len/2] * w[j * step]) % MOD_::MOD;

                    a[i + j] = u + v;
                    if (a[i + j] >= MOD_::MOD) a[i + j] -= MOD_::MOD;

                    a[i + j + len/2] = u - v;
                    if (a[i + j + len/2] < 0) a[i + j + len/2] += MOD_::MOD;
                }
            }
        }

        if (inverse) {
            int n_inv = modpow(n, MOD_::MOD - 2, MOD_::MOD);
            for (int& x : a) x = (1LL * x * n_inv) % MOD_::MOD;
        }
    }
};


template<class MOD_, int LEN_ARRAY>
std::vector<int> NTT<MOD_, LEN_ARRAY>::roots;

template<class MOD_, int LEN_ARRAY>
std::vector<int> NTT<MOD_, LEN_ARRAY>::roots_inv;

template<class MOD_, int LEN_ARRAY>
std::vector<int> NTT<MOD_, LEN_ARRAY>::rev;

template<class MOD_, int LEN_ARRAY>
bool NTT<MOD_, LEN_ARRAY>::initialized = false;

template<class MOD_, int LEN_ARRAY>
std::mutex NTT<MOD_, LEN_ARRAY>::init_mutex;







template<class Mod, template<class Mod_1, int Power_2> class Func>
void DispatchNTT(int power_2, std::vector<int>& Nu1, std::vector<int>& Nu2) {
    switch (power_2) {
        case 1:  Func<Mod, 1>::operator()(Nu1, Nu2); break;
        case 2:  Func<Mod, 2>::operator()(Nu1, Nu2); break;
        case 3:  Func<Mod, 3>::operator()(Nu1, Nu2); break;
        case 4:  Func<Mod, 4>::operator()(Nu1, Nu2); break;
        case 5:  Func<Mod, 5>::operator()(Nu1, Nu2); break;
        case 6:  Func<Mod, 6>::operator()(Nu1, Nu2); break;
        case 7:  Func<Mod, 7>::operator()(Nu1, Nu2); break;
        case 8:  Func<Mod, 8>::operator()(Nu1, Nu2); break;
        case 9:  Func<Mod, 9>::operator()(Nu1, Nu2); break;
        case 10: Func<Mod, 10>::operator()(Nu1, Nu2); break;
        case 11: Func<Mod, 11>::operator()(Nu1, Nu2); break;
        case 12: Func<Mod, 12>::operator()(Nu1, Nu2); break;
        case 13: Func<Mod, 13>::operator()(Nu1, Nu2); break;
        case 14: Func<Mod, 14>::operator()(Nu1, Nu2); break;
        case 15: Func<Mod, 15>::operator()(Nu1, Nu2); break;
        case 16: Func<Mod, 16>::operator()(Nu1, Nu2); break;
        case 17: Func<Mod, 17>::operator()(Nu1, Nu2); break;
        case 18: Func<Mod, 18>::operator()(Nu1, Nu2); break;
        case 19: Func<Mod, 19>::operator()(Nu1, Nu2); break;
        case 20: Func<Mod, 20>::operator()(Nu1, Nu2); break;
        case 21: Func<Mod, 21>::operator()(Nu1, Nu2); break;
        case 22: Func<Mod, 22>::operator()(Nu1, Nu2); break;
        case 23: Func<Mod, 23>::operator()(Nu1, Nu2); break;
        case 24: Func<Mod, 24>::operator()(Nu1, Nu2); break;
        case 25: Func<Mod, 25>::operator()(Nu1, Nu2); break;
        case 26: Func<Mod, 26>::operator()(Nu1, Nu2); break;
        case 27: Func<Mod, 27>::operator()(Nu1, Nu2); break;
        case 28: Func<Mod, 28>::operator()(Nu1, Nu2); break;

        default: 
            throw Error_Too_High_Degree();
    }
}






template<class Mod, int Power_2>
struct PerformNTTMultiplication{
    static void operator() (std::vector<int>& Nu1, std::vector<int>& Nu2) {
        NTT<Mod, Power_2>::ensure_initialized();
        
        std::thread thread1(NTT<Mod, Power_2>::transform, std::ref(Nu1), false);
        std::thread thread2(NTT<Mod, Power_2>::transform, std::ref(Nu2), false);

        thread1.join();
        thread2.join();

        for (int i = 0; i < (1 << Power_2); i++) 
            Nu1[i] = (1LL * Nu1[i] * Nu2[i]) % Mod::MOD;
        
        NTT<Mod, Power_2>::transform(Nu1, true);
    }
};









