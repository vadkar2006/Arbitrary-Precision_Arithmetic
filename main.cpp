#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <cassert>

#include "Library/Quotient_Number/Quotient_Number.h"



// Тест 1: Конструкторы и базовый вывод
void test_constructors_and_output() {
    std::cout << "[TEST 1] Constructors & output" << std::endl;
    
    Int_Number a;                // 0 по умолчанию
    Int_Number b(123456789);
    Int_Number c(-9876543210LL);
    Int_Number d("12345678901234567890");
    
    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
    std::cout << "c = " << c << std::endl;
    std::cout << "d = " << d << std::endl;

    
    assert(a.To_Clear_String() == "+0");
    assert(b.To_Clear_String() == "+123456789");
    assert(c.To_Clear_String() == "-9876543210");
    assert(d.To_Clear_String() == "+12345678901234567890");
    
    std::cout << " PASSED\n" << std::endl;
}

// Тест 2: Сравнения
void test_comparisons() {
    std::cout << "[TEST 2] Comparisons" << std::endl;
    
    Int_Number a(100);
    Int_Number b(200);
    Int_Number c(100);
    
    assert(a < b);
    assert(b > a);
    assert(a == c);
    assert(a != b);
    assert(a <= c);
    assert(b >= a);
    
    std::cout << " PASSED\n" << std::endl;
}

// Тест 3: Сложение и вычитание
void test_addition_subtraction() {
    std::cout << "[TEST 3] Addition & Subtraction" << std::endl;
    
    Int_Number a(123456789);
    Int_Number b(987654321);
    
    Int_Number c = a + b;
    assert(c.To_Clear_String() == "+1111111110");
    
    Int_Number d = b - a;
    assert(d.To_Clear_String() == "+864197532");
    
    Int_Number e = a - b;
    assert(e.To_Clear_String() == "-864197532");
    
    a += b;
    assert(a.To_Clear_String() == "+1111111110");
    
    std::cout << " PASSED\n" << std::endl;
}

// Тест 4: Умножение (в том числе NTT)
void test_multiplication() {
    std::cout << "[TEST 4] Multiplication" << std::endl;
    
    Int_Number a("123456789");
    Int_Number b("987654321");
    
    Int_Number c = a * b;
    // 123456789 * 987654321 = 121932631112635269
    assert(c.To_Clear_String() == "+121932631112635269");
    
    Int_Number d = a * 1000000;
    assert(d.To_Clear_String() == "+123456789000000");
    
    a *= b;
    assert(a.To_Clear_String() == "+121932631112635269");
    
    std::cout << " PASSED\n" << std::endl;
}

// Тест 5: Деление и остаток
void test_division_modulo() {
    std::cout << "[TEST 5] Division & Modulo" << std::endl;
    
    Int_Number a("1__000_000_000__000_000_000");
    Int_Number b("123456789");
    
    Int_Number q = a / b;
    Int_Number r = a % b;
    


    assert(q.To_Clear_String() == "+8100000073");
    assert(r.To_Clear_String() == "+87654403");
    
    Int_Number zero(0);
    bool exception_caught = false;
    try {
        Int_Number res = a / zero;
    } catch (...) {
        exception_caught = true;
    }
    assert(exception_caught);
    
    std::cout << " PASSED\n" << std::endl;
}

// Тест 6: Побитовые сдвиги (умножение/деление на 10^9)
void test_shifts() {
    std::cout << "[TEST 6] Shifts (multiply/divide by 10^9)" << std::endl;
    
    Int_Number a("12345678901234567890");
    
    // << 1 умножает на основание системы (10^9)
    Int_Number b = a << 1;
    // Ожидается: 12345678901234567890 * 1e9 = 12345678901234567890000000000
    assert(b.To_Clear_String() == "+12345678901234567890000000000");
    
    // >> 1 делит на основание системы
    Int_Number c = a >> 2;
    // 12345678901234567890 / 1e9 = 12345678901 (остаток отбрасывается)
    assert(c.To_Clear_String() == "+12345678901");
    
    // Сдвиг на 0
    Int_Number d = a >> 0;
    assert(d == a);
    

    
    std::cout << " PASSED\n" << std::endl;
}

