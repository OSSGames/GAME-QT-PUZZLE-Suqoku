/*
 
 Name: Suqoku
 Autor: Andreas Konarski
 Lizenz: GPL v3 or later
 Plattformen: Alle Systeme, auf denen QT 4.6 verfuegbar ist. Kompatibel mit QT 5.0.0.
 
 Kontakt: programmieren@konarski-wuppertal.de
 home: www.konarski-wuppertal.de
 
 Falls ich mit diesem Programm die Rechte von irgend jemand verletzen sollte, bitte ich um einen Hinweis. Wenn dies Tatsaechlich der Fall ist, entferne ich es schnellstmoeglich von meiner Homepage.
 
 */

#ifndef UNDO_H
#define UNDO_H

#include <QObject>
#include <QList>
#include "Undo_element.h"

class Scene;
class SudokuElement;

class Undo : public QObject
{
	Q_OBJECT
	
public:
	Undo(Scene *parent);
	virtual ~Undo();
	
	void reset();
	int size() const;
	void rueckgaengig();
	void wiederherstellen();
	
public slots:
	void tipp_abgegeben(SudokuElement*, int);
	void tipp_zurueckgenommen(SudokuElement*, int);
	void kandidat_gesperrt(SudokuElement*, int);
	void kandidat_entsperrt(SudokuElement*, int);
	
signals:
	void rueckgaengig_verfuegbar(bool);
	void wiederherstellen_verfuegbar(bool);
	void anzahl_zuege(int);
	void restart_verfuegbar(bool);
	
private:
	QList<Undo_element> elementliste;
	Scene *scene;
	int Momentane_position;
};

#endif
