#pragma once
#include "Vozilo.h"
#include <string>


using namespace std;

class Radni_stroj :
	public Vozilo
{
public:
	Radni_stroj(string tip, int id, string dReg, string n);
	~Radni_stroj();

};

