#pragma once
#include "Vozilo.h"
#include "Zaposlenik.h"
#include "Zapisi.h"
#include <string>
#include <vector>

using namespace std;
class Main
{
public:
	Main();
	~Main();

	vector<Vozilo*> vVozilo;
	vector<Zaposlenik*> vZaposlenik;
	vector<Zapisi*> vZapisi;

	Zaposlenik* PronadiZaposlneikPoId(int nId);
	Vozilo* PronadiVoziloPoId(int nId);

	bool CitanjeXMLVozila();
	bool CitanjeXMLZaposlenici();
	bool CitanjeXMLZapisi();
	bool CitanjeZapisa();
	void DodavanjeVozila();
	void BrisanjeVozila();
	void ObrisiXML(string odabir);
	void AzurirajXML(string odabir2, string nazivNew, string datumRegNew);
	void AzurirajVozilo();
	void ListaVozila();
	void Izbornik1();
	int POS_INT_TRANS(char* system_number);
	bool Switch_ERROR_message_ALERT(int d, int m, int y, string datum);
	bool ProvjeriIntegerUnos(string unos);
	int Get_The_Number_of_Days(int godina1, int godina2, int dan1, int dan2, int mjesec1, int mjesec2);
	void PisiXMLZapis(int tip, string ZapID, string VozID, string Datum);
	string Date();
	void RegistriranjeVozila();
	void ZaduziVozilo();
	void VratiVozilo();
	void BrojZaduzivanjaPoZaposleniku();
	void ProsjekVozila();
	void Izbornik2();
	void GlavniIzbornik();
	void Povratak();
	
};
