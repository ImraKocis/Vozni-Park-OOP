#pragma once
#include "Vozilo.h"
#include <string>

using namespace std;
class Motocikl :
	public Vozilo
{
public:
	Motocikl(string tip, int id, string dReg, string n);
	~Motocikl();

};

