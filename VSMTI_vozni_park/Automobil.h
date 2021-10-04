#pragma once
#include "Vozilo.h"
#include <string>

using namespace std;

class Automobil :
	public Vozilo
{
public:
	Automobil(string tip,int id, string dReg, string n);
	~Automobil();	
};

