#include "SudokuErzeuger.h"
#include "ErzeugerElement.h"
#include "Random.h"
#include <QDebug>
#include <QMutex>

using namespace std;

SudokuErzeuger::SudokuErzeuger() : QObject(), freigeben(0)
{	
	// die listen der erzeuger elemente aufbauen
	// die elemente aufbauen
    for (int idx2 = 1; idx2 <= 9; idx2++)
	{
        for (int idx1 = 1; idx1 <= 9; idx1++)
		{
			// element erzeugen
			ErzeugerElement *element = new ErzeugerElement(this);
			
			// den objekt namen setzen
			element->setObjectName(Position(idx1, idx2).toString());
			
			gesamtliste.insert(Position(idx1, idx2), element);
		}
	}
	
	// die untergruppen aufbauen
	baue_liste_auf(1, 1, gruppe1);
	baue_liste_auf(4, 1, gruppe2);
	baue_liste_auf(7, 1, gruppe3);
	baue_liste_auf(1, 4, gruppe4);
	baue_liste_auf(4, 4, gruppe5);
	baue_liste_auf(7, 4, gruppe6);
	baue_liste_auf(1, 7, gruppe7);
	baue_liste_auf(4, 7, gruppe8);
	baue_liste_auf(7, 7, gruppe9);
}


SudokuErzeuger::~SudokuErzeuger()
{
}


void SudokuErzeuger::verarbeitung()
{
	QMutex mutex;
	
	mutex.lock();
	
	emit begonnen();
	
	// sicherstellen, das das raetsel auch gueltig ist
	bool ok = true;
	
	do
	{
		ok = true;
		
		reset();
		
		// die gruppem ausfuellen
		if (ok == true) ok = fuelle_gruppe_aus(gruppe1);
		if (ok == true) ok = fuelle_gruppe_aus(gruppe2);
		if (ok == true) ok = fuelle_gruppe_aus(gruppe3);
		if (ok == true) ok = fuelle_gruppe_aus(gruppe4);
		if (ok == true) ok = fuelle_gruppe_aus(gruppe5);
		if (ok == true) ok = fuelle_gruppe_aus(gruppe6);
		
		QList<ErzeugerElement*> rest(gruppe7);
		rest += gruppe8;
		rest += gruppe9;
		
		if (ok == true) ok = fuelle_gruppe_aus(rest);
	} while (ok == false);
	
	verdecke_elemente(gruppe1);
	verdecke_elemente(gruppe2);
	verdecke_elemente(gruppe3);
	verdecke_elemente(gruppe4);
	verdecke_elemente(gruppe5);
	verdecke_elemente(gruppe6);
	verdecke_elemente(gruppe7);
	verdecke_elemente(gruppe8);
	verdecke_elemente(gruppe9);
	
	emit erzeugtes_raetsel(zuString());
	
	emit fertig();
	
	mutex.unlock();
}


void SudokuErzeuger::neues_einfaches_spiel()
{
	freigeben = 4;
	
	verarbeitung();
}


void SudokuErzeuger::baue_liste_auf(int x, int y, QList<ErzeugerElement*>& erg) const
{
    for (int idx2a = x, idx2b = 1; idx2a < (x + 3); idx2a++, idx2b++)
	{
        for (int idx1a = y, idx1b = 1; idx1a < (y + 3); idx1a++, idx1b++)
		{
			ErzeugerElement *element = gesamtliste.value(Position(idx1a, idx2a));
			
			erg.append(element);
		}
	}
}


