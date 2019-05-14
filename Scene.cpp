#include "Scene.h"
#include "SudokuElement.h"
#include "Undo.h"
#include "SudokuErzeuger.h"
#include <QDebug>
#include <QThread>

#define GROESSE 576

using namespace std;

// konstruktor und destroktor
Scene::Scene(QObject *parent) : QGraphicsScene(parent), Gefiltert(0), ausgewaehltes_element(0), Spielstring("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"), Startzeitpunkt(QDateTime::currentDateTime()), schutz_spielstring(false), erzeuger(0)
{
	// die klasse Position fuer das signal - slot system registrieren
	qRegisterMetaType<Position>("Position");
	
	// die elemente aufbauen
    for (int idx2 = 1; idx2 <= 9; idx2++)
	{
        for (int idx1 = 1; idx1 <= 9; idx1++)
		{
			// element erzeugen
			SudokuElement *element = new SudokuElement(this);
			
			// seine position setzen
			element->setPos((idx1 - 1) * ELEMENT_GROESSE, (idx2 - 1) * ELEMENT_GROESSE);
			
			// den objekt namen setzen
			element->setObjectName(Position(idx1, idx2).toString());
			
			// das element in die scene einfuegen
			addItem(element);
			
			gesamtliste.insert(Position(idx1, idx2), element);
			
			// das element auch in die liste der unausgefuellten elemente einfuegen
			unausgefuellte.append(element);
			
			// signal slot verbindungen, die das element betreffen
			// filter ermoeglichen
			connect(this, SIGNAL(filtere_wert(int)), element, SLOT(filtere(int)));
			
			// einzelne loesen ermoeglichen
			connect(this, SIGNAL(loese_einzelne()), element, SLOT(loese_wenn_einzeln()));
			
			// das steuern der sichtbarkeit der kandidaten ermoeglichen
			connect(this, SIGNAL(anzeige_kandidaten(bool)), element, SLOT(setze_sichtbarkeit_kandidaten(bool)));
			
			// durchverbinden von signalen der elemente
			connect(element, SIGNAL(tipp_abgegeben(SudokuElement*, int)), this, SIGNAL(tipp_abgegeben(SudokuElement*,int)));
			connect(element, SIGNAL(tipp_zurueckgenommen(SudokuElement*, int)), this, SIGNAL(tipp_zurueckgenommen(SudokuElement*, int)));
			connect(element, SIGNAL(ausgewaehlt(SudokuElement*)), this, SIGNAL(ausgewaehlt(SudokuElement*)));
			connect(element, SIGNAL(zahl_gesetzt(SudokuElement*, int)), this, SIGNAL(zahl_gesetzt(SudokuElement*, int)));
			connect(element, SIGNAL(zahl_zurueckgesetzt(SudokuElement*, int)), this, SIGNAL(zahl_zurueckgesetzt(SudokuElement*, int)));
			connect(element, SIGNAL(kandidat_gesperrt(SudokuElement*, int)), this, SIGNAL(kandidat_gesperrt(SudokuElement*, int)));
			connect(element, SIGNAL(kandidat_entsperrt(SudokuElement*, int)), this, SIGNAL(kandidat_entsperrt(SudokuElement*, int)));
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
	
	// die gruppenliste aufbauen
	gruppenliste.append(&gruppe1);
	gruppenliste.append(&gruppe2);
	gruppenliste.append(&gruppe3);
	gruppenliste.append(&gruppe4);
	gruppenliste.append(&gruppe5);
	gruppenliste.append(&gruppe6);
	gruppenliste.append(&gruppe7);
	gruppenliste.append(&gruppe8);
	gruppenliste.append(&gruppe9);
	
	// die linien einfuegen
	QPen linienstift;
	linienstift.setColor(Qt::black);
	linienstift.setWidth(3);
	
	QLineF linie1(QPointF(ELEMENT_GROESSE * 3.0, 0), QPointF(ELEMENT_GROESSE * 3.0, GROESSE));
	addLine(linie1, linienstift);
	
	QLineF linie2(QPointF(ELEMENT_GROESSE * 6.0, 0), QPointF(ELEMENT_GROESSE * 6.0, GROESSE));
	addLine(linie2, linienstift);
	
	QLineF linie3(QPointF(0, ELEMENT_GROESSE * 3.0), QPointF(GROESSE, ELEMENT_GROESSE * 3.0));
	addLine(linie3, linienstift);
	
	QLineF linie4(QPointF(0, ELEMENT_GROESSE * 6.0), QPointF(GROESSE, ELEMENT_GROESSE * 6.0));
	addLine(linie4, linienstift);
	
	QLineF linie5(QPointF(linienstift.width() / 2.0, linienstift.width() / 2.0), QPointF(GROESSE - (linienstift.width() / 2.0), linienstift.width() / 2.0));
	addLine(linie5, linienstift);
	
	QLineF linie6(QPointF(linienstift.width() / 2.0, GROESSE - (linienstift.width() / 2.0)), QPointF(GROESSE - (linienstift.width() / 2.0), GROESSE - (linienstift.width() / 2.0)));
	addLine(linie6, linienstift);
	
	QLineF linie7(QPointF(linienstift.width() / 2.0, linienstift.width() / 2.0), QPointF(linienstift.width() / 2.0, GROESSE - (linienstift.width() / 2.0)));
	addLine(linie7, linienstift);
	
	QLineF linie8(QPointF(GROESSE - (linienstift.width() / 2.0), linienstift.width() / 2.0), QPointF(GROESSE - (linienstift.width() / 2.0), GROESSE - (linienstift.width() / 2.0)));
	addLine(linie8, linienstift);
	
	// undo erzeugen
	undo = new Undo(this);
	
	// den thread fuer den erzeuger erstellen
	erzeugerthread = new QThread(this);
	
	// erzeuger aufbauen
	erzeuger = new SudokuErzeuger();
	erzeuger->moveToThread(erzeugerthread);
	
	erzeugerthread->start();
	
	// signal - slot verbindungen
	// verbindungen fuer undo
	connect(this, SIGNAL(tipp_abgegeben(SudokuElement*, int)), undo, SLOT(tipp_abgegeben(SudokuElement*, int)));
	connect(this, SIGNAL(tipp_zurueckgenommen(SudokuElement*, int)), undo, SLOT(tipp_zurueckgenommen(SudokuElement*, int)));
	connect(this, SIGNAL(kandidat_gesperrt(SudokuElement*, int)), undo, SLOT(kandidat_gesperrt(SudokuElement*, int)));
	connect(this, SIGNAL(kandidat_entsperrt(SudokuElement*, int)), undo, SLOT(kandidat_entsperrt(SudokuElement*, int)));
	connect(undo, SIGNAL(rueckgaengig_verfuegbar(bool)), this, SIGNAL(rueckgaengig_verfuegbar(bool)));
	connect(undo, SIGNAL(wiederherstellen_verfuegbar(bool)), this, SIGNAL(wiederherstellen_verfuegbar(bool)));
	connect(undo, SIGNAL(anzahl_zuege(int)), this, SIGNAL(anzahl_zuege(int)));
	connect(undo, SIGNAL(restart_verfuegbar(bool)), this, SIGNAL(restart_verfuegbar(bool)));
	
	// verbindungen fuer erzeuger
	connect(erzeuger, SIGNAL(begonnen()), this, SIGNAL(erzeugung_begonnen()));
	connect(erzeuger, SIGNAL(fertig()), this, SIGNAL(erzeugung_fertig()));
	connect(this, SIGNAL(erzeuge_einfaches_spiel()), erzeuger, SLOT(neues_einfaches_spiel()));
	connect(this, SIGNAL(erzeuge_mittleres_spiel()), erzeuger, SLOT(neues_mittleres_spiel()));
	connect(erzeuger, SIGNAL(erzeugtes_raetsel(const QString&)), this, SLOT(annahme_raetsel(const QString&)));
	connect(this, SIGNAL(initialisiere_zufallsgenerator()), erzeuger, SLOT(initialisiere_zufallsgenerator()));
	
	// den zufallsgenerator des erzeugers initialisieren
	emit initialisiere_zufallsgenerator();
}


Scene::~Scene()
{
	erzeugerthread->quit();
	erzeugerthread->wait();
	
	delete erzeuger;
}


// private hilfsfunktionen der scene
void Scene::baue_liste_auf(int x, int y, QMap<Position, SudokuElement*>& erg) const
{
    for (int idx2a = x, idx2b = 1; idx2a < (x + 3); idx2a++, idx2b++)
	{
        for (int idx1a = y, idx1b = 1; idx1a < (y + 3); idx1a++, idx1b++)
		{
			SudokuElement *element = gesamtliste.value(Position(idx1a, idx2a));
			
			// dem element einen zeiger auf seine gruppe uebergeben
			element->registriere_gruppe(&erg);
			
			erg.insert(Position(idx1b, idx2b), element);
		}
	}
}


// informationen ueber elemente abfragen und setzen
int Scene::zahl(const Position& position) const
{
	int erg = 0;
	
	if (gesamtliste.contains(position)) erg = gesamtliste.value(position)->zahl();
	
	else qDebug() << tr("Error in \"int Scene::zahl(const Position& position) const\"");
	
	return erg;
}


bool Scene::setze_zahl(const Position& position, int zahl)
{
	bool erg = false;
	
	if (gesamtliste.contains(position)) erg = gesamtliste.value(position)->setze_zahl(zahl);
	
	else qDebug() << tr("Error in \"bool Scene::setze_zahl(const Position& position, int zahl)\"");
	
	return erg;
}


void Scene::setze_zahl_zurueck(const Position& position)
{
	if (gesamtliste.contains(position)) gesamtliste.value(position)->setze_zahl_zurueck();
	
	else qDebug() << tr("Error in \"void Scene::setze_zahl_zurueck(const Position& position)\"");
}


int Scene::tip(const Position& position) const
{
	int erg = 0;
	
	if (gesamtliste.contains(position)) erg = gesamtliste.value(position)->tip();
	
	else qDebug() << tr("Error in \"int Scene::tip(const Position& position) const\"");
	
	return erg;
}


bool Scene::setze_tip(const Position& position, int zahl, bool senden)
{
	bool erg = false;
	
	if (gesamtliste.contains(position)) erg = gesamtliste.value(position)->setze_tip(zahl, senden);
	
	else qDebug() << tr("Error in \"void Scene::setze_tip(const Position& position, int zahl)\"");
	
	return erg;
}


void Scene::setze_tip_zurueck(const Position& position, bool senden)
{
	if (gesamtliste.contains(position)) gesamtliste.value(position)->setze_tip_zurueck(senden);
	
	else qDebug() << tr("Error in \"void Scene::setze_tip_zurueck(const Position& position, bool senden)\"");
}


int Scene::zahl_und_tip(const Position& position) const
{
	int erg = 0;
	
	if (gesamtliste.contains(position)) erg = gesamtliste.value(position)->zahl_und_tip();
	
	else qDebug() << tr("Error in \"int Scene::zahl_und_tip(const Position& position) const\"");
	
	return erg;
}


bool Scene::zahl_bekannt(const Position& position) const
{
	bool erg = false;
	
	if (gesamtliste.contains(position)) erg = gesamtliste.value(position)->zahl_bekannt();
	
	else qDebug() << tr("Error in \"bool Scene::zahl_bekannt(const Position& position) const\"");
	
	return erg;
}


bool Scene::ist_zahl(const Position& position) const
{
	bool erg = false;
	
	if (gesamtliste.contains(position)) erg = gesamtliste.value(position)->ist_zahl();
	
	else qDebug() << tr("Error in \"bool Scene::ist_zahl(const Position& position) const\"");
	
	return erg;
}


bool Scene::ist_tip(const Position& position) const
{
	bool erg = false;
	
	if (gesamtliste.contains(position)) erg = gesamtliste.value(position)->ist_tip();
	
	else qDebug() << tr("Error in \"bool Scene::ist_tip(const Position& position) const\"");
	
	return erg;
}


bool Scene::tip_verfuegbar(const Position& position, int zahl_) const
{
	bool erg = false;
	
	if (gesamtliste.contains(position)) erg = gesamtliste.value(position)->tip_verfuegbar(zahl_);
	
	else qDebug() << tr("Error in \"bool Scene::tip_verfuegbar(const Position& position, int zahl_) const\"");
	
	return erg;
}


void Scene::setze_verfuegbarkeit_tip(const Position& position, int zahl_, bool zustand)
{
	if (gesamtliste.contains(position)) gesamtliste.value(position)->setze_verfuegbarkeit_tip(zahl_, zustand);
	
	else qDebug() << tr("Error in \"void Scene::setze_verfuegbarkeit_tip(const Position& position, int zahl_, bool zustand)\"");
}


void Scene::setze_verfuegbarkeit_tip_zurueck(const Position& position)
{
	if (gesamtliste.contains(position)) gesamtliste.value(position)->setze_verfuegbarkeit_tip_zurueck();
	
	else qDebug() << tr("Error in \"void Scene::setze_verfuegbarkeit_tip_zurueck(const Position& position)\"");
}


bool Scene::tip_ausgeschlossen(const Position& position, int zahl_) const
{
	bool erg = false;
	
	if (gesamtliste.contains(position)) erg = gesamtliste.value(position)->tip_ausgeschlossen(zahl_);
	
	else qDebug() << tr("Error in \"bool Scene::tip_ausgeschlossen(const Position& position, int zahl_) const\"");
	
	return erg;
}


void Scene::setze_ausschluss_tip(const Position& position, int zahl_, bool zustand, bool senden)
{
	if (gesamtliste.contains(position)) gesamtliste.value(position)->setze_ausschluss_tip(zahl_, zustand, senden);
	
	else qDebug() << tr("Error in \"void Scene::setze_ausschluss_tip(const Position& position, int zahl_, bool zustand)\"");
}


int Scene::farbmarkierung(const Position& position) const
{
	int erg = 0;
	
	if (gesamtliste.contains(position)) erg = gesamtliste.value(position)->farbmarkierung();
	
	else qDebug() << tr("Error in \"int Scene::farbmarkierung(const Position& position) const\"");
	
	return erg;
}


void Scene::setze_farbmarkierung(const Position& position, int wert)
{
	if (gesamtliste.contains(position)) gesamtliste.value(position)->setze_farbmarkierung(wert);
	
	else qDebug() << tr("Error in \"void Scene::setze_farbmarkierung(const Position& position, int wert)\"");
}


void Scene::aktualisiere_verfuegbarkeiten(SudokuElement* element, int tip_)
{
	// zunaechst die eigene gruppe des elements aktualisieren
    for (int idx = 0; idx < element->gruppe()->size(); idx++)
	{
		SudokuElement *testelement = element->gruppe()->value(element->gruppe()->keys().at(idx));
		
		if (testelement != element && testelement != 0) testelement->setze_verfuegbarkeit_tip(tip_, false);
	}
	
	// die elemente, die sich in der selben reihe und spalte befinden ebenfalls aktualisieren
	int reihe = gesamtliste.key(element).x();
	int spalte = gesamtliste.key(element).y();
	
	if (reihe != 0 && spalte != 0)
	{
        for (int idx = 1; idx <= 9; idx++)
		{
			SudokuElement *zielelement_x = gesamtliste.value(Position(reihe, idx));
			
			if (zielelement_x != 0 && zielelement_x != element && zielelement_x->gruppe() != element->gruppe()) zielelement_x->setze_verfuegbarkeit_tip(tip_, false);
			
			SudokuElement *zielelement_y = gesamtliste.value(Position(idx, spalte));
			
			if (zielelement_y != 0 && zielelement_y != element && zielelement_y->gruppe() != element->gruppe()) zielelement_y->setze_verfuegbarkeit_tip(tip_, false);
		}
	}
	
	// das element aus der liste der unausgefuellten entfernen und in die liste der ausgefuellten einfuegen
	unausgefuellte.removeAll(element);
	ausgefuellte.append(element);
	
	emit spielstand(ausgefuellte.size(), unausgefuellte.size());
	
	if (unausgefuellte.isEmpty() == true) emit gewonnen();
}


void Scene::setze_verfuegbarkeiten_zurueck(SudokuElement* element, int tip_)
{
	// zunaechst die eigene gruppe des elements aktualisieren
    for (int idx = 0; idx < element->gruppe()->size(); idx++)
	{
		SudokuElement *testelement = element->gruppe()->value(element->gruppe()->keys().at(idx));
		
		if (testelement != element && testelement != 0) testelement->setze_verfuegbarkeit_tip(tip_, true);
	}
	
	// die elemente, die sich in der selben reihe und spalte befinden ebenfalls aktualisieren
	int reihe = gesamtliste.key(element).x();
	int spalte = gesamtliste.key(element).y();
	
	if (reihe != 0 && spalte != 0)
	{
        for (int idx = 1; idx <= 9; idx++)
		{
			SudokuElement *zielelement_x = gesamtliste.value(Position(reihe, idx));
			
			if (zielelement_x != 0 && zielelement_x != element && zielelement_x->gruppe() != element->gruppe()) zielelement_x->setze_verfuegbarkeit_tip(tip_, true);
			
			SudokuElement *zielelement_y = gesamtliste.value(Position(idx, spalte));
			
			if (zielelement_y != 0 && zielelement_y != element && zielelement_y->gruppe() != element->gruppe()) zielelement_y->setze_verfuegbarkeit_tip(tip_, true);
		}
	}
	
	// das element aus der liste der ausgefuellten entfernen und in die liste der unausgefuellten einfuegen
	ausgefuellte.removeAll(element);
	unausgefuellte.append(element);
	
	emit spielstand(ausgefuellte.size(), unausgefuellte.size());
}


const Position Scene::position(SudokuElement* element) const
{
	return gesamtliste.key(element);
}


SudokuElement* Scene::element(const Position& position_) const
{
	return gesamtliste.value(position_);
}


QList<int> Scene::zahlen_und_tips_in_reihe(SudokuElement* test_element) const
{
	QList<int> erg;
	
	if (enthaelt(test_element) == true)
	{
		int reihe = position(test_element).x();
		int spalte = position(test_element).y();
		
        for (int idx = 1; idx <= 9; idx++)
		{
			if (enthaelt(element(Position(reihe, idx))) == true)
			{
				SudokuElement *ziel = element(Position(reihe, idx));
				
				if ((ziel->ist_tip() == true || ziel->ist_zahl() == true) && idx != spalte) erg.append(ziel->zahl_und_tip());
			}
			
			else qDebug() << tr("Unable to find ziel in \"QList<int> Scene::zahlen_und_tips_in_reihe(SudokuElement* test_element) const\"");
		}
	}
	
	else qDebug() << tr("Unable to find test_element in \"QList<int> Scene::zahlen_und_tips_in_reihe(SudokuElement* test_element) const\"");
	
	return erg;
}


QList<int> Scene::zahlen_und_tips_in_spalte(SudokuElement* test_element) const
{
	QList<int> erg;
	
	if (enthaelt(test_element) == true)
	{
		int reihe = position(test_element).x();
		int spalte = position(test_element).y();
		
        for (int idx = 1; idx <= 9; idx++)
		{
			if (enthaelt(element(Position(idx, spalte))) == true)
			{
				SudokuElement *ziel = element(Position(idx, spalte));
				
				if ((ziel->ist_tip() == true || ziel->ist_zahl() == true) && idx != reihe) erg.append(ziel->zahl_und_tip());
			}
			
			else qDebug() << tr("Unable to find ziel in \"QList<int> Scene::zahlen_und_tips_in_spalte(SudokuElement* test_element) const\"");
		}
	}
	
	else qDebug() << tr("Unable to find test_element in \"QList<int> Scene::zahlen_und_tips_in_spalte(SudokuElement* test_element) const\"");
	
	return erg;
}


QList<int> Scene::zahlen_und_tips_in_gruppe(SudokuElement* test_element) const
{
	QList<int> erg;
	QList<Position> schluessel(test_element->gruppe()->keys());
	
    for (int idx = 0; idx < schluessel.size(); idx++)
	{
		SudokuElement *ziel = test_element->gruppe()->value(schluessel.at(idx));
		
		if (ziel->ist_tip() == true || ziel->ist_zahl() == true) erg.append(ziel->zahl_und_tip());
	}
	
	return erg;
}


bool Scene::enthaelt(SudokuElement* test_element) const
{
	return gesamtliste.values().contains(test_element);
}


bool Scene::enthaelt(const Position& test_position) const
{
	return gesamtliste.contains(test_position);
}


// informationen ueber das spiel verarbeiten
int Scene::anzahl_ausgefuellte_zellen() const
{
	return ausgefuellte.size();
}


int Scene::anzahl_unausgefuellte_zellen() const
{
	return unausgefuellte.size();
}


int Scene::gefilltert() const
{
	return Gefiltert;
}


void Scene::filtere(int zahl)
{
	if (zahl >= 0 && zahl <= SCENE_MAX_FILTER)
	{
		Gefiltert = zahl;
		
		emit filtere_wert(Gefiltert);
	}
	
	else qDebug() << tr("Unknown filter order in \"void Scene::filtere(int zahl)\"");
}


void Scene::verarbeite_auswahl(SudokuElement* element)
{
	// wenn sich die auswahl geaendert hat
	if (element != ausgewaehltes_element)
	{
		// wenn das vormals ausgewaehlte element ungleich 0 ist die vorauswahl zuruecksetzen
		if (ausgewaehltes_element != 0) ausgewaehltes_element->setze_vorauswahl(false);
		
		// das neue ausgewaehlte element speichern
		ausgewaehltes_element = element;
	}
}


const QString& Scene::raetsel() const
{
	return Spielstring;
}


int Scene::spieldauer() const
{
	return Startzeitpunkt.secsTo(QDateTime::currentDateTime());
}


bool Scene::spiel_gewonnen() const
{
	return unausgefuellte.isEmpty();
}


bool Scene::laufendes_spiel() const
{
	bool erg = false;
	
	if ((undo->size() > 0 || Spielstring != "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX") && spiel_gewonnen() == false) erg = true;
	
	return erg;
}


// einstellungen an der scene und dem spiel insgesamt vornehemen
void Scene::reset()
{
	// alle elemente, die sich in die liste der ausgefuellten elemente befinden wieder in die liste der unausgefuellten elemente ueberfuehren
	unausgefuellte += ausgefuellte;
	ausgefuellte.clear();
	
	ausgewaehltes_element = 0;
	
	emit spielstand(ausgefuellte.size(), unausgefuellte.size());
	
	QList<Position> schluessel(gesamtliste.keys());
	
    for (int idx = 0; idx < schluessel.size(); idx++) gesamtliste.value(schluessel.at(idx))->reset();
	
	if (schutz_spielstring == false) Spielstring = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
	
	Startzeitpunkt = QDateTime::currentDateTime();
	
	undo->reset();
	
	emit scene_resettet();
	emit anzahl_zuege(0);
}


bool Scene::raetsel_annehmen(const QString& raetselstring)
{
	bool erg = false;
	
	if (raetselstring.size() == 81)
	{
		reset();
		
        int idx_raetselstring = 0;
		bool lade_erfolg = true;
		
        for (int idx_y = 1; idx_y <= 9 && lade_erfolg == true; idx_y++)
		{
            for (int idx_x = 1; idx_x <= 9; idx_x++ && lade_erfolg == true, idx_raetselstring++)
			{
				if (raetselstring.at(idx_raetselstring).isDigit() == true)
				{
					lade_erfolg = setze_zahl(Position(idx_x, idx_y), QString(raetselstring.at(idx_raetselstring)).toInt());
				}
			}
		}
		
		// wenn es beim aufbau des zu ladenden raetsels zu einem fehler kam ueber den fehler informieren und die scene resetten
		if (lade_erfolg == false)
		{
			emit laden_fehlgeschlagen();
			
			reset();
		}
		
		else
		{
			erg = true;
			
			if (Spielstring != raetselstring)
			{
				Spielstring = raetselstring;
				
				// alle zeichen, die keine zahl sind in X umtauschen
                for (int idx = 0; idx < Spielstring.size(); idx++)
				{
					if (Spielstring.at(idx).isDigit() == false) Spielstring[idx] = 'X';
				}
			}
		}
	}
	
	else emit laden_fehlgeschlagen();
	
	return erg;
}


void Scene::restart()
{
	if (Spielstring.isEmpty() == true) reset();
	
	else
	{
		schutz_spielstring = true;
		
		raetsel_annehmen(Spielstring);
		
		schutz_spielstring = false;
	}
}


void Scene::mache_raetsel_aus_zustand()
{
	QString tmp_string;
	
    for (int idx_y = 1; idx_y <= 9; idx_y++)
	{
        for (int idx_x = 1; idx_x <= 9; idx_x++)
		{
			int tmp = gesamtliste.value(Position(idx_x, idx_y))->zahl_und_tip();
			
			if (tmp != 0) tmp_string += QString::number(tmp);
			else tmp_string += "X";
		}
	}
	
	if (tmp_string.size() == 81)
	{
		raetsel_annehmen(tmp_string);
		
		emit speichern_verfuegbar(true);
	}
}


void Scene::loese_alle_einzelnen()
{
	emit loese_einzelne();
}


void Scene::neues_leichtes_zufallsspiel()
{
	reset();
	
	emit erzeuge_einfaches_spiel();
}


void Scene::neues_mittleres_spiel()
{
	reset();
	
	emit erzeuge_mittleres_spiel();
}


// slots, die die scene und das spiel insgesamt betreffen
void Scene::zeige_kandidaten(bool zustand)
{
	emit anzeige_kandidaten(zustand);
}


void Scene::rueckgaengig()
{
	undo->rueckgaengig();
}


void Scene::wiederherstellen()
{
	undo->wiederherstellen();
}


void Scene::annahme_raetsel(const QString& txt)
{
	if (raetsel_annehmen(txt) == false) qDebug() << tr("Error in \"void Scene::annahme_raetsel(const QString& txt)\"");
}
