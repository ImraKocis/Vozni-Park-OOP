#include "Main.h"
#include <string>
#include <iostream>
#include "tinyxml2.h"
#include "stdafx.h"
#include "Osoba.h"
#include "Automobil.h"
#include "Kamion.h"
#include "Motocikl.h"
#include "Radni_stroj.h"
#include "Vozilo.h"
#include "Zaposlenik.h"
#include "Zapisi.h"
#include <vector>
#include <conio.h>
#include <stdlib.h>
#include <chrono>
#include <ctime>  
#include "TextTable.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cstdlib>

using namespace std;
using namespace tinyxml2;
using std::chrono::system_clock;

Main::Main()
{	
};
Main::~Main()
{
};

vector <int> vID;
vector <int> vZapisZapId;
vector <int> vZapisVozId;
vector <int> vBrojKoristenja;
vector <int> vIDZap;
vector <string> vReg;

bool Main::CitanjeXMLVozila()
{
	vVozilo.clear();
	vID.clear();
	vReg.clear();
	XMLDocument oXmlDocument;
	XMLError oResult = oXmlDocument.LoadFile("vozila.xml");
	if (oResult != XML_SUCCESS)
	{
		cout << "Greška u XML datoteci!" << endl;
		return false;
	}
	XMLNode* oRoot = oXmlDocument.FirstChildElement("dataset");
	if (oRoot == nullptr) return false;
	XMLElement* oVozila = oRoot->FirstChildElement("Vozila");
	if (oVozila == nullptr) return false;
	for (XMLElement* pChildRoot = oVozila->FirstChildElement("Vozilo"); pChildRoot != NULL; pChildRoot = pChildRoot->NextSiblingElement())
	{
		string sTip = (pChildRoot->Attribute("tip"));
		int nID = (stoi(pChildRoot->Attribute("id")));
		string sNaziv = (pChildRoot->Attribute("naziv"));
		string sDatReg = (pChildRoot->Attribute("datum_reg"));
		this->vVozilo.push_back(new Vozilo(sTip, nID, sNaziv, sDatReg));
		vID.push_back(nID);
		vReg.push_back(sDatReg);
	}
	return true;
}

bool Main::CitanjeXMLZaposlenici()
{
	vZaposlenik.clear();
	vIDZap.clear();
	XMLDocument oXmlDocument;
	XMLError oResult = oXmlDocument.LoadFile("zaposlenici.xml");
	if (oResult != XML_SUCCESS)
	{
		cout << "Greška u XML datoteci!" << endl;
		return false;
	}
	XMLNode* oRoot = oXmlDocument.FirstChildElement("dataset");
	if (oRoot == nullptr) return false;
	XMLElement* oZaposlenik = oRoot->FirstChildElement("Zaposlenici");
	if (oZaposlenik == nullptr) return false;
	for (XMLElement* pChild = oZaposlenik->FirstChildElement("Zaposlenik"); pChild != NULL; pChild = pChild->NextSiblingElement())
	{
		string sIme = (pChild->Attribute("ime"));
		string sPrezime = (pChild->Attribute("prezime"));
		string sOIB = (pChild->Attribute("oib"));
		int sId = (stoi(pChild->Attribute("id")));
		vZaposlenik.push_back(new Zaposlenik(sIme, sPrezime, sOIB, sId, nullptr));
		vIDZap.push_back(sId);
	}
	return true;
}

bool Main::CitanjeXMLZapisi()
{
	vBrojKoristenja.clear();
	vZapisVozId.clear();
	XMLDocument oDoc;
	XMLError oResult = oDoc.LoadFile("zapis.xml");
	if (oResult != tinyxml2::XML_SUCCESS) return false;
	XMLNode* oRoot = oDoc.FirstChildElement("dataset");
	if (oRoot == nullptr) return false;
	XMLElement* oZapis = oRoot->FirstChildElement("Zapis");
	if (oZapis == nullptr) return false;
	for (XMLElement* oElem = oZapis->FirstChildElement("Zaduzivanje"); oElem != NULL; oElem = oElem->NextSiblingElement())
	{
		
		int nZaposlenikId = atoi(oElem->Attribute("zaposlenik_id"));
		int nVoziloId = atoi(oElem->Attribute("vozilo_id"));
		Vozilo* oVozilo = this->PronadiVoziloPoId(nVoziloId);
		Zaposlenik* oZaposlneik = this->PronadiZaposlneikPoId(nZaposlenikId);
		if (oVozilo != nullptr && oZaposlneik != nullptr)
		{
			oZaposlneik->m_oVozilo = oVozilo;
		}
		vZapisVozId.push_back(nVoziloId);
		vZapisZapId.push_back(nZaposlenikId);
	}
	return true;
}

bool Main::CitanjeZapisa()
{
	vZapisi.clear();
	XMLDocument oDoc;
	XMLError oResult = oDoc.LoadFile("zapis.xml");
	if (oResult != tinyxml2::XML_SUCCESS) return false;
	XMLNode* oRoot = oDoc.FirstChildElement("dataset");
	if (oRoot == nullptr) return false;
	XMLElement* oZapis = oRoot->FirstChildElement("Zapis_Sve");
	if (oZapis == nullptr) return false;
	for (XMLElement* oElem = oZapis->FirstChildElement("Akcije"); oElem != NULL; oElem = oElem->NextSiblingElement())
	{
		int nIdVozila = atoi(oElem->Attribute("vozilo_id"));
		int nIdZaposlenik = atoi(oElem->Attribute("zaposlenik_id"));
		string sAkcija = (oElem->Attribute("tip_akcije"));
					
		vZapisi.push_back(new Zapisi(nIdVozila, nIdZaposlenik, sAkcija));
		//vZapisVozId.push_back(nIdVozila);//ID-jevi vozila koja su se zaduzivala
		
	}
	return true;
}



Zaposlenik* Main::PronadiZaposlneikPoId(int nId)
{
	Zaposlenik* oZaposlneik = nullptr;
	for (int i = 0; i < vZaposlenik.size(); i++)
	{
		if (vZaposlenik[i]->m_sId == nId)
		{
			oZaposlneik = vZaposlenik[i];
		}
	}
	return oZaposlneik;
}

