#pragma once
#include "Osoba.h"
#include "Vozilo.h"
#include <string>
class Zaposlenik : public Osoba
{
public:
	Zaposlenik(string o, string sIme, string sPrezime, int sId, Vozilo *oVozilo);
	~Zaposlenik();
	string m_sOib;
	int m_sId;
	Vozilo *m_oVozilo;
};

