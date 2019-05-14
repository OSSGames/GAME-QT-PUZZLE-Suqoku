#include "Undo.h"
#include "Scene.h"
#include "SudokuElement.h"
#include <QDebug>

using namespace std;

Undo::Undo(Scene *parent) : QObject(parent), scene(parent), Momentane_position(0)
{
}


Undo::~Undo()
{
}


void Undo::reset()
{
	elementliste.clear();
	Momentane_position = 0;
	
	emit rueckgaengig_verfuegbar(false);
	emit wiederherstellen_verfuegbar(false);
	emit anzahl_zuege(0);
	emit restart_verfuegbar(false);
}


void Undo::tipp_abgegeben(SudokuElement* element, int zahl)
{
	while (elementliste.isEmpty() == false && Momentane_position < elementliste.size()) elementliste.removeLast();
	
	elementliste.append(Undo_element(UNDO_TIPP, scene->position(element), zahl, true));
	Momentane_position++;
	
	if (scene->anzahl_unausgefuellte_zellen() > 0) emit rueckgaengig_verfuegbar(true);
	else emit rueckgaengig_verfuegbar(false);
	
	emit wiederherstellen_verfuegbar(false);
	emit anzahl_zuege(Momentane_position);
	emit restart_verfuegbar(true);
}


void Undo::tipp_zurueckgenommen(SudokuElement* element, int zahl)
{
	while (elementliste.isEmpty() == false && Momentane_position < elementliste.size()) elementliste.removeLast();
	
	elementliste.append(Undo_element(UNDO_TIPP, scene->position(element), zahl, false));
	Momentane_position++;
	
	emit rueckgaengig_verfuegbar(true);
	emit wiederherstellen_verfuegbar(false);
	emit anzahl_zuege(Momentane_position);
	emit restart_verfuegbar(true);
}


void Undo::kandidat_gesperrt(SudokuElement* element, int zahl)
{
	while (elementliste.isEmpty() == false && Momentane_position < elementliste.size()) elementliste.removeLast();
	
	elementliste.append(Undo_element(UNDO_KANDIDAT, scene->position(element), zahl, true));
	Momentane_position++;
	
	emit rueckgaengig_verfuegbar(true);
	emit wiederherstellen_verfuegbar(false);
	emit anzahl_zuege(Momentane_position);
	emit restart_verfuegbar(true);
}


void Undo::kandidat_entsperrt(SudokuElement* element, int zahl)
{
	while (elementliste.isEmpty() == false && Momentane_position < elementliste.size()) elementliste.removeLast();
	
	elementliste.append(Undo_element(UNDO_KANDIDAT, scene->position(element), zahl, false));
	Momentane_position++;
	
	emit rueckgaengig_verfuegbar(true);
	emit wiederherstellen_verfuegbar(false);
	emit anzahl_zuege(Momentane_position);
	emit restart_verfuegbar(true);
}


int Undo::size() const
{
	return elementliste.size();
}


void Undo::rueckgaengig()
{
	if (Momentane_position > 0 && elementliste.isEmpty() == false)
	{
		Momentane_position--;
		
		int zahl = elementliste.at(Momentane_position).get_zahl();
		bool aktivitaet = elementliste.at(Momentane_position).get_aktivitaet();
		
		switch (elementliste.at(Momentane_position).get_art())
		{
			case UNDO_TIPP:
				
				if (aktivitaet == true) scene->setze_tip_zurueck(elementliste.at(Momentane_position).get_position(), false);
				else scene->setze_tip(elementliste.at(Momentane_position).get_position(), zahl, false);
				
				break;
				
			case UNDO_KANDIDAT:
				
				scene->setze_ausschluss_tip(elementliste.at(Momentane_position).get_position(), zahl, !aktivitaet, false);
				
				break;
				
			default:
				
				qDebug() << tr("Unknown kind of move in \"void Undo::rueckgaengig()\"");
				
				break;
		}
		
		if (Momentane_position > 0) emit rueckgaengig_verfuegbar(true);
		else emit rueckgaengig_verfuegbar(false);
		
		emit wiederherstellen_verfuegbar(true);
		emit anzahl_zuege(Momentane_position);
	}
}


void Undo::wiederherstellen()
{
	if (Momentane_position < elementliste.size())
	{
		int zahl = elementliste.at(Momentane_position).get_zahl();
		bool aktivitaet = elementliste.at(Momentane_position).get_aktivitaet();
		
		switch (elementliste.at(Momentane_position).get_art())
		{
			case UNDO_TIPP:
				
				if (aktivitaet == false) scene->setze_tip_zurueck(elementliste.at(Momentane_position).get_position(), false);
				else scene->setze_tip(elementliste.at(Momentane_position).get_position(), zahl, false);
				
				break;
				
			case UNDO_KANDIDAT:
				
				scene->setze_ausschluss_tip(elementliste.at(Momentane_position).get_position(), zahl, aktivitaet, false);
				
				break;
				
			default:
				
				qDebug() << tr("Unknown kind of move in \"void Undo::wiederherstellen()\"");
				
				break;
		}
		
		Momentane_position++;
		
		emit rueckgaengig_verfuegbar(true);
		
		if (Momentane_position < elementliste.size()) emit wiederherstellen_verfuegbar(true);
		else emit wiederherstellen_verfuegbar(false);
		
		emit anzahl_zuege(Momentane_position);
	}
}