Vozilo* Main::PronadiVoziloPoId(int nId)
{
	Vozilo* oVozilo = nullptr;
	for (int i = 0; i < vVozilo.size(); i++)
	{
		if (vVozilo[i]->m_nId == nId)
		{
			oVozilo = vVozilo[i];
		}
	}
	return oVozilo;
}
void Main::PisiXMLZapis(int tip ,string ZapID, string VozID, string Datum)
{
	XMLDocument oXmlDocument;
	oXmlDocument.LoadFile("zapis.xml");
	XMLElement* pRoot = oXmlDocument.FirstChildElement("dataset");
	XMLElement* pElement = pRoot->FirstChildElement("Zapis_Sve");
	XMLElement* pRootElement = oXmlDocument.NewElement("Akcije");
	oXmlDocument.InsertFirstChild(pRootElement);
	if (tip == 1)
	{
		pRootElement->SetAttribute("tip_akcije", "Zaduzivanje");
	}
	else if (tip == 2)
	{
		pRootElement->SetAttribute("tip_akcije", "Vracanje");
	}
	pRootElement->SetAttribute("zaposlenik_id", ZapID.c_str());
	pRootElement->SetAttribute("zaposlenik_id", ZapID.c_str());
	pRootElement->SetAttribute("vozilo_id", VozID.c_str());
	pRootElement->SetAttribute("datum", Datum.c_str());
	pElement->InsertEndChild(pRootElement);
	oXmlDocument.SaveFile("zapis.xml");
}
bool Main::ProvjeriIntegerUnos(string unos)
{
	int is_num = 0,
		decimal_count = 0,
		user_converted_num;

	bool is_num_bool = false;

	if (unos[0] == '-')
	{
		is_num++;
	}

	if (unos[0] == '0' && isdigit(unos[1]))
	{
		is_num = 0;
	}
	else
	{
		for (int i = 0; i < unos.size(); i++)
		{
			if (isdigit(unos[i]))
			{
				is_num++;
			}
			if (unos[i] == '.')
			{
				decimal_count++;
			}
		}
	}

	if (decimal_count == 1)
	{
		is_num = 0;
	}

	if (is_num == unos.size())
	{
		stringstream str_stream_object(unos);
		str_stream_object >> user_converted_num;

		is_num_bool = true;

		return is_num_bool;
	}
	else
	{
		cout << "Ovaj unos prima samo brojeve!!" << endl;

		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		is_num_bool = false;
		is_num = 0;
		decimal_count = 0;
		return is_num_bool;
	}
}


