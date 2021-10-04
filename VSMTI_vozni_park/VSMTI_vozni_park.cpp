
#include <iostream>
#include "Osoba.h"
#include "Automobil.h"
#include "Kamion.h"
#include "Motocikl.h"
#include "Radni_stroj.h"
#include "Vozilo.h"
#include "Zaposlenik.h"
#include <vector>
#include "Main.h"
 

using namespace std;


/*----------------------------------------------+
+					OOP							+
+				Imra Kocis						+
+				0135256033						+	
+				Vozni park						+
+----------------------------------------------*/

int main()
{
	Main *Vozni_park = new Main();
	Vozni_park->GlavniIzbornik();

	system("pause");
	return 0;
}

