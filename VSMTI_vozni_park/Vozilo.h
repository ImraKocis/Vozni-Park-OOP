#pragma once
#include <string>

using namespace std;

class Vozilo 
{
public:
	Vozilo( string tip, int id, string dReg, string n);
	~Vozilo();
	string GetName() { return m_sNaziv; }
	string GetDate() { return m_sDatumRegistracje; }
	int GetID() { return (m_nId); }
	string m_sTip;	
	int m_nId;
	string m_sDatumRegistracje;
	string m_sNaziv;

	
};