void Main::DodavanjeVozila()
{
	system("CLS");
	CitanjeXMLVozila();
	string novoVozilo = "";
	string uneseniDatum = "", dan = "", mjesec = "", godina = "", dan1 = "", mjesec1 = "", godina1 = "";
	int i_dan, i_mjesec, i_godina, i_ID;
	int provjera = 0;
	string max = "";
	bool is_num_bool = false;
	bool x = 1;
	bool Sc = 0;
	cout << "***DODAVANJE VOZILA***";
	cout << "\nOdaberite tip vozila.\n\n1. Automobil\n\n2. Kamion\n\n3. Motocikl\n\n4. Radni Stroj" << endl;
	cout << "\nUnesite redni broj zeljene akcije:\n";
	int izbor = _getch();
	while (izbor != 49 && izbor != 50 && izbor != 51 && izbor != 52)
	{
		cout << "Ponovite izbor" << endl;
		izbor = _getch();
	}	
	switch (izbor)
	{
		case 49:
		{
			system("CLS");
			cout << "\nNaziv automobila: " << endl;
			getline(cin, novoVozilo);

			do
			{	
				cin.ignore(4096, '\n');
				system("CLS");
				cout << "ID: ";			
				cin>>max;
				is_num_bool = ProvjeriIntegerUnos(max);
				if (is_num_bool == true)
				{
					if (count(vID.begin(), vID.end(), stoi(max)))
					{
						cout << "\nUneseni ID je vec zauzet!!";
						is_num_bool = false;
					}
				}				
			} while (is_num_bool == false);
			do {
				provjera = 0;
				system("CLS");
				cout << "\n\nOdaberite novi datum registracije u obliku DD-MM-YYYY" << endl;
				cin.ignore(4096, '\n');
				getline(cin, uneseniDatum);
				if (uneseniDatum.size() < 10 || uneseniDatum.size() > 10)
				{
					cout << "Krivi format datuma" << endl;
					provjera = 1;
				}
				else
				{
					for (int i = 0; i < 10; i++)
					{
						if (i >= 0 && i < 2)
						{
							dan += uneseniDatum[i];
						}
						if (i >= 3 && i < 5)
						{
							mjesec += uneseniDatum[i];
						}
						if (i > 5)
						{
							godina += uneseniDatum[i];
						}
					}
					try
					{
						i_dan = stoi(dan);
						i_mjesec = stoi(mjesec);
						i_godina = stoi(godina);
					}
					catch (const invalid_argument& e)
					{
						cout << "Krivi format datuma" << endl;
						return;
					}
					Sc = Switch_ERROR_message_ALERT(i_dan, i_mjesec, i_godina, uneseniDatum);
					system("pause");
				}
			} while (!Sc);
			if (provjera != 1)
			{
				XMLDocument oXmlDocument;
				oXmlDocument.LoadFile("vozila.xml");
				XMLElement* pRoot = oXmlDocument.FirstChildElement("dataset");
				XMLElement* pElement = pRoot->FirstChildElement("Vozila");
				XMLElement* pRootElement = oXmlDocument.NewElement("Vozilo");
				oXmlDocument.InsertFirstChild(pRootElement);
				pRootElement->SetAttribute("tip", "Automobil");
				pRootElement->SetAttribute("id", stoi(max));
				pRootElement->SetAttribute("naziv", novoVozilo.c_str());
				pRootElement->SetAttribute("datum_reg", uneseniDatum.c_str());
				pElement->InsertEndChild(pRootElement);
				oXmlDocument.SaveFile("vozila.xml");
			}		
			vVozilo.clear();
			vID.clear();
			break;
		}
		case 50:
		{
			system("CLS");
			cout << "\nNaziv kamiona: ";
			getline(cin, novoVozilo);
			do
			{
				cin.ignore(4096, '\n');
				system("CLS");
				cout << "ID: ";
				cin >> max;
				is_num_bool = ProvjeriIntegerUnos(max);
				if (is_num_bool == true)
				{
					if (count(vID.begin(), vID.end(), stoi(max)))
					{
						cout << "\nUneseni ID je vec zauzet!!";
						is_num_bool = false;
					}
				}
			} while (is_num_bool == false);
			do {
				provjera = 0;
				system("CLS");
				cout << "\n\nOdaberite novi datum registracije u obliku DD-MM-YYYY" << endl;
				cin.ignore(4096, '\n');
				getline(cin, uneseniDatum);
				if (uneseniDatum.size() < 10 || uneseniDatum.size() > 10)
				{
					cout << "Krivi format datuma" << endl;
					provjera = 1;
				}
				else
				{
					for (int i = 0; i < 10; i++)
					{
						if (i >= 0 && i < 2)
						{
							dan += uneseniDatum[i];
						}
						if (i >= 3 && i < 5)
						{
							mjesec += uneseniDatum[i];
						}
						if (i > 5)
						{
							godina += uneseniDatum[i];
						}
					}
					try
					{
						i_dan = stoi(dan);
						i_mjesec = stoi(mjesec);
						i_godina = stoi(godina);
					}
					catch (const invalid_argument& e)
					{
						cout << "Krivi format datuma" << endl;
						return;
					}
					Sc = Switch_ERROR_message_ALERT(i_dan, i_mjesec, i_godina, uneseniDatum);
					system("pause");
				}
			} while (!Sc);
			if (provjera != 1) 
			{
				XMLDocument oXmlDocument;
				oXmlDocument.LoadFile("vozila.xml");
				XMLElement* pRoot = oXmlDocument.FirstChildElement("dataset");
				XMLElement* pElement = pRoot->FirstChildElement("Vozila");
				XMLElement* pRootElement = oXmlDocument.NewElement("Vozilo");
				oXmlDocument.InsertFirstChild(pRootElement);
				pRootElement->SetAttribute("tip", "Kamion");
				pRootElement->SetAttribute("id", stoi(max));
				pRootElement->SetAttribute("naziv", novoVozilo.c_str());
				pRootElement->SetAttribute("datum_reg", uneseniDatum.c_str());
				pElement->InsertEndChild(pRootElement);
				oXmlDocument.SaveFile("vozila.xml");
			}			
			vVozilo.clear();
			vID.clear();
			break;
		}
		case 51:
		{
			system("CLS");
			cout << "\nNaziv motocikla: ";
			getline(cin, novoVozilo);
			do
			{
				cin.ignore(4096, '\n');
				system("CLS");
				cout << "ID: ";
				cin >> max;
				is_num_bool = ProvjeriIntegerUnos(max);
				if (is_num_bool == true)
				{
					if (count(vID.begin(), vID.end(), stoi(max)))
					{
						cout << "\nUneseni ID je vec zauzet!!";
						is_num_bool = false;
					}
				}
			} while (is_num_bool == false);
			do {
				provjera = 0;
				system("CLS");
				cout << "\n\nOdaberite novi datum registracije u obliku DD-MM-YYYY" << endl;
				cin.ignore(4096, '\n');
				getline(cin, uneseniDatum);
				if (uneseniDatum.size() < 10 || uneseniDatum.size() > 10)
				{
					cout << "Krivi format datuma" << endl;
					provjera = 1;
				}
				else
				{
					for (int i = 0; i < 10; i++)
					{
						if (i >= 0 && i < 2)
						{
							dan += uneseniDatum[i];
						}
						if (i >= 3 && i < 5)
						{
							mjesec += uneseniDatum[i];
						}
						if (i > 5)
						{
							godina += uneseniDatum[i];
						}
					}
					try
					{
						i_dan = stoi(dan);
						i_mjesec = stoi(mjesec);
						i_godina = stoi(godina);
					}
					catch (const invalid_argument& e)
					{
						cout << "Krivi format datuma" << endl;
						return;
					}
					Sc = Switch_ERROR_message_ALERT(i_dan, i_mjesec, i_godina, uneseniDatum);
					system("pause");
				}
			} while (!Sc);
			if (provjera != 1)
			{
				XMLDocument oXmlDocument;
				oXmlDocument.LoadFile("vozila.xml");
				XMLElement* pRoot = oXmlDocument.FirstChildElement("dataset");
				XMLElement* pElement = pRoot->FirstChildElement("Vozila");
				XMLElement* pRootElement = oXmlDocument.NewElement("Vozilo");
				oXmlDocument.InsertFirstChild(pRootElement);
				pRootElement->SetAttribute("tip", "Motocikl"),
				pRootElement->SetAttribute("id", stoi(max));
				pRootElement->SetAttribute("naziv", novoVozilo.c_str());
				pRootElement->SetAttribute("datum_reg", uneseniDatum.c_str());
				pElement->InsertEndChild(pRootElement);
				oXmlDocument.SaveFile("vozila.xml");
			}		
			vVozilo.clear();
			vID.clear();
			break;
		}
		case 52:
		{
			system("CLS");
			cout << "\nNaziv radnog stroja: ";
			getline(cin, novoVozilo);
			do
			{
				cin.ignore(4096, '\n');
				system("CLS");
				cout << "ID: ";
				cin >> max;
				is_num_bool = ProvjeriIntegerUnos(max);
				if (is_num_bool == true)
				{
					if (count(vID.begin(), vID.end(), stoi(max)))
					{
						cout << "\nUneseni ID je vec zauzet!!";
						is_num_bool = false;
					}
				}
			} while (is_num_bool == false);
			do {
				provjera = 0;
				system("CLS");
				cout << "\n\nOdaberite novi datum registracije u obliku DD-MM-YYYY" << endl;
				cin.ignore(4096, '\n');
				getline(cin, uneseniDatum);
				if (uneseniDatum.size() < 10 || uneseniDatum.size() > 10)
				{
					cout << "Krivi format datuma" << endl;
					provjera = 1;
				}
				else
				{
					for (int i = 0; i < 10; i++)
					{
						if (i >= 0 && i < 2)
						{
							dan += uneseniDatum[i];
						}
						if (i >= 3 && i < 5)
						{
							mjesec += uneseniDatum[i];
						}
						if (i > 5)
						{
							godina += uneseniDatum[i];
						}
					}
					try
					{
						i_dan = stoi(dan);
						i_mjesec = stoi(mjesec);
						i_godina = stoi(godina);
					}
					catch (const invalid_argument& e)
					{
						cout << "Krivi format datuma" << endl;
						return;
					}
					Sc = Switch_ERROR_message_ALERT(i_dan, i_mjesec, i_godina, uneseniDatum);
					system("pause");
				}
			} while (!Sc);
			if (provjera != 1)
			{
				XMLDocument oXmlDocument;
				oXmlDocument.LoadFile("vozila.xml");
				XMLElement* pRoot = oXmlDocument.FirstChildElement("dataset");
				XMLElement* pElement = pRoot->FirstChildElement("Vozila");
				XMLElement* pRootElement = oXmlDocument.NewElement("Vozilo");
				oXmlDocument.InsertFirstChild(pRootElement);
				pRootElement->SetAttribute("tip", "Radni stroj");
				pRootElement->SetAttribute("id", stoi(max));
				pRootElement->SetAttribute("naziv", novoVozilo.c_str());
				pRootElement->SetAttribute("datum_reg", uneseniDatum.c_str());
				pElement->InsertEndChild(pRootElement);
				oXmlDocument.SaveFile("vozila.xml");
			}		
			vVozilo.clear();
			vID.clear();
			break;
		}
	}	
}
void Main::ObrisiXML(string odabir)
{
	XMLDocument oXmlDocument;
	oXmlDocument.LoadFile("vozila.xml");
	XMLElement* pRoot = oXmlDocument.FirstChildElement("dataset");
	XMLElement* pChild = pRoot->FirstChildElement("Vozila");
	XMLElement* pChildRoot;
	for (pChildRoot = pChild->FirstChildElement("Vozilo"); pChildRoot != NULL; pChildRoot = pChildRoot->NextSiblingElement())
	{
		if (pChildRoot->Attribute("id") == odabir)
		{
			pChild->DeleteChild(pChildRoot);
			oXmlDocument.SaveFile("vozila.xml");
			break;
		}
	}
}

