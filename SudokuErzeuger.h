/*
 
 Name: Suqoku
 Autor: Andreas Konarski
 Lizenz: GPL v3 or later
 Plattformen: Alle Systeme, auf denen QT 4.6 verfuegbar ist. Kompatibel mit QT 5.0.0.
 
 Kontakt: programmieren@konarski-wuppertal.de
 home: www.konarski-wuppertal.de
 
 Falls ich mit diesem Programm die Rechte von irgend jemand verletzen sollte, bitte ich um einen Hinweis. Wenn dies Tatsaechlich der Fall ist, entferne ich es schnellstmoeglich von meiner Homepage.
 
 */

#ifndef SUDOKUERZEUGER_H
#define SUDOKUERZEUGER_H

#include <QObject>
#include <QMap>
#include <QList>
#include "Position.h"

class SudokuElement;
class ErzeugerElement;

class SudokuErzeuger : public QObject
{
	Q_OBJECT
	
public:
	// die klasse soll in verbindung mit einem thread benutzt werden. deshalb darf es kein elternobjekt haben. dadurch muss es jedoch (nach beenden des threads) manuell geloescht werden !!!
	SudokuErzeuger();
	virtual ~SudokuErzeuger();
	
	void reaktion_auf_zahlsetzung(ErzeugerElement*);
	QString zuString();
	void reset();
	
	public slots:
	void neues_einfaches_spiel();
	void neues_mittleres_spiel();
	void initialisiere_zufallsgenerator();
	
signals:
	void begonnen();
	void fertig();
	void erzeugtes_raetsel(const QString&);
	
private:
	QMap<Position, ErzeugerElement*> gesamtliste;
	QList<ErzeugerElement*> gruppe1, gruppe2, gruppe3, gruppe4, gruppe5, gruppe6, gruppe7, gruppe8, gruppe9;
	int freigeben;
	
	void verarbeitung();
	void baue_liste_auf(int x, int y, QList<ErzeugerElement*>&) const;
	ErzeugerElement* finde_element_mit_wenigsten_moeglichkeiten(const QList<ErzeugerElement*>&);
	bool fuelle_gruppe_aus(const QList<ErzeugerElement*>&);
	void verdecke_elemente(QList<ErzeugerElement*>&);
};

#endif
