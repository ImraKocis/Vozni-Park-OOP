#include "Radni_stroj.h"
#include "Vozilo.h"
#include "stdafx.h"
#include <iostream>
#include <string>

using namespace std;

Radni_stroj::Radni_stroj(string tip,int id, string n, string dReg) : Vozilo(tip ,id, n , dReg)
{
	
};

Radni_stroj::~Radni_stroj()
{
};