void Main::AzurirajXML(string odabir2, string nazivNew, string datumRegNew)
{
	XMLDocument oXmlDocument;
	oXmlDocument.LoadFile("vozila.xml");
	XMLElement* pRoot = oXmlDocument.FirstChildElement("dataset");
	XMLElement* pChild = pRoot->FirstChildElement("Vozila");
	XMLElement* pChildRoot;
	for (pChildRoot = pChild->FirstChildElement("Vozilo"); pChildRoot != NULL; pChildRoot = pChildRoot->NextSiblingElement())
	{
		if (pChildRoot->Attribute("id") == odabir2)
		{
			pChildRoot->SetAttribute("id", odabir2.c_str());
			pChildRoot->SetAttribute("naziv", nazivNew.c_str());
			pChildRoot->SetAttribute("datum_reg", datumRegNew.c_str());			
			oXmlDocument.SaveFile("vozila.xml");
			break;
		}
	}
}

void Main::AzurirajVozilo()
{
	system("CLS");
	string ID_new="";
	string uneseniDatum = "", dan = "", mjesec = "", godina = "", dan1 = "", mjesec1 = "", godina1 = "";
	int i_dan, i_mjesec, i_godina, i_ID;
	bool x = true;
	bool Sc = 0;
	cout << "***AZURIRANJE VOZILA***\n" << endl;
	CitanjeXMLVozila();

	TextTable t('-', '|', '+');
	int rbr = 1;

	t.add("R.Br");
	t.add("ID vozila");
	t.add("Naziv vozila");
	t.add("Datum registracije");
	t.endOfRow();

	for (int x = 0; x < vVozilo.size(); x++)
	{
		t.add(to_string(rbr));
		t.add(to_string(vVozilo[x]->GetID()));
		t.add(vVozilo[x]->GetName());
		t.add(vVozilo[x]->GetDate());
		t.endOfRow();
		rbr++;
	}
	bool is_num_bool = false;
	cout << t;
	cout << "\nUpisite ID vozila koje zelite azurirati" << endl;
	do
	{
		cout << "ID: ";
		cin >> ID_new;
		cin.ignore(4096, '\n');
		is_num_bool = ProvjeriIntegerUnos(ID_new);
		if (is_num_bool == true)
		{
			if (count(vID.begin(), vID.end(), stoi(ID_new)))
			{
				//is_num_bool = true;
			}
			else
			{
				cout << "\nUneseni ID ne postoji!!\n";
				is_num_bool = false;
			}
		}
	} while (is_num_bool == false);
	
	cout << "\nUpisite novi naziv vozila: " << endl;
	cin.ignore(4096, '\n');
	string nazivNew;
	getline(cin, nazivNew);
	do {
		system("CLS");
		cout << "\n\nOdaberite novi datum registracije u obliku DD-MM-YYYY" << endl;
		cin.ignore(4096, '\n');
		getline(cin, uneseniDatum);
		if (uneseniDatum.size() < 10 || uneseniDatum.size() > 10)
		{
			cout << "Krivi format datuma" << endl;
		}
		else
		{
			for (int i = 0; i < 10; i++)
			{
				if (i >= 0 && i < 2)
				{
					dan += uneseniDatum[i];
				}
				if (i >= 3 && i < 5)
				{
					mjesec += uneseniDatum[i];
				}
				if (i > 5)
				{
					godina += uneseniDatum[i];
				}
			}
			try
			{
				i_dan = stoi(dan);
				i_mjesec = stoi(mjesec);
				i_godina = stoi(godina);
			}
			catch (const invalid_argument& e)
			{
				cout << "Krivi format datuma" << endl;
				return;
			}
			Sc = Switch_ERROR_message_ALERT(i_dan, i_mjesec, i_godina, uneseniDatum);
			system("pause");
		}
	} while (!Sc);
	vVozilo.clear();
	AzurirajXML(ID_new, nazivNew, uneseniDatum);
	system("CLS");
	cout << "Vozilo uspjesno azurirano" << endl;
}


void Main::BrisanjeVozila()
{
	system("CLS");
	cout << "***BRISANJE VOZILA***\n" << endl;
	CitanjeXMLVozila();
	string ID_new="";
	bool x = true;
	bool Sc = 0;
	TextTable t('-', '|', '+');
	int rbr = 1;

	t.add("R.Br");
	t.add("ID vozila");
	t.add("Naziv vozila");
	t.add("Datum registracije");
	t.endOfRow();

	for (int x = 0; x < vVozilo.size(); x++)
	{
		t.add(to_string(rbr));
		t.add(to_string(vVozilo[x]->GetID()));
		t.add(vVozilo[x]->GetName());
		t.add(vVozilo[x]->GetDate());
		t.endOfRow();
		rbr++;
	}
	bool is_num_bool = false;
	cout << t;
	cout << "\nUpisite ID vozila koje zelite obrisati:" << endl;
	do
	{			
		cout << "ID: ";
		cin >> ID_new;
		is_num_bool = ProvjeriIntegerUnos(ID_new);
		if (is_num_bool == true)
		{
			if (count(vID.begin(), vID.end(), stoi(ID_new)))
			{
				is_num_bool = true;
			}
			else
			{
				cout << "\nUneseni ID ne postoji!!";
				is_num_bool = false;
			}
		}
	} while (is_num_bool == false);
	vVozilo.clear();
	ObrisiXML(ID_new);
	system("CLS");
	cout << "Vozilo uspjesno uklonjeno" << endl;
}

void Main::Izbornik1()
{
	system("CLS");
	cout << "***POD IZBORNIK***" << endl;
	cout << "Odaberite opciju:\n\n1. Dodavanje vozila \n\n2. Azuriranje vozila\n\n3. Brisanje vozila\n" << endl;
	cout << "Vas izbor je: ";
	int izbor = _getch();
	while (izbor != 49 && izbor != 50 && izbor != 51)
	{
		cout << "Ponovite izbor: " << endl;
		izbor = _getch();
	}
	switch (izbor)
	{
		case 49:
			DodavanjeVozila();
			break;

		case 50:
			AzurirajVozilo();
			break;

		case 51:
			BrisanjeVozila();
			break;
	}
}

int Main::POS_INT_TRANS(char* system_number)
{
	int sum = 0;

	if (system_number == 0)
	{
		return -1;
	}
	for (int i = 0; system_number[i] != '\0'; i++)
	{
		if (system_number[i] < 48 || system_number[i]>57)
		{
			return -1;//no arithmetic characters
		}
	}
	sum = atoi(system_number);

	return sum;
}

