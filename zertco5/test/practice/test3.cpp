/*
 * test3.cpp
 *
 *  Created on: 2014��12��26��
 *      Author: Administrator
 */

#include <cstdio>

void f1() {}

void f2() {
	return f1();
}

int main() {
	return 0;
}

