/*
 
 Name: Suqoku
 Autor: Andreas Konarski
 Begonnen: 21.05.2010.
 Erstellt: 31.07.2016.
 Version: 0.69
 Lizenz: GPL v3 or later
 Plattformen: Alle Systeme, auf denen QT 4.6 verfuegbar ist. Kompatibel mit QT 5.0.0.
 
 Kontakt: programmieren@konarski-wuppertal.de
 home: www.konarski-wuppertal.de
 
 Falls ich mit diesem Programm die Rechte von irgend jemand verletzen sollte, bitte ich um einen Hinweis. Wenn dies Tatsaechlich der Fall ist, entferne ich es schnellstmoeglich von meiner Homepage.
 
 */

#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QLocale>
#include "MainWindow.h"
#include "EinzelAnwendung.h"

using namespace std;

int main(int argc, char *argv[])
{
	EinzelAnwendung anwendung(argc, argv, "Suqoku");
	
	int erg = 0;
	
#ifndef Q_OS_MAC
	
	if (anwendung.isRunning() == false)
	{
		
#endif
		
		// die anwendung uebersetzen
		QTranslator uebersetzer, uebersetzer2;
		
		QString dateiname_eigene_uebersetzung;
		dateiname_eigene_uebersetzung = QString(":/translations/Uebersetzung_%1").arg(QLocale::system().name());
		
		// die uebersetzungs dateien in die uebersetzer laden
		uebersetzer.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
		uebersetzer2.load(dateiname_eigene_uebersetzung);
		
		// die uebersetzer installieren
		anwendung.installTranslator(&uebersetzer);
		anwendung.installTranslator(&uebersetzer2);
		
		// das hauptfenster erzeugen ...
		MainWindow hauptfenster;
		
		// nicht unter macos
#ifndef Q_OS_MAC
		
		// das activation window setzen
		anwendung.setActivationWindow(&hauptfenster, true);
		
#endif
		
		// die annahme von dateien aus anderen instanzen ermoeglichen
		QObject::connect(&anwendung, SIGNAL(oeffne_dateien(const QStringList)), &hauptfenster, SLOT(annahme_von_extern(const QStringList)));
		
		// ... und anzeigen
		hauptfenster.show();
		
		// unter macos probleme mit der fenstergroesse verhindern
#ifdef Q_OS_MAC
		
		// die fenstergroesse fixieren
		hauptfenster.setFixedSize(hauptfenster.sizeHint());
		
		// verhindern das das fenster bei jedem start ein stueck weiter nach oben wandert
		hauptfenster.lade_fenstergeometrie();
		
#endif
		
#ifndef Q_OS_MAC
		
		anwendung.dateien_hinzufuegen(anwendung.argumente());
		
#endif
		
		erg = anwendung.exec();
		
#ifndef Q_OS_MAC
		
	}
	
	else
	{
		anwendung.sende_dateien();
		
		erg = 0;
	}
	
#endif
	
	return erg;
}