void Main::ListaVozila()
{
	system("CLS");
	CitanjeXMLVozila();
	string datumRegistracije, trenutniDatum, dan = "", mjesec = "", godina = "", reg_dan = "", reg_mjesec = "", reg_godina = "";

	vector<int> ukupniDani;

	trenutniDatum = Date();

	for (int i = 0; i < 10; i++)
	{
		if (i >= 0 && i < 2)
		{
			dan += trenutniDatum[i];
		}
		if (i >= 3 && i < 5)
		{
			mjesec += trenutniDatum[i];
		}
		if (i > 5)
		{
			godina += trenutniDatum[i];
		}
	}
	int ukupnoDana = 0;
	for (int i = 0; i < vID.size(); i++)
	{
		datumRegistracije = vReg.at(i);
		for (int j = 0; j < 10; j++)
		{
			if (j >= 0 && j < 2)
			{
				reg_dan += datumRegistracije[j];
			}
			if (j >= 3 && j < 5)
			{
				reg_mjesec += datumRegistracije[j];
			}
			if (j > 5)
			{
				reg_godina += datumRegistracije[j];
			}
		}
		int upd_god = 0;
		bool isteklo = false;
		if (vVozilo[i]->m_sTip == "Automobil" || vVozilo[i]->m_sTip == "Kamion")
		{
			upd_god = stoi(reg_godina) + 1;
			if (stoi(reg_godina) < stoi(godina))
			{
				if (stoi(reg_mjesec) < stoi(mjesec))
				{
					isteklo = true;
				}
				if (stoi(reg_mjesec) == stoi(mjesec))
				{
					if (stoi(reg_dan) < stoi(dan))
					{
						isteklo = true;
					}
				}
			}
		}
		else if (vVozilo[i]->m_sTip == "Motocikl")
		{
			upd_god = stoi(reg_godina) + 2;
			if (stoi(reg_godina) + 1 < stoi(godina))
			{
				if (stoi(reg_mjesec) < stoi(mjesec))
				{
					isteklo = true;
				}
				if (stoi(reg_mjesec) == stoi(mjesec))
				{
					if (stoi(reg_dan) < stoi(dan))
					{
						isteklo = true;
					}
				}
			}
		}
		else if (vVozilo[i]->m_sTip == "Radni stroj")
		{
			upd_god = stoi(reg_godina) + 3;
			if (stoi(reg_godina) + 2 < stoi(godina))
			{
				if (stoi(reg_mjesec) < stoi(mjesec))
				{
					isteklo = true;
				}
				if (stoi(reg_mjesec) == stoi(mjesec))
				{
					if (stoi(reg_dan) < stoi(dan))
					{
						isteklo = true;
					}
				}
			}
		}
		ukupnoDana = Get_The_Number_of_Days(stoi(godina), upd_god, stoi(dan), stoi(reg_dan), stoi(mjesec), stoi(reg_mjesec));
		if (isteklo == true)
		{
			ukupnoDana = 0;
		}
		ukupniDani.push_back(ukupnoDana);
		reg_dan = "", reg_mjesec = "", reg_godina = "";
	}
	cout << "***LISTA VOZILA***\n\n" << endl;
	TextTable t('-', '|', '+');
	int rbr = 1;
	t.add("R.Br");
	t.add("ID vozila");
	t.add("Naziv vozila");
	t.add("Datum registracije");
	t.add("Br. dana do isteka reg.");
	t.endOfRow();

	for (int z = 0; z < vID.size(); z++)
	{
		t.add(to_string(rbr));
		t.add(to_string(vVozilo[z]->GetID()));
		t.add(vVozilo[z]->GetName());
		t.add(vVozilo[z]->GetDate());
		t.add(to_string(ukupniDani[z]));
		t.endOfRow();
		rbr++;
	}
	cout << t;
}

bool Main::Switch_ERROR_message_ALERT(int d, int m, int y, string datum)
{
	int control = -1;

	if (y < 0)
		control = 1;
	if (m > 12)
		control = 2;
	if (m < 1)
		control = 3;
	if (d > 31)
		control = 4;
	if (d < 1)
		control = 5;
	if (m == 2/*February*/ && d > 29)
		control = 6;
	if (m == 2 && (y % 4) != 0/*no leap year*/ && d == 29)
		control = 7;
	if (m == 4/*April*/ && d == 31)
		control = 8;
	if (m == 6/*June*/ && d == 31)
		control = 9;
	if (m == 9/*September*/ && d == 31)
		control = 10;
	if (m == 11/*November*/ && d == 31)
		control = 11;

	if (datum[2] != '-' || datum[5] != '-')
	{
		control = 0;
	}

	switch (control)
	{
	case  0: cout << "--ERROR--Wrong format of the given data." << endl; break;
	case  1: cout << "--ERROR--The year you gave is invalid.Year should be >0 and <=9999." << endl; return 0; break;
	case  2: cout << "--ERROR--The month you gave is invalid.Month should be >0 and <13." << endl; return 0; break;
	case  3: cout << "--ERROR--The month you gave is invalid.Month should be >0 and <13." << endl; return 0; break;
	case  4: cout << "--ERROR--The day you gave is too large.Day should be >0 and <32." << endl; return 0; break;
	case  5: cout << "--ERROR--The day you gave is invalid.Day should be >0 and <32." << endl; return 0; break;
	case  6: cout << "--ERROR--There is no such day[" << d << "] on February." << endl; return 0; break;
	case  7: cout << "--ERROR--The year[" << y << "] is not a leap one.Thus,29th February " << y << " is not a valid date." << endl; return 0; break;
	case  8: cout << "--ERROR--April has just 30 days." << endl; return 0; break;
	case  9: cout << "--ERROR--June has just 30 days." << endl; return 0; break;
	case 10: cout << "--ERROR--September has just 30 days." << endl; return 0; break;
	case 11: cout << "--ERROR--November has just 30 days." << endl; return 0; break;

	default: return 1; break;
	}
}

int Main::Get_The_Number_of_Days(int godina1, int godina2, int dan1, int dan2, int mjesec1, int mjesec2)
{
	int m_day1, m_month1, m_year1, m_day2, m_month2, m_year2;
	int C, Bound, month_CounteR;
	int TotaL_DAYS = 0;

	if ((godina1 < godina2) || ((godina1 == godina2) && (mjesec1 < mjesec2)) || ((godina1 == godina2) && (mjesec1 == mjesec2) && (dan1 <= dan2)))
	{
		m_day1 = dan1;
		m_day2 = dan2;
		m_month1 = mjesec1;
		m_month2 = mjesec2;
		m_year1 = godina1;
		m_year2 = godina2;
	}
	if ((godina1 > godina2) || ((godina1 == godina2) && (mjesec1 > mjesec2)) || ((godina1 == godina2) && (mjesec1 == mjesec2) && (dan1 > dan2)))
	{
		m_day1 = dan2;
		m_day2 = dan1;
		m_month1 = mjesec2;
		m_month2 = mjesec1;
		m_year1 = godina2;
		m_year2 = godina1;
	}

	for (int y = m_year1; y <= m_year2; y++)
	{
		if (y == m_year1)
		{
			C = m_month1;
			month_CounteR = 12;
		}
		if (y == m_year2)
		{
			month_CounteR = m_month2 - 1;
		}

		for (C; C <= month_CounteR; C++)
		{
			if (((C % 2 == 1) && (C != 9) && (C != 11)) || C == 8 || C == 10 || C == 12)//-January-March-May-July-August-October-December-
			{
				TotaL_DAYS = TotaL_DAYS + 31;
				continue;
			}

			if ((C == 2) && (y % 4 == 0))//-February and leap year-
			{
				TotaL_DAYS = TotaL_DAYS + 29;
				continue;
			}

			if ((C == 2) && (y % 4 != 0))//-February and no leap year-
			{
				TotaL_DAYS = TotaL_DAYS + 28;
				continue;
			}

			TotaL_DAYS = TotaL_DAYS + 30;//-Normal-*-April-June-September-November-
		}//--end of internal loop
		//cout<<TotaL_DAYS<<endl;
		C = 1;
	}//--end of external loop

	TotaL_DAYS = TotaL_DAYS - m_day1 + m_day2;

	return TotaL_DAYS;
}

