/*
 
 Name: Suqoku
 Autor: Andreas Konarski
 Lizenz: GPL v3 or later
 Plattformen: Alle Systeme, auf denen QT 4.6 verfuegbar ist. Kompatibel mit QT 5.0.0.
 
 Kontakt: programmieren@konarski-wuppertal.de
 home: www.konarski-wuppertal.de
 
 Falls ich mit diesem Programm die Rechte von irgend jemand verletzen sollte, bitte ich um einen Hinweis. Wenn dies Tatsaechlich der Fall ist, entferne ich es schnellstmoeglich von meiner Homepage.
 
 */

#ifndef UNDO_ELEMENT_H
#define UNDO_ELEMENT_H

#define UNDO_TIPP 1
#define UNDO_KANDIDAT 2

#include "Position.h"

class Undo_element
{
public:
	Undo_element(int art_, const Position& position_, int zahl_, bool aktivitaet_);
	virtual ~Undo_element();
	
	int get_art() const;
	const Position& get_position() const;
	int get_zahl() const;
	bool get_aktivitaet() const;
	
private:
	int art;
	Position position;
	int zahl;
	bool aktivitaet;
};

#endif
