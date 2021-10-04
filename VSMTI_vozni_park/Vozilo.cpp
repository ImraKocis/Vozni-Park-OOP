#include "Vozilo.h"
//#include "Zaposlenik.h"
//#include "Osoba.h"
#include <string>

using namespace std;

Vozilo::Vozilo(string tip ,int id, string n, string dReg/*, Zaposlenik *oZaposlenik*/)
{
	m_sTip = tip;
	m_nId = id;
	m_sNaziv = n;
	m_sDatumRegistracje = dReg;
	/*if (oZaposlenik != nullptr)
	{
		*m_oZaposlenik = *oZaposlenik;
	}
	*/
};

Vozilo::~Vozilo()
{
};