string Main::Date()
{
	auto now = system_clock::now();
	auto in_time_t = system_clock::to_time_t(now);

	stringstream ss;
	ss << put_time(localtime(&in_time_t), "%d-%m-%Y");
	return ss.str();
}

void Main::RegistriranjeVozila()
{
	system("CLS");
	CitanjeXMLVozila();
	string datumRegistracije, trenutniDatum, dan = "", mjesec = "", godina = "", reg_dan = "", reg_mjesec = "", reg_godina = "";
	string uneseniDatum = "", dan1 = "", mjesec1 = "", godina1 = "";
	int i_dan, i_mjesec, i_godina, i_ID;
	cout << "***REGISTRIRANJE VOZILA***\n" << endl;
	TextTable t('-', '|', '+');
	int rbr = 1;
	string ID ="";
	bool x = true;
	t.add("R.Br");
	t.add("ID vozila");
	t.add("Naziv vozila");
	t.add("Datum registracije");
	t.endOfRow();

	for (int x = 0; x < vVozilo.size(); x++)
	{
		t.add(to_string(rbr));
		t.add(to_string(vVozilo[x]->GetID()));
		t.add(vVozilo[x]->GetName());
		t.add(vVozilo[x]->GetDate());
		t.endOfRow();
		rbr++;
	}
	cout << t;
	bool is_num_bool = false;
	do
	{
		cout << "Unesite ID vozila koje zelite registrirati\n" << endl;
		cout << "ID: ";
		cin >> ID;
		is_num_bool = ProvjeriIntegerUnos(ID);
		if (is_num_bool == true)
		{
			if (count(vID.begin(), vID.end(), stoi(ID)))
			{
				is_num_bool = true;
			}
			else
			{
				cout << "\nUneseni ID ne postoji!!";
				is_num_bool = false;
			}
		}
	} while (is_num_bool == false);

	bool Sc = 0;
	x = true;
	int provjera = 0;
	
	do 
	{
		provjera = 0;
		system("CLS");
		cout << "Odaberite novi datum registracije u obliku DD-MM-YYYY" << endl;
		cin.ignore(4096, '\n');
		getline(cin, uneseniDatum);
		if (uneseniDatum.size() < 10 || uneseniDatum.size() > 10)
		{
			cout << "Krivi format datuma" << endl;
			provjera = 1;
		}
		else
		{
			for (int i = 0; i < 10; i++)
			{
				if (i >= 0 && i < 2)
				{
					dan += uneseniDatum[i];
				}
				if (i >= 3 && i < 5)
				{
					mjesec += uneseniDatum[i];
				}
				if (i > 5)
				{
					godina += uneseniDatum[i];
				}
			}
			try
			{
				i_dan = stoi(dan);
				i_mjesec = stoi(mjesec);
				i_godina = stoi(godina);
			}
			catch (const invalid_argument& e)
			{
				cout << "Krivi format datuma" << endl;
				return;
			}
			Sc = Switch_ERROR_message_ALERT(i_dan, i_mjesec, i_godina, uneseniDatum);
			_getch();
		}
	} while (!Sc);
	if (provjera != 1)
	{
		XMLDocument oXmlDocument;
		oXmlDocument.LoadFile("vozila.xml");
		XMLElement* pRoot = oXmlDocument.FirstChildElement("dataset");
		XMLElement* pChild = pRoot->FirstChildElement("Vozila");
		XMLElement* pChildRoot;
		for (pChildRoot = pChild->FirstChildElement("Vozilo"); pChildRoot != NULL; pChildRoot = pChildRoot->NextSiblingElement())
		{
			if (pChildRoot->Attribute("id") == ID)
			{
				pChildRoot->SetAttribute("datum_reg", uneseniDatum.c_str());
				oXmlDocument.SaveFile("vozila.xml");
				break;
			}
		}
	}
	vVozilo.clear();
	CitanjeXMLVozila();
	string d_isteka_Cijeli = "";
	cout << "***DATUMI ISTEKA RAGISTRACIJE***\n" << endl;
	TextTable table('-', '|', '+');
	rbr = 1;
	table.add("R.Br");
	table.add("ID vozila");
	table.add("Naziv vozila");
	table.add("Datum registracije");
	table.add("Datum isteka reg.");
	table.endOfRow();

	for (int x = 0; x < vVozilo.size(); x++)
	{
		table.add(to_string(rbr));
		table.add(to_string(vVozilo[x]->GetID()));
		table.add(vVozilo[x]->GetName());
		table.add(vVozilo[x]->GetDate());
		string datum1 = vVozilo[x]->GetDate();
		for (int i = 0; i < 10; i++)
		{
			if (i >= 0 && i < 2)
			{
				dan1 += datum1[i];
			}
			if (i >= 3 && i < 5)
			{
				mjesec1 += datum1[i];
			}
			if (i > 5)
			{
				godina1 += datum1[i];
			}
		}		
		int upd_god = 0;
		if (vVozilo[x]->m_sTip == "Automobil" || vVozilo[x]->m_sTip == "Kamion")
		{
			upd_god = stoi(godina1) + 1;
			d_isteka_Cijeli = dan1 + "-" + mjesec1 + "-" + to_string(upd_god);		
		}
		else if (vVozilo[x]->m_sTip == "Motocikl")
		{
			upd_god = stoi(godina1) + 2;
			d_isteka_Cijeli = dan1 + "-" + mjesec1 + "-" + to_string(upd_god);		
		}
		else if (vVozilo[x]->m_sTip == "Radni stroj")
		{
			upd_god = stoi(godina1) + 3;
			d_isteka_Cijeli = dan1 + "-" + mjesec1 + "-" + to_string(upd_god);
		}
		table.add(d_isteka_Cijeli);
		table.endOfRow();
		dan1 = "";
		mjesec1 = "";
		godina1 = "";
		d_isteka_Cijeli = "";		
		rbr++;	
	}
	cout << table;
}