void SudokuErzeuger::reaktion_auf_zahlsetzung(ErzeugerElement* element)
{
	int zahl = element->zahl();
	
	QList<ErzeugerElement*> *gruppe = 0;
	
	if (gruppe1.contains(element) == true) gruppe = &gruppe1;
	else if (gruppe2.contains(element) == true) gruppe = &gruppe2;
	else if (gruppe3.contains(element) == true) gruppe = &gruppe3;
	else if (gruppe4.contains(element) == true) gruppe = &gruppe4;
	else if (gruppe5.contains(element) == true) gruppe = &gruppe5;
	else if (gruppe6.contains(element) == true) gruppe = &gruppe6;
	else if (gruppe7.contains(element) == true) gruppe = &gruppe7;
	else if (gruppe8.contains(element) == true) gruppe = &gruppe8;
	else if (gruppe9.contains(element) == true) gruppe = &gruppe9;
	else qDebug() << tr("Cant find element group in \"void SudokuErzeuger::reaktion_auf_zahlsetzung(ErzeugerElement* element)\"");
	
	if (gruppe != 0)
	{
        for (int idx = 0; idx < gruppe->size(); idx++)
		{
			ErzeugerElement *ziel = gruppe->at(idx);
			
			if (ziel != element) ziel->setze_moeglichkeit(zahl, false);
		}
		
        for (int idx = 1; idx <= 9; idx++)
		{
			// reihe
			ErzeugerElement *zielx = gesamtliste.value(Position(idx, gesamtliste.key(element).y()));
			
			if (zielx != element) zielx->setze_moeglichkeit(zahl, false);
			
			// spalte
			ErzeugerElement *ziely = gesamtliste.value(Position(gesamtliste.key(element).x(), idx));
			
			if (ziely != element) ziely->setze_moeglichkeit(zahl, false);
		}
	}
}


ErzeugerElement* SudokuErzeuger::finde_element_mit_wenigsten_moeglichkeiten(const QList<ErzeugerElement*>& liste)
{
	ErzeugerElement* erg = 0;
	
    for (int idx = 0; idx < liste.size(); idx++)
	{
		if (liste.at(idx)->ist_ausgefuellt() == false && (erg == 0 || (erg != 0 && erg->anzahl_moeglichkeiten() > liste.at(idx)->anzahl_moeglichkeiten()))) erg = liste.at(idx);
	}
	
	return erg;
}


bool SudokuErzeuger::fuelle_gruppe_aus(const QList<ErzeugerElement*>& liste)
{
	bool erg = true;
	
	ErzeugerElement *ziel = finde_element_mit_wenigsten_moeglichkeiten(liste);
	
	while (ziel != 0)
	{
		if (ziel->anzahl_moeglichkeiten() > 0) ziel->setze_moeglichkeit(Random::random(0, ziel->anzahl_moeglichkeiten() - 1));
		
		else
		{
			erg = false;
			
			break;
		}
		
		ziel = finde_element_mit_wenigsten_moeglichkeiten(liste);
	}
	
	return erg;
}


QString SudokuErzeuger::zuString()
{
	QString erg;
	
    for (int idx_y = 1; idx_y <= 9; idx_y++)
	{
        for (int idx_x = 1; idx_x <= 9; idx_x++)
		{
			ErzeugerElement *ziel = gesamtliste.value(Position(idx_x, idx_y));
			
			if (ziel->ist_ausgefuellt() == true) erg += QString::number(ziel->zahl());
			else erg += "X";
		}
	}
	
	return erg;
}


void SudokuErzeuger::verdecke_elemente(QList<ErzeugerElement*>& liste)
{
	QList<int> hilfsliste;
	hilfsliste.append(0);
	hilfsliste.append(1);
	hilfsliste.append(2);
	hilfsliste.append(3);
	hilfsliste.append(4);
	hilfsliste.append(5);
	hilfsliste.append(6);
	hilfsliste.append(7);
	hilfsliste.append(8);
	
	// je nach schwierigkeitsstufe einige elemente wieder verdecken
    for (int idx = 0; idx < (9 - freigeben) && hilfsliste.isEmpty() == false; idx++)
	{
		int auswahl = hilfsliste.at(Random::random(0, hilfsliste.size() - 1));
		
		liste.at(auswahl)->loesche_zahl();
		
		hilfsliste.removeAll(auswahl);
	}
}


void SudokuErzeuger::reset()
{
	// zunaechst alle elemente zuruecksetzen
    for (int idx_y = 1; idx_y <= 9; idx_y++)
	{
        for (int idx_x = 1; idx_x <= 9; idx_x++)
		{
			gesamtliste.value(Position(idx_x, idx_y))->reset();
		}
	}
}


void SudokuErzeuger::neues_mittleres_spiel()
{
	freigeben = 3;
	
	verarbeitung();
}


void SudokuErzeuger::initialisiere_zufallsgenerator()
{
	// den zufallsgenerator initialisieren
	Random::initialisiere();
}
