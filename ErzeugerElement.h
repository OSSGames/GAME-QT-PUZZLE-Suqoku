/*
 
 Name: Suqoku
 Autor: Andreas Konarski
 Lizenz: GPL v3 or later
 Plattformen: Alle Systeme, auf denen QT 4.6 verfuegbar ist. Kompatibel mit QT 5.0.0.
 
 Kontakt: programmieren@konarski-wuppertal.de
 home: www.konarski-wuppertal.de
 
 Falls ich mit diesem Programm die Rechte von irgend jemand verletzen sollte, bitte ich um einen Hinweis. Wenn dies Tatsaechlich der Fall ist, entferne ich es schnellstmoeglich von meiner Homepage.
 
 */

#ifndef ERZEUGERELEMENT_H
#define ERZEUGERELEMENT_H

#include <QObject>
#include <QMap>
#include <QList>

class SudokuErzeuger;

class ErzeugerElement : public QObject
{
	Q_OBJECT
	
public:
	ErzeugerElement(SudokuErzeuger* parent);
	virtual ~ErzeugerElement();
	
	int anzahl_moeglichkeiten() const;
	bool ist_ausgefuellt() const;
	bool ist_moeglich(int) const;
	void setze_zahl(int);
	int zahl() const;
	void setze_moeglichkeit(int, bool);
	QList<int> moegliche_zahlen() const;
	void setze_moeglichkeit(int);
	void loesche_zahl();
	void reset();
	
private:
	SudokuErzeuger *erzeuger;
	
	QMap<int, bool> moeglichkeiten;
	int Zahl;
};

#endif