void Main::ZaduziVozilo()
{
	CitanjeXMLVozila();
	CitanjeXMLZaposlenici();
	CitanjeXMLZapisi();
	system("CLS");
	int rbr = 1;
	string id = "";
	bool x = true;
	bool Sc = 0;
	int brojacKoristenjaVozila = 0, brojKoristenjaZaposlenika = 0;
	string uneseniDatum = "", dan = "", mjesec = "", godina = "";
	int i_dan, i_mjesec, i_godina, i_ID;
	cout << "***ZADUZIVANJE VOZILA***\n\n" << endl;
	cout << "**PRIKAZ DOSTUPNIH VOZILA**\n" << endl;
	TextTable t('-', '|', '+');
	t.add("R.Br");
	t.add("ID vozila");
	t.add("Naziv vozila");
	t.add("Datum registracije");
	t.endOfRow();

	for (int x = 0; x < vVozilo.size(); x++)
	{
		if (count(vZapisVozId.begin(), vZapisVozId.end(),vVozilo[x]->m_nId))
		{

		}
		else
		{
			t.add(to_string(rbr));
			t.add(to_string(vVozilo[x]->GetID()));
			t.add(vVozilo[x]->GetName());
			t.add(vVozilo[x]->GetDate());
			t.endOfRow();
			rbr++;
		}
	}
	cout << t;
	cout << "\nUpisite ID zeljenog vozila" << endl;
	bool is_num_bool = false;
	do
	{
		cout << "ID: ";
		cin >> id;
		is_num_bool = ProvjeriIntegerUnos(id);
		if (is_num_bool == true)
		{
			if (count(vID.begin(), vID.end(), stoi(id)))
			{
				is_num_bool = true;
			}
			else
			{
				cout << "\nUneseni ID ne postoji!!";
				is_num_bool = false;
			}
		}
	} while (is_num_bool == false);
	system("CLS");
	cout << "**PRIKAZ DOSTUPNIH ZAPOSLENIKA**\n\n" << endl;
	TextTable t2('-', '|', '+');
	string izbor="";
	rbr = 1;
	x = true;
	t2.add("R.Br");
	t2.add("ID zaposlenika");	
	t2.add("Ime");
	t2.add("Prezime");
	t2.add("OIB zaposlenika");
	t2.endOfRow();
	for (int k = 0; k < vZaposlenik.size(); k++)
	{
		if (vZaposlenik[k]->m_oVozilo == nullptr)
		{
			t2.add(to_string(rbr));
			t2.add(to_string(vZaposlenik[k]->m_sId));
			t2.add(vZaposlenik[k]->m_sOib);
			t2.add(vZaposlenik[k]->m_sIme);
			t2.add(vZaposlenik[k]->m_sPrezime);
			t2.endOfRow();
			rbr++;
		}
	}
	cout << t2;
	int i_ID2;
	bool is_num_bool2 = false;
	do
	{
		cout << "Unesite ID zaposlenika\n";
		cout << "ID: ";
		cin >> izbor;
		is_num_bool2 = ProvjeriIntegerUnos(izbor);
		if (is_num_bool2 == true)
		{				
			if (count(vIDZap.begin(), vIDZap.end(), stoi(izbor)))
			{
				is_num_bool2 = true;
			}
			else
			{
				cout << "\nUneseni ID ne postoji!!";
				is_num_bool2 = false;
			}		
		}
	} while (is_num_bool2 == false);

	do {
		system("CLS");
		cin.ignore(4096, '\n');
		cout << "\n\nOdaberite zeljeni datum zaduzivanja u obliku DD-MM-YYYY" << endl;
		getline(cin, uneseniDatum);
		if (uneseniDatum.size() < 10 || uneseniDatum.size() > 10)
		{
			cout << "Krivi format datuma" << endl;
		}
		else
		{
			for (int i = 0; i < 10; i++)
			{
				if (i >= 0 && i < 2)
				{
					dan += uneseniDatum[i];
				}
				if (i >= 3 && i < 5)
				{
					mjesec += uneseniDatum[i];
				}
				if (i > 5)
				{
					godina += uneseniDatum[i];
				}
			}
			try
			{
				i_dan = stoi(dan);
				i_mjesec = stoi(mjesec);
				i_godina = stoi(godina);
			}
			catch (const invalid_argument& e)
			{
				cout << "Krivi format datuma" << endl;
				return;
			}
			Sc = Switch_ERROR_message_ALERT(i_dan, i_mjesec, i_godina, uneseniDatum);
			_getch();
		}
	} while (!Sc);
	XMLDocument oXmlDocument;
	oXmlDocument.LoadFile("zapis.xml");
	XMLElement* pRoot = oXmlDocument.FirstChildElement("dataset");
	XMLElement* pElement = pRoot->FirstChildElement("Zapis");
	XMLElement* pRootElement = oXmlDocument.NewElement("Zaduzivanje");
	oXmlDocument.InsertFirstChild(pRootElement);
	pRootElement->SetAttribute("zaposlenik_id", izbor.c_str());
	pRootElement->SetAttribute("vozilo_id", id.c_str());
	pRootElement->SetAttribute("datum", uneseniDatum.c_str());
	pElement->InsertEndChild(pRootElement);
	oXmlDocument.SaveFile("zapis.xml");
	PisiXMLZapis(1,izbor,id, uneseniDatum);	
}

void Main::VratiVozilo()
{
	CitanjeXMLVozila();
	CitanjeXMLZaposlenici();
	CitanjeXMLZapisi();
	system("CLS");
	int rbr = 1;
	string id = "";
	bool x = true;
	bool Sc = 0;
	int izborNovo = 0;
	string uneseniDatum = "", dan = "", mjesec = "", godina = "";
	int i_dan, i_mjesec, i_godina, i_ID;
	cout << "***VRATI VOZILO***\n\n" << endl;
	cout << "**ODABIR IZ ZADUZENIH VOZILA**\n" << endl;
	TextTable t('-', '|', '+');
	t.add("R.Br");
	t.add("ID vozila");
	t.add("Naziv vozila");
	t.add("Datum registracije");
	t.endOfRow();

	for (int x = 0; x < vVozilo.size(); x++)
	{
		if (count(vZapisVozId.begin(), vZapisVozId.end(), vID.at(x)))
		{
			t.add(to_string(rbr));
			t.add(to_string(vVozilo[x]->GetID()));
			t.add(vVozilo[x]->GetName());
			t.add(vVozilo[x]->GetDate());
			t.endOfRow();
			rbr++;
		}	
	}
	cout << t;
	cout << "\nUpisite ID vozila kojeg zelite vratiti" << endl;
	bool is_num_bool = false;
	do
	{
		cout << "ID: ";
		cin >> id;
		is_num_bool = ProvjeriIntegerUnos(id);
		if (is_num_bool == true)
		{			
			if (count(vID.begin(), vID.end(), stoi(id)))
			{
				is_num_bool = true;
			}
			else
			{
				cout << "\nUneseni ID ne postoji!!";
				is_num_bool = false;
			}
			
		}
	} while (is_num_bool == false);


	for (int i = 0; i < vZapisVozId.size(); i++)
	{
		if (vZapisVozId[i] == stoi(id))
		{
			izborNovo = vZapisZapId[i];
		}
	}
	
	do {
		system("CLS");
		cin.ignore(4096, '\n');
		cout << "Odaberite zeljeni datum vracanja u obliku DD-MM-YYYY" << endl;
		getline(cin, uneseniDatum);
		if (uneseniDatum.size() < 10 || uneseniDatum.size() > 10)
		{
			cout << "Krivi format datuma" << endl;
		}
		else
		{
			for (int i = 0; i < 10; i++)
			{
				if (i >= 0 && i < 2)
				{
					dan += uneseniDatum[i];
				}
				if (i >= 3 && i < 5)
				{
					mjesec += uneseniDatum[i];
				}
				if (i > 5)
				{
					godina += uneseniDatum[i];
				}
			}
			try
			{
				i_dan = stoi(dan);
				i_mjesec = stoi(mjesec);
				i_godina = stoi(godina);
			}
			catch (const invalid_argument& e)
			{
				cout << "Krivi format datuma" << endl;
				return;
			}
			Sc = Switch_ERROR_message_ALERT(i_dan, i_mjesec, i_godina, uneseniDatum);
			_getch();
		}
	} while (!Sc);

	
	XMLDocument oXmlDoc;
	oXmlDoc.LoadFile("zapis.xml");
	XMLElement* pRoot1 = oXmlDoc.FirstChildElement("dataset");
	XMLElement* pChild1 = pRoot1->FirstChildElement("Zapis");
	XMLElement* pChildRoot1;
	for (pChildRoot1 = pChild1->FirstChildElement("Zaduzivanje"); pChildRoot1 != NULL; pChildRoot1 = pChildRoot1->NextSiblingElement())
	{
		if (pChildRoot1->Attribute("vozilo_id") == id && pChildRoot1->Attribute("zaposlenik_id") == to_string(izborNovo))
		{
			pChild1->DeleteChild(pChildRoot1);
			oXmlDoc.SaveFile("zapis.xml");
			break;
		}
	}
	PisiXMLZapis(2, to_string(izborNovo), id, uneseniDatum);
}

