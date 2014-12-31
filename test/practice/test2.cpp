/*
 * test2.cpp
 *
 *  Created on: 2014Äê4ÔÂ30ÈÕ
 *      Author: Administrator
 */
#include <cstdio>

struct INT
{
	int							value;
	operator int() {return value;}
};

int main() {
	INT i;
	int i2 = i;
	return 0;
}

