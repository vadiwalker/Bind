#include "bind.h"
#include <iostream>
#include <functional>

BIND::placeholder<0> __1;
BIND::placeholder<1> __2;
BIND::placeholder<2> __3;
BIND::placeholder<3> __4;

using namespace std::placeholders;

int sum(int a, int b) {
	return a + b;
}

std::string concat(std::string a, std::string b, std::string c) {
	return a + b + c;
}

void EXPECT_TRUE(bool val) {
	if (!val) {
		std::cerr << "Mistake\n";
	}
}

void test1() {
	auto x = BIND::bind(concat, "123", "123", "90");
	auto y = std::bind(concat, "123", "123", "90");

	auto a = x();
	auto b = y();

	EXPECT_TRUE(a == b);
}

void test2() {
	auto x = BIND::bind(sum, __2, __2);
	auto y = std::bind(sum, _2, _2);
	EXPECT_TRUE(x(11, 16) == y(11, 16));
}

void test3() {
	auto x = BIND::bind(concat, __1, "3131", __1);
	auto y = std::bind(concat, _1, "3131", _1);
	EXPECT_TRUE(x("test") == y("test"));
}

void test4() {
	auto x = std::bind(sum, _1, _2);
	auto y = std::bind(sum, _2, _2);
	auto z = std::bind(sum, x, y);

	auto xx = BIND::bind(sum, __1, __2);
	auto yy = BIND::bind(sum, __2, __2);
	auto zz = BIND::bind(sum, xx, yy);

	EXPECT_TRUE(zz(13, 14) == z(13, 14));
	
}

int main() {
	

	test1();
	test2();
	test3();
	test4();
	std::cout << "ALL DONE, CONGRATS!\n";
	return 0;
}
