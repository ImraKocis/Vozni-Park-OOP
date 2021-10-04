#include "Zapisi.h"
#include <string>

using namespace std;

Zapisi::Zapisi(int nIdVozila, int nIdZaposlenika, string sAkcija)
{
	m_sIdVozila = nIdVozila;
	m_sIdZaposlenika = nIdZaposlenika;
	m_sAkcija = sAkcija;
	
};

Zapisi::~Zapisi()
{};
