#pragma once
#include <string>
using namespace std;
class Osoba
{
public:
	Osoba();
	Osoba(string sIme, string sPrezime);
	virtual ~Osoba();
	string m_sIme;
	string m_sPrezime;
};

