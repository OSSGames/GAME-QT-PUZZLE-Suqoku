#include "ErzeugerElement.h"
#include "SudokuErzeuger.h"
#include <QDebug>

using namespace std;

ErzeugerElement::ErzeugerElement(SudokuErzeuger* parent) : QObject(parent), erzeuger(parent), Zahl(0)
{
	// zunaechst ist alles moeglich
	for (int idx = 1; idx <= 9; idx++) moeglichkeiten.insert(idx, true);
}


ErzeugerElement::~ErzeugerElement()
{
}


int ErzeugerElement::anzahl_moeglichkeiten() const
{
	return moeglichkeiten.keys(true).size();
}


bool ErzeugerElement::ist_ausgefuellt() const
{
	bool erg = false;
	
	if (Zahl != 0) erg = true;
	
	return erg;
}


bool ErzeugerElement::ist_moeglich(int wert) const
{
	bool erg = false;
	
	if (moeglichkeiten.contains(wert) == true) erg = moeglichkeiten.value(wert);
	
	else qDebug() << tr("Error in \"bool ErzeugerElement::ist_moeglich(int wert) const\"");
	
	return erg;
}


void ErzeugerElement::setze_zahl(int wert)
{
	if (wert >= 1 && wert <= 9 && ist_ausgefuellt() == false && moeglichkeiten.value(wert) == true)
	{
		Zahl = wert;
		
		erzeuger->reaktion_auf_zahlsetzung(this);
	}
	
	else qDebug() << tr("Error in \"void ErzeugerElement::setze_zahl(int wert) const\"");
}


int ErzeugerElement::zahl() const
{
	return Zahl;
}


void ErzeugerElement::setze_moeglichkeit(int wert, bool zustand)
{
	if (moeglichkeiten.contains(wert) == true) moeglichkeiten.insert(wert, zustand);
	
	else qDebug() << tr("Error in \"void ErzeugerElement::setze_moeglichkeit(int, bool)\"");
}


QList<int> ErzeugerElement::moegliche_zahlen() const
{
	return moeglichkeiten.keys(true);
}


void ErzeugerElement::setze_moeglichkeit(int idx)
{
	QList<int> schluessel(moeglichkeiten.keys(true));
	
	if (idx < schluessel.size() && idx >= 0) setze_zahl(schluessel.at(idx));
	
	else qDebug() << tr("Error in \"void ErzeugerElement::setze_moeglichkeit(int idx)\"");
}


void ErzeugerElement::loesche_zahl()
{
	Zahl = 0;
}


void ErzeugerElement::reset()
{
	Zahl = 0;
	
	for (int idx = 1; idx <= 9; idx++) moeglichkeiten.insert(idx, true);
}
