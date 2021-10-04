#include "Zaposlenik.h"
#include <string>
#include "Vozilo.h"

using namespace std;

Zaposlenik::Zaposlenik(string o, string sIme, string sPrezime, int sId,  Vozilo *oVozilo) : Osoba(sIme, sPrezime)
{
	m_sOib = o;
	m_sIme = sIme;
	m_sPrezime = sPrezime;
	m_sId = sId;
	if (oVozilo != nullptr)
	{
		*m_oVozilo = *oVozilo;
	}
};

Zaposlenik::~Zaposlenik()
{
};
