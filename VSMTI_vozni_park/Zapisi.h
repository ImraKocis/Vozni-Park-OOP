#pragma once
#include <string>

using namespace std;
class Zapisi
{
public:
	Zapisi(int nIdVozila, int nIdZaposlenika, string sAkcija);
	~Zapisi();
	int m_sIdVozila;
	int m_sIdZaposlenika;
	string m_sAkcija;
	

};

