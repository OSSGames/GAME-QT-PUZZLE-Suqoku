/*
 
 Name: Suqoku
 Autor: Andreas Konarski
 Lizenz: GPL v3 or later
 Plattformen: Alle Systeme, auf denen QT 4.6 verfuegbar ist. Kompatibel mit QT 5.0.0.
 
 Kontakt: programmieren@konarski-wuppertal.de
 home: www.konarski-wuppertal.de
 
 Falls ich mit diesem Programm die Rechte von irgend jemand verletzen sollte, bitte ich um einen Hinweis. Wenn dies Tatsaechlich der Fall ist, entferne ich es schnellstmoeglich von meiner Homepage.
 
 */

#ifndef SCENE_H
#define SCENE_H

#define SCENE_FILTER_NICHTS 0
#define SCENE_FILTER_1 1
#define SCENE_FILTER_2 2
#define SCENE_FILTER_3 3
#define SCENE_FILTER_4 4
#define SCENE_FILTER_5 5
#define SCENE_FILTER_6 6
#define SCENE_FILTER_7 7
#define SCENE_FILTER_8 8
#define SCENE_FILTER_9 9
#define SCENE_FILTER_RED 10
#define SCENE_FILTER_GREEN 11
#define SCENE_FILTER_BLUE 12
#define SCENE_FILTER_CYAN 13
#define SCENE_FILTER_MAGENTA 14
#define SCENE_FILTER_YELLOW 15
#define SCENE_FILTER_PAARE 16
#define SCENE_FILTER_DREIER 17
#define SCENE_MAX_FILTER 17

#include <QGraphicsScene>
#include <QMap>
#include <QByteArray>
#include <QDateTime>
#include "Position.h"

class SudokuElement;
class Undo;
class SudokuErzeuger;
class QThread;

class Scene : public QGraphicsScene
{
	Q_OBJECT
	
public:
	// konstruktor und destroktor
	Scene(QObject *parent = 0);
	virtual ~Scene();
	
	// informationen ueber elemente abfragen und setzen
	int zahl(const Position&) const;
	bool setze_zahl(const Position&, int);
	void setze_zahl_zurueck(const Position&);
	int tip(const Position&) const;
	bool setze_tip(const Position&, int, bool senden = true);
	void setze_tip_zurueck(const Position&, bool senden = true);
	int zahl_und_tip(const Position&) const;
	bool zahl_bekannt(const Position&) const;
	bool ist_zahl(const Position&) const;
	bool ist_tip(const Position&) const;
	bool tip_verfuegbar(const Position&, int) const;
	void setze_verfuegbarkeit_tip(const Position&, int, bool);
	void setze_verfuegbarkeit_tip_zurueck(const Position&);
	bool tip_ausgeschlossen(const Position&, int) const;
	void setze_ausschluss_tip(const Position&, int, bool, bool senden = true);
	int farbmarkierung(const Position&) const;
	void setze_farbmarkierung(const Position&, int);
	void aktualisiere_verfuegbarkeiten(SudokuElement*, int);
	void setze_verfuegbarkeiten_zurueck(SudokuElement*, int);
	const Position position(SudokuElement*) const;
	SudokuElement *element(const Position&) const;
	QList<int> zahlen_und_tips_in_reihe(SudokuElement*) const;
	QList<int> zahlen_und_tips_in_spalte(SudokuElement*) const;
	QList<int> zahlen_und_tips_in_gruppe(SudokuElement*) const;
	bool enthaelt(SudokuElement*) const;
	bool enthaelt(const Position&) const;
	
	// informationen ueber das spiel verarbeiten
	int anzahl_ausgefuellte_zellen() const;
	int anzahl_unausgefuellte_zellen() const;
	int gefilltert() const;
	void filtere(int);
	void verarbeite_auswahl(SudokuElement*);
	const QString& raetsel() const;
	int spieldauer() const;
	bool spiel_gewonnen() const;
	bool laufendes_spiel() const;
	
	// einstellungen an der scene und dem spiel insgesamt vornehemen
	void reset();
	bool raetsel_annehmen(const QString&);
	void restart();
	void mache_raetsel_aus_zustand();
	void loese_alle_einzelnen();
	void neues_leichtes_zufallsspiel();
	void neues_mittleres_spiel();
	
public slots:
	// slots, die die scene und das spiel insgesamt betreffen
	void zeige_kandidaten(bool);
	void rueckgaengig();
	void wiederherstellen();
	
	// einstellungen an der scene und dem spiel insgesamt vornehemen
	void annahme_raetsel(const QString&);
	
signals:
	// signale, die ueber den zustand der scene, bzw. des spiels insgesamt informieren
	void gewonnen();
	void spielstand(int, int);
	void filtere_wert(int);
	void scene_resettet();
	void laden_fehlgeschlagen();
	void loese_einzelne();
	void speichern_verfuegbar(bool);
	void anzeige_kandidaten(bool);
	void rueckgaengig_verfuegbar(bool);
	void wiederherstellen_verfuegbar(bool);
	void anzahl_zuege(int);
	void restart_verfuegbar(bool);
	void erzeugung_begonnen();
	void erzeugung_fertig();
	
	// signale, die gezielt ueber aktivitaeten einzelner elemente informieren
	void tipp_abgegeben(SudokuElement*, int);
	void tipp_zurueckgenommen(SudokuElement*, int);
	void ausgewaehlt(SudokuElement*);
	void zahl_gesetzt(SudokuElement*, int);
	void zahl_zurueckgesetzt(SudokuElement*, int);
	void kandidat_gesperrt(SudokuElement*, int);
	void kandidat_entsperrt(SudokuElement*, int);
	
	// signale, die den erzeuger steuern
	void initialisiere_zufallsgenerator();
	void erzeuge_einfaches_spiel();
	void erzeuge_mittleres_spiel();
	
private:
	QMap<Position, SudokuElement*> gesamtliste, gruppe1, gruppe2, gruppe3, gruppe4, gruppe5, gruppe6, gruppe7, gruppe8, gruppe9;
	QList<QMap<Position, SudokuElement*>*> gruppenliste;
	QList<SudokuElement*> unausgefuellte, ausgefuellte;
	int Gefiltert;
	SudokuElement *ausgewaehltes_element;
	QString Spielstring;
	QDateTime Startzeitpunkt;
	Undo *undo;
	bool schutz_spielstring;
	SudokuErzeuger *erzeuger;
	QThread *erzeugerthread;
	
	// private hilfsfunktionen der scene
	void baue_liste_auf(int x, int y, QMap<Position, SudokuElement*>&) const;
};

#endif