// Тест 7: Класс Quotient_Number (рациональные числа)
void test_quotient_number() {
    std::cout << "[TEST 7] Quotient_Number (rational)" << std::endl;
    
    Quotient_Number::Changing_Length_Fractional_Output(10);
    
    Int_Number up(1);
    Int_Number down(3);
    Quotient_Number q1(up, down);
    
    std::cout << "1/3 = " << q1 << std::endl;
    
    Quotient_Number q2(2, 4);   // сократится до 1/2
    std::cout << "2/4 = " << q2 << std::endl;
    
    Quotient_Number q3(5);
    Quotient_Number q4(2);
    
    Quotient_Number sum = q3 + q4;
    Quotient_Number diff = q3 - q4;
    Quotient_Number prod = q3 * q4;
    Quotient_Number quot = q3 / q4;
    
    std::cout << "5 + 2 = " << sum << std::endl;
    std::cout << "5 - 2 = " << diff << std::endl;
    std::cout << "5 * 2 = " << prod << std::endl;
    std::cout << "5 / 2 = " << quot << std::endl;
    
    // Сравнения
    Quotient_Number q5(1, 3);
    Quotient_Number q6(2, 6);
    assert(q5 == q6);
    
    Quotient_Number q7(1, 2);
    assert(q5 < q7);
    
    std::cout << " PASSED\n" << std::endl;
}

// Тест 8: Длинные числа с переносами
void test_carry_propagation() {
    std::cout << "[TEST 8] Carry propagation" << std::endl;
    
    // 999999999 + 1 = 1000000000
    Int_Number a("999999999");
    Int_Number b(1);
    Int_Number c = a + b;
    assert(c.To_Clear_String() == "+1000000000");
    
    // 1000000000 - 1 = 999999999
    Int_Number d(1000000000);
    Int_Number e = d - b;
    assert(e.To_Clear_String() == "+999999999");
    
    // 1000000000 * 1000000000 = 10^18
    Int_Number f(1000000000);
    Int_Number g = f * f;
    assert(g.To_Clear_String() == "+1000000000000000000");
    
    std::cout << " PASSED\n" << std::endl;
}

// Тест 9: Отрицательные числа
void test_negative_numbers() {
    std::cout << "[TEST 9] Negative numbers" << std::endl;
    
    Int_Number a(-123);
    Int_Number b(456);
    
    Int_Number c = a + b;      // 333
    assert(c.To_Clear_String() == "+333");
    
    Int_Number d = a - b;      // -579
    assert(d.To_Clear_String() == "-579");
    
    Int_Number e = -a;         // 123
    assert(e.To_Clear_String() == "+123");
    
    Int_Number f = a * b;      // 56088
    assert(f.To_Clear_String() == "+56088");
    
    Int_Number g(100);
    Int_Number h(-3);
    Int_Number i = g / h;      // -33
    assert(i.To_Clear_String() == "-33");
    
    Int_Number j = g % h;      // 1 (100 % -3 = -1)
    assert(j.To_Clear_String() == "-1");
    
    std::cout << " PASSED\n" << std::endl;
}

// Тест 10: Оператор | (Quotient_Number из деления)
void test_quotient_operator() {
    std::cout << "[TEST 10] Operator | (quotient)" << std::endl;
    
    Int_Number a(10);
    Int_Number b(3);
    
    Quotient_Number q = a | b;   // должно вернуть 10/3
    
    std::cout << "10 | 3 = " << q << std::endl;
    
    // Проверка приведения к несократимой дроби
    Int_Number c(6);
    Int_Number d(9);
    Quotient_Number q2 = c | d;   // должно быть 2/3
    
    std::cout << "6 | 9 = " << q2 << std::endl;
    
    std::cout << " PASSED\n" << std::endl;
}

int main() {
    std::cout << "========== STARTING TESTS ==========\n" << std::endl;
    
    try {
        test_constructors_and_output();
        test_comparisons();
        test_addition_subtraction();
        test_multiplication();
        test_division_modulo();
        test_shifts();
        test_quotient_number();
        test_carry_propagation();
        test_negative_numbers();
        test_quotient_operator();
        
        std::cout << "\n========== ALL TESTS PASSED ==========" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "\n[EXCEPTION] " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}







