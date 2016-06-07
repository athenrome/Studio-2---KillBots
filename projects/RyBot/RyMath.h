#pragma once

#include <stdlib.h> 

#include <iostream>

int RandomRange(int min, int max)
{
	int newNum = rand() % (max - min) + min + 1;
	std::cout << newNum << std::endl;
	return newNum;
}