void Main::BrojZaduzivanjaPoZaposleniku()
{
	CitanjeXMLVozila();
	CitanjeXMLZaposlenici();	
	CitanjeZapisa();
	system("CLS");
	int rbr = 1;
	cout << "***STATISTIKA ZADUZIVANJA PO ZAPOSLENIKU***\n\n" << endl;
	TextTable t2('-', '|', '+');
	t2.add("R.Br");
	t2.add("ID zaposlenika");
	t2.add("Ime");
	t2.add("Prezime");
	t2.add("OIB zaposlenika");
	t2.add("Broj zaduzivanja");
	t2.endOfRow();
	for (int x = 0; x < vZaposlenik.size(); x++)
	{	
		int brojZaduzivanja = 0;
		for (int i = 0; i < vZapisi.size(); i++)
		{			
			if (vZaposlenik[x]->m_sId == vZapisi[i]->m_sIdZaposlenika)
			{			
				if (vZapisi[i]->m_sAkcija == "Zaduzivanje")
				{
					brojZaduzivanja++;
				}
			}		
		}
		t2.add(to_string(rbr));
		t2.add(to_string(vZaposlenik[x]->m_sId));
		t2.add(vZaposlenik[x]->m_sOib);
		t2.add(vZaposlenik[x]->m_sIme);
		t2.add(vZaposlenik[x]->m_sPrezime);
		t2.add(to_string(brojZaduzivanja));
		t2.endOfRow();
		rbr++;
	}
	cout << t2;
}

void Main::ProsjekVozila()
{
	CitanjeXMLVozila();
	CitanjeXMLZaposlenici();
	CitanjeZapisa();
	system("CLS");
	int rbr = 1;
	cout << "***STATISTIKA KORISTENJA VOZILA***\n\n" << endl;
	TextTable t2('-', '|', '+');
	t2.add("R.Br");
	t2.add("ID vozila");
	t2.add("Naziv vozila");
	t2.add("Broj zaduzivanja");
	t2.endOfRow();
	for (int j = 0; j < vVozilo.size(); j++)
	{
		int brojZaduzivanja = 0;
		for (int i = 0; i < vZapisi.size(); i++)
		{
			if (vVozilo[j]->m_nId == vZapisi[i]->m_sIdVozila)
			{
				if (vZapisi[i]->m_sAkcija == "Zaduzivanje")
				{
					brojZaduzivanja++;
				}
			}
		}

		t2.add(to_string(rbr));
		t2.add(to_string(vVozilo[j]->m_nId));
		t2.add(vVozilo[j]->m_sNaziv);		
		t2.add(to_string(brojZaduzivanja));
		t2.endOfRow();
		rbr++;
	}
	cout << t2;
}

void Main::Izbornik2()
{
	cout << "***POD IZBORNIK***" << endl;
	cout << "Odaberite opciju:\n\n1. Statistika zaduzivanja po zaposleniku \n\n2. Statistika koristenja vozila\n" << endl;
	cout << "Vas izbor je: ";
	int izbor = _getch();
	while (izbor != 49 && izbor != 50 )
	{
		cout << "Ponovite izbor: " << endl;
		izbor = _getch();
	}
	switch (izbor)
	{

	case 49:
		BrojZaduzivanjaPoZaposleniku();
		break;

	case 50:
		ProsjekVozila();
		break;

	}
}

void Main::Povratak()
{
	cout << "Q -> Glavni izbornik " << endl;
	cout << "X -> Izlaz" << endl;
	int izbor = _getch();
	while (izbor != 113 && izbor != 120)
	{
		cout << "Ponovite izbor: " << endl;
		izbor = _getch();
	}
	switch (izbor)
	{
	case 113:
	{
		cout << "\n";
		vVozilo.clear();
		vZapisi.clear();
		vZaposlenik.clear();
		vZapisVozId.clear();
		vZapisZapId.clear();
		vID.clear();
		vReg.clear();	
		CitanjeXMLVozila();
		CitanjeXMLZapisi();
		CitanjeXMLZaposlenici();
		CitanjeZapisa();
		GlavniIzbornik();
		break;
	}
	case 120:
	{
		exit(0);
		break;
	}
	}
}

void Main::GlavniIzbornik()
{
	system("CLS");
	cout << "****IZBORNIK****" << endl;
	cout << "Odaberite opciju:\n\n1. Dodavanje/Azuriranje/Brisanje vozilo \n\n2. Lista vozila \n\n3. Registriranje vozila \n\n4. Zaduzi vozilo \n\n5. Vrati vozilo \n\n6. Statistika\n" << endl;
	cout << "Vas izbor je: ";
	int izbor = _getch();
	while (izbor != 49 && izbor != 50 && izbor != 51 && izbor != 52 && izbor != 53 && izbor != 54)
	{
		cout << "Ponovite izbor: " << endl;
		izbor = _getch();
	}
	switch (izbor)
	{
	case 49:
		Izbornik1();
		Povratak();
		break;

	case 50:
		ListaVozila();
		Povratak();
		break;

	case 51:
		RegistriranjeVozila();
		Povratak();
		break;

	case 52:
		ZaduziVozilo();
		Povratak();
		break;

	case 53:
		VratiVozilo();
		Povratak();
		break;

	case 54:
		Izbornik2();
		Povratak();
		break;
	}
}