/*
 
 Name: Suqoku
 Autor: Andreas Konarski
 Lizenz: GPL v3 or later
 Plattformen: Alle Systeme, auf denen QT 4.6 verfuegbar ist. Kompatibel mit QT 5.0.0.
 
 Kontakt: programmieren@konarski-wuppertal.de
 home: www.konarski-wuppertal.de
 
 Falls ich mit diesem Programm die Rechte von irgend jemand verletzen sollte, bitte ich um einen Hinweis. Wenn dies Tatsaechlich der Fall ist, entferne ich es schnellstmoeglich von meiner Homepage.
 
 */

#ifndef SUDOKUELEMENT_H
#define SUDOKUELEMENT_H

#define ELEMENT_GROESSE 64

#include <QObject>
#include <QGraphicsItem>
#include <QMap>
#include "Position.h"

class Scene;

class SudokuElement : public QObject, public QGraphicsItem
{
	Q_OBJECT
	
public:
	SudokuElement(Scene *oparent = 0, QGraphicsItem *gparent = 0);
	virtual ~SudokuElement();
	
	Scene* meinescene() const;
	
	void registriere_gruppe(QMap<Position, SudokuElement*>*);
	QMap<Position, SudokuElement*>* gruppe() const;
	
	int zahl() const;
	int tip() const;
	bool setze_zahl(int);
	bool setze_tip(int, bool senden = true);
	void setze_tip_zurueck(bool senden = true);
	bool ist_zahl() const;
	bool ist_tip() const;
	bool zahl_bekannt() const;
	bool tip_verfuegbar(int) const;
	bool tip_ausgeschlossen(int) const;
	void setze_verfuegbarkeit_tip(int, bool);
	void setze_verfuegbarkeit_tip_zurueck();
	void setze_ausschluss_tip(int, bool, bool senden = true);
	int zahl_und_tip() const;
	void setze_zahl_zurueck();
	void reset();
	void setze_ausschluss_tip_zurueck();
	void setze_farbmarkierung(int);
	int farbmarkierung() const;
	int ist_vorausgewaehlt() const;
	void setze_vorauswahl(bool);
	bool sichtbarkeit_kandidaten() const;
	
public slots:
	void filtere(int);
	void loese_wenn_einzeln();
	void setze_sichtbarkeit_kandidaten(bool);
	
signals:
	void tipp_abgegeben(SudokuElement*, int);
	void tipp_zurueckgenommen(SudokuElement*, int);
	void ausgewaehlt(SudokuElement*);
	void zahl_gesetzt(SudokuElement*, int);
	void zahl_zurueckgesetzt(SudokuElement*, int);
	void kandidat_gesperrt(SudokuElement*, int);
	void kandidat_entsperrt(SudokuElement*, int);
	
private:
	Scene *Meinescene;
	QRectF aussenring, ring1, ring2, ring3, ring4, ring5, ring6, ring7, ring8, ring9;
	int Zahl, Tip, Filterung, Aktiver_farbmarker;
	QMap<int, bool> verfuegbarkeit;
	QMap<int, bool> ausgeschlossen;
	QMap<Position, SudokuElement*> *meine_gruppe;
	bool vormals_ausgewaehlt, Sichtbarkeit_kandidaten;
	
	virtual void paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*);
	virtual QRectF boundingRect() const;
	virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent*);
	virtual void keyPressEvent(QKeyEvent*);
	virtual void focusInEvent(QFocusEvent*);
	virtual void focusOutEvent(QFocusEvent*);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent*);
	virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent*);
};

#endif
