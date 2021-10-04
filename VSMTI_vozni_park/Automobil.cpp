#include "Automobil.h"
#include "Vozilo.h"

#include <string>



using namespace std;

Automobil::Automobil(string tip, int id, string n, string dReg) : Vozilo(tip ,id, n, dReg)
{
	
};

Automobil::~Automobil()
{
};
