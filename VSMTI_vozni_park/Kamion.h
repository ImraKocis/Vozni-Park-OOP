#pragma once
#include "Vozilo.h"
#include <string>

using namespace std;
class Kamion :
	public Vozilo
{
public:
	Kamion(string tip, int id, string dReg, string n);
	~Kamion();

};

