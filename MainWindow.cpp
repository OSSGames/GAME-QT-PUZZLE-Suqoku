#include "MainWindow.h"
#include "Scene.h"
#include <QSettings>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QLabel>
#include <QToolBar>
#include <QActionGroup>
#include <QAction>
#include <QTimer>
#include <QCloseEvent>
#include <QClipboard>
#include <QMimeData>

#define TOOLBAR_ICON_GROESSE_MAC 21
#define TOOLBAR_ICON_GROESSE_LINUX 21
#define TOOLBAR_SPACING 0
#define TIMER_TOLLERANZ_MS 10
#define SEKUNDEN_PRO_TAG 86400
#define GRENZE_FENSTERTITEL 50

using namespace std;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), einstellungen(0), scene(0), lade_dialog(0), speicher_dialog(0), siegmeldung(0), sicherheitsfrage(0), ladefehler(0), status_label(0), zeit_label(0), zugzahl_label(0), action_zeige_filter_toolbar(0), timer(0), zwischenablage(0), externe_annahme_frei(true)
{
	// die benutzeroberflaeche aufbauen
	setupUi(this);
	
	// osx spezifische einstellungen vornehmen
#ifdef Q_OS_MAC
	
	setUnifiedTitleAndToolBarOnMac(true);
	
#endif
	
	// unter macos action_close verbergen
#ifdef Q_OS_MAC
	
	action_close->setVisible(false);
	
#endif
	
	// scene erstellen ...
	scene = new Scene(viewer);
	
	// ... und im viewer registrieren
	viewer->setScene(scene);
	
	// den lade dialog erstellen
	lade_dialog = new QFileDialog(this);
	lade_dialog->setWindowModality(Qt::WindowModal);
	lade_dialog->setAcceptMode(QFileDialog::AcceptOpen);
	lade_dialog->setDirectory(QDir::home());
	
	// leider gibt es unter mac os version lion momentan probleme (abstuertze) bei verwendung von nativen file dialogs. deswegen unter mac os dieses feature abschalten...
#ifdef Q_OS_MAC
	
	// Problem durch QT 4.8.2 behoben - lade_dialog->setOption(QFileDialog::DontUseNativeDialog, true);
	
#endif
	
	// den speicher dialog erstellen
	speicher_dialog = new QFileDialog(this);
	speicher_dialog->setWindowModality(Qt::WindowModal);
	speicher_dialog->setAcceptMode(QFileDialog::AcceptSave);
	speicher_dialog->setDirectory(QDir::home());
	speicher_dialog->setConfirmOverwrite(true);
	speicher_dialog->setDefaultSuffix("txt");
	
	// leider gibt es unter mac os version lion momentan probleme (abstuertze) bei verwendung von nativen file dialogs. deswegen unter mac os dieses feature abschalten...
#ifdef Q_OS_MAC
	
	// Problem durch QT 4.8.2 behoben - speicher_dialog->setOption(QFileDialog::DontUseNativeDialog, true);
	
#endif
	
	// siegmeldung erzeugen
	siegmeldung = new QMessageBox(this);
	siegmeldung->setWindowModality(Qt::WindowModal);
	siegmeldung->setWindowTitle(tr("Suqoku - You won !"));
	siegmeldung->setText(tr("You solved it !"));
	siegmeldung->addButton(QMessageBox::Ok);
	
	// sicherheitsfrage erstellen
	sicherheitsfrage = new QMessageBox(this);
	sicherheitsfrage->setWindowModality(Qt::WindowModal);
	sicherheitsfrage->setContentsMargins(3, 3, 3, 3);
	sicherheitsfrage->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
	sicherheitsfrage->setDefaultButton(QMessageBox::Cancel);
	
	// ladefehler erzeugen
	ladefehler = new QMessageBox(this);
	ladefehler->setContentsMargins(3, 3, 3, 3);
	ladefehler->setStandardButtons(QMessageBox::Ok);
	ladefehler->setDefaultButton(QMessageBox::Ok);
	ladefehler->setText(tr("Unable to load game"));
	ladefehler->setInformativeText(tr("The game could not been loaded. Try checking the file with a text editor."));
	
	// status label erzeugen ...
	status_label = new QLabel(this);
	
	// ... und der statusleiste als permanentes widget hinzufuegen
	statusBar()->addPermanentWidget(status_label);
	
	// zeit label erzeugen ...
	zeit_label = new QLabel(this);
	
	// ... und ebenfalls der statusleiste als permanentes widget hinzufuegen
	statusBar()->addPermanentWidget(zeit_label);
	
	// zugzahl_label erzeugen ...
	zugzahl_label = new QLabel(this);
	zugzahl_label->setText(tr("No moves"));
	
	// ... und ebenfalls der statusleiste als permanentes widget hinzufuegen
	statusBar()->addPermanentWidget(zugzahl_label);
	
	// timer erstellen
	timer = new QTimer(this);
	timer->setInterval(1000);
	
	// gruppe fuer filter actionen erzeugen
	QActionGroup *filter_gruppe = new QActionGroup(this);
	filter_gruppe->addAction(action_leer);
	filter_gruppe->addAction(action_filter_1);
	filter_gruppe->addAction(action_filter_2);
	filter_gruppe->addAction(action_filter_3);
	filter_gruppe->addAction(action_filter_4);
	filter_gruppe->addAction(action_filter_5);
	filter_gruppe->addAction(action_filter_6);
	filter_gruppe->addAction(action_filter_7);
	filter_gruppe->addAction(action_filter_8);
	filter_gruppe->addAction(action_filter_9);
	filter_gruppe->addAction(action_filter_red);
	filter_gruppe->addAction(action_filter_green);
	filter_gruppe->addAction(action_filter_blue);
	filter_gruppe->addAction(action_filter_cyan);
	filter_gruppe->addAction(action_filter_magenta);
	filter_gruppe->addAction(action_filter_yellow);
	filter_gruppe->addAction(action_filter_paare);
	filter_gruppe->addAction(action_filter_dreier);
	action_leer->setChecked(true);
	
	// filter_bar anpassen
	filter_bar->layout()->setSpacing(TOOLBAR_SPACING);
	
	// nur unter macosx
#ifdef Q_OS_MAC
	
	filter_bar->setIconSize(QSize(TOOLBAR_ICON_GROESSE_MAC, TOOLBAR_ICON_GROESSE_MAC));
	
#endif
	
	// nur, wenn es sich nicht um macos handelt die action zum steuern der sichtbarkeit der toolbar ins menu heften
#ifndef Q_OS_MAC
	
	// die sichtbarkeit von filter_bar steuerbar machen
	action_zeige_filter_toolbar = filter_bar->toggleViewAction();
	
	// ... und die action hinzufuegen
	menu_optionen->addAction(action_zeige_filter_toolbar);
	
	// nur unter linux
#ifdef Q_OS_LINUX
	
	filter_bar->setIconSize(QSize(TOOLBAR_ICON_GROESSE_LINUX, TOOLBAR_ICON_GROESSE_LINUX));
	
#endif
	
#endif
	
	// zwischenablage erfassen
	zwischenablage = QApplication::clipboard();
	
	// einstellungen erstellen
	einstellungen = new QSettings("konarski-wuppertal", qApp->applicationName(), this);
	
	// initialisierungen
	// status_label initialisieren
	aktualisiere_status_label(scene->anzahl_ausgefuellte_zellen(), scene->anzahl_unausgefuellte_zellen());
	
	// signal - slot verbindungen
	// action_about ermoeglichen
	connect(action_about, SIGNAL(triggered(bool)), this, SLOT(about()));
	
	// action_about_qt ermoeglichen
	connect(action_about_qt, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt()));
	
	// freigabesteuerung fuer externe annahme ermoeglichen
	connect(action_save, SIGNAL(triggered(bool)), this, SLOT(sperre_annahme_von_extern()));
	connect(action_save_as, SIGNAL(triggered(bool)), this, SLOT(sperre_annahme_von_extern()));
	connect(speicher_dialog, SIGNAL(finished(int)), this, SLOT(entsperre_annahme_von_extern()));
	
	// das starten eines leeren spiels ermoeglichen
	connect(action_leeres_spiel, SIGNAL(triggered(bool)), this, SLOT(starte_leeres_spiel()));
	
	// das laden von spielen ermoeglichen
	connect(action_oeffnen, SIGNAL(triggered(bool)), this, SLOT(vorbereitung_laden()));
	connect(lade_dialog, SIGNAL(fileSelected(const QString&)), this, SLOT(raetsel_laden(const QString&)));
	
	// das sperren der aktion zum leosen einzelner wenn gewonnen ermoeglichen
	connect(scene, SIGNAL(gewonnen()), this, SLOT(reaktionen_auf_spielende()));
	
	// zbd das entsperren von action_loese_einzelne
	connect(scene, SIGNAL(scene_resettet()), this, SLOT(reaktionen_auf_spielbeginn()));
	
	// wenn man gewonnen hat den timer stoppen
	connect(scene, SIGNAL(gewonnen()), this, SLOT(stoppe_timer()));
	
	// das anzeigen der siegmeldung ermoeglichen
	connect(scene, SIGNAL(gewonnen()), siegmeldung, SLOT(show()));
	
	// das aktualisieren von status_label ermoeglichen
	connect(scene, SIGNAL(spielstand(int, int)), this, SLOT(aktualisiere_status_label(int, int)));
	
	// geometrieanpassung bei aenderung der filter_bar ermoeglichen. jedoch nicht unter macos.
#ifndef Q_OS_MAC
	
	connect(action_zeige_filter_toolbar, SIGNAL(triggered(bool)), this, SLOT(aktualisiere_groesse()));
	
#endif
	
	// das steuern der filter ermoeglichen
	connect(action_leer, SIGNAL(triggered(bool)), this, SLOT(filter_nichts(bool)));
	connect(action_filter_1, SIGNAL(triggered(bool)), this, SLOT(filter_1(bool)));
	connect(action_filter_2, SIGNAL(triggered(bool)), this, SLOT(filter_2(bool)));
	connect(action_filter_3, SIGNAL(triggered(bool)), this, SLOT(filter_3(bool)));
	connect(action_filter_4, SIGNAL(triggered(bool)), this, SLOT(filter_4(bool)));
	connect(action_filter_5, SIGNAL(triggered(bool)), this, SLOT(filter_5(bool)));
	connect(action_filter_6, SIGNAL(triggered(bool)), this, SLOT(filter_6(bool)));
	connect(action_filter_7, SIGNAL(triggered(bool)), this, SLOT(filter_7(bool)));
	connect(action_filter_8, SIGNAL(triggered(bool)), this, SLOT(filter_8(bool)));
	connect(action_filter_9, SIGNAL(triggered(bool)), this, SLOT(filter_9(bool)));
	connect(action_filter_red, SIGNAL(triggered(bool)), this, SLOT(filter_red(bool)));
	connect(action_filter_green, SIGNAL(triggered(bool)), this, SLOT(filter_green(bool)));
	connect(action_filter_blue, SIGNAL(triggered(bool)), this, SLOT(filter_blue(bool)));
	connect(action_filter_cyan, SIGNAL(triggered(bool)), this, SLOT(filter_cyan(bool)));
	connect(action_filter_magenta, SIGNAL(triggered(bool)), this, SLOT(filter_magenta(bool)));
	connect(action_filter_yellow, SIGNAL(triggered(bool)), this, SLOT(filter_yellow(bool)));
	connect(action_filter_paare, SIGNAL(triggered(bool)), this, SLOT(filter_paare(bool)));
	connect(action_filter_dreier, SIGNAL(triggered(bool)), this, SLOT(filter_dreier(bool)));
	
	// wenn die scene resettet wurde den gewaehlten filter zuruecksetzen
	connect(scene, SIGNAL(scene_resettet()), this, SLOT(reset()));
	
	// das restarten von spielen ermoeglichen
	connect(action_restart, SIGNAL(triggered(bool)), this, SLOT(restart()));
	
	// das erzeugen von raetseln aus dem zustand ermoeglichen
	connect(action_neues_von_zustand, SIGNAL(triggered(bool)), this, SLOT(mache_raetsel_aus_zustand()));
	
	// das loesen aller einzelnen ermoeglichen
	connect(action_loese_einzelne, SIGNAL(triggered(bool)), this, SLOT(loese_einzelne()));
	
	// das speichern von raetseln ermoeglichen
	connect(action_save, SIGNAL(triggered(bool)), this, SLOT(vorbereitung_raetsel_speichern()));
	connect(speicher_dialog, SIGNAL(fileSelected(const QString&)), this, SLOT(speichere_raetsel(const QString&)));
	connect(action_save_as, SIGNAL(triggered(bool)), this, SLOT(vorbereitung_raetsel_speichern_unter()));
	
	// das steuern der speicher verfuegbarkeit aus scene heraus ermoeglichen
	connect(scene, SIGNAL(speichern_verfuegbar(bool)), action_save, SLOT(setEnabled(bool)));
	
	// reaktionen auf timer ermoeglichen
	connect(timer, SIGNAL(timeout()), this, SLOT(reaktionen_auf_timer()));
	
	// ein reset der scene startet den timer neu
	connect(scene, SIGNAL(scene_resettet()), this, SLOT(reaktionen_auf_timer()));
	
	// das steuern der sichtbarkeit der kandidaten setzen
	connect(action_zeige_kandidaten, SIGNAL(triggered(bool)), scene, SLOT(zeige_kandidaten(bool)));
	
	// steuerung der timer_label sichtbarkeit ermoeglichen
	connect(action_zeige_timer, SIGNAL(triggered(bool)), zeit_label, SLOT(setVisible(bool)));
	
	// steuerung des status_label ermoeglichen
	connect(action_zeige_status, SIGNAL(triggered(bool)), status_label, SLOT(setVisible(bool)));
	
	// action_undo ermoeglichen
	connect(action_undo, SIGNAL(triggered(bool)), scene, SLOT(rueckgaengig()));
	
	// action_redo ermoeglichen
	connect(action_redo, SIGNAL(triggered(bool)), scene, SLOT(wiederherstellen()));
	
	// steuern der verfuegbarkeit von undo und redo ermoeglichen
	connect(scene, SIGNAL(rueckgaengig_verfuegbar(bool)), action_undo, SLOT(setEnabled(bool)));
	connect(scene, SIGNAL(wiederherstellen_verfuegbar(bool)), action_redo, SLOT(setEnabled(bool)));
	
	// steuern der verfuegbarkeit von restart ermoeglichen
	connect(scene, SIGNAL(restart_verfuegbar(bool)), action_restart, SLOT(setEnabled(bool)));
	
	// das aktualisieren der zugzahl ermoeglichen
	connect(scene, SIGNAL(anzahl_zuege(int)), this, SLOT(aktualisiere_anzahl_zuege(int)));
	
	// das steuern der sichtbarkeit der zugzahl ermoeglichen
	connect(action_zeige_zugzahl, SIGNAL(triggered(bool)), zugzahl_label, SLOT(setVisible(bool)));
	
	// wenn die kandidaten nicht angezeigt werden das loesen der einzelnen blockieren
	connect(action_zeige_kandidaten, SIGNAL(triggered(bool)), action_loese_einzelne, SLOT(setEnabled(bool)));
	
	// information bei ladefehler ermoeglichen
	connect(scene, SIGNAL(laden_fehlgeschlagen()), ladefehler, SLOT(show()));
	
	// einfuegen und kopieren ermoeglichen
	connect(action_kopieren, SIGNAL(triggered(bool)), this, SLOT(kopieren()));
	connect(action_einfuegen, SIGNAL(triggered(bool)), this, SLOT(einfuegen()));
	
	// das steuern der freigabe fuers einfuegen ermoeglichen
	connect(zwischenablage, SIGNAL(dataChanged()), this, SLOT(steuerung_freigabe_einfuegen()));
	
	// das starten von zufaellig erzeugten spielen ermoeglichen
	connect(action_neues_leichtes_zufallsspiel, SIGNAL(triggered(bool)), this, SLOT(neues_leichtes_zufallsspiel()));
	connect(action_neues_mittleres_spiel, SIGNAL(triggered(bool)), this, SLOT(neues_mittleres_spiel()));
	
	// sperren und freigeben von viewer ermoeglichen
	connect(scene, SIGNAL(erzeugung_begonnen()), this, SLOT(sperre()));
	connect(scene, SIGNAL(erzeugung_fertig()), this, SLOT(entsperre()));
	
	// das starten von getesteten spielen ermoeglichen
	connect(action_easy_01, SIGNAL(triggered(bool)), this, SLOT(starte_easy_01()));
	connect(action_easy_02, SIGNAL(triggered(bool)), this, SLOT(starte_easy_02()));
	
	// wenn ein spiel fertig eruegt wurde die speicherbuttons freigeben
	connect(scene, SIGNAL(erzeugung_fertig()), this, SLOT(speichern_button_nach_zufallsspiel_erzeugung_freigeben()));
	
	// das schliessen ueber action_close ermoeglichen
	connect(action_close, SIGNAL(triggered(bool)), this, SLOT(close()));
	
	// initialisierungen
	einstellungen_laden();
	
	// die fenstergroesse auf sizehint feststellen. jedoch nicht unter macos, dort wuerde dies zu problemen mit der femstergroesse fuehren. unter macos wird dies in main, unmittelbar nach show durchgefuehrt
#ifndef Q_OS_MAC
	
	setFixedSize(sizeHint());
	
#endif
	
	// den timer vorbereiten und starten
	reaktionen_auf_timer();
	starte_timer();
	
	// die speicher aktion sperren
	action_save->setEnabled(false);
	action_save_as->setEnabled(false);
	
	// den fenstertitel initialisieren
	aktualisiere_fenstertitel(tr("Empty game"));
}


MainWindow::~MainWindow()
{
	einstellungen_speichern();
}


void MainWindow::about()
{
	QMessageBox::about(this, tr("About Suqoku"), tr("Suqoku version %1 \n\nAuthor:\tAndreas Konarski\nLicence:\tgpl v3 or later\n\nContact:\n\nprogrammieren@konarski-wuppertal.de\nwww.konarski-wuppertal.de").arg(VERSION));
}


void MainWindow::raetsel_laden(const QString& dateiname)
{
	QFile datenstrom(dateiname);
	
	if (datenstrom.open(QIODevice::ReadOnly | QIODevice::Text) == true)
	{
		action_save->setEnabled(false);
		action_save_as->setEnabled(true);
		
		Dateiname.clear();
		
		QString spielstring(QString::fromUtf8(datenstrom.readAll()));
		
		datenstrom.close();
		
		// wenn die annahme des raetsels erfolgreich ist
		if (scene->raetsel_annehmen(filtere_raetselstring(spielstring)) == true)
		{
			// nur, wenn keine resource geladen wurde
			if (dateiname.startsWith(":/") == false) aktualisiere_fenstertitel(dateiname);
		}
		
		// wenn die annahme des raetsels scheitert
		else aktualisiere_fenstertitel(tr("Empty game"));
	}
	
	else qDebug() << tr("Unable to open \"%1\"").arg(dateiname);
}


void MainWindow::starte_leeres_spiel()
{
	bool freigabe = true;
	
	if (scene->laufendes_spiel() == true) freigabe = sicherheitsfrage_spiel_abbruch(tr("Break current game ?"), tr("Did you really want to start a new empty game and break the current game ?"));
	
	if (freigabe == true)
	{
		aktualisiere_fenstertitel(tr("Empty game"));
		
		scene->reset();
		
		Dateiname.clear();
		
		action_save->setEnabled(false);
		action_save_as->setEnabled(false);
	}
}


void MainWindow::einstellungen_laden()
{
	// die fenstergeometrie laden
	lade_fenstergeometrie();
	
	// die festereinstellungen laden
	restoreState(einstellungen->value("MainWindow/state").toByteArray());
	
	// den zustand der filter toolbar laden. ausser unter macos.
#ifndef Q_OS_MAC
	
	filter_bar->setVisible(einstellungen->value("MainWindow/filter_bar", true).toBool());
	
#endif
	
	// die sichtbarkeit der kandidaten laden
	action_zeige_kandidaten->setChecked(einstellungen->value("Scene/sichtbarkeit_kandidaten", true).toBool());
	scene->zeige_kandidaten(action_zeige_kandidaten->isChecked());
	action_loese_einzelne->setEnabled(action_zeige_kandidaten->isChecked());
	
	// die sichtbarkeit von zeit_label laden
	action_zeige_timer->setChecked(einstellungen->value("MainWindow/sichtbarkeit_zeit_label", true).toBool());
	zeit_label->setVisible(action_zeige_timer->isChecked());
	
	// die sichtbarkeit des status laden
	action_zeige_status->setChecked(einstellungen->value("MainWindow/sichtbarkeit_status", true).toBool());
	status_label->setVisible(action_zeige_status->isChecked());
	
	// die sichtbarkeit der zugzahl ermoeglichen
	action_zeige_zugzahl->setChecked(einstellungen->value("MainWindow/sichtbarkeit_zugzahl", true).toBool());
	zugzahl_label->setVisible(action_zeige_zugzahl->isChecked());
}


void MainWindow::einstellungen_speichern()
{
	// die fenstergeometrie speichern
	einstellungen->setValue("MainWindow/geometrie", saveGeometry());
	
	// die festereinstellungen speichern
	einstellungen->setValue("MainWindow/state", saveState());
	
	// den zustand der filter toolbar speichern. ausser unter macos.
#ifndef Q_OS_MAC
	
	einstellungen->setValue("MainWindow/filter_bar", action_zeige_filter_toolbar->isChecked());
	
#endif
	
	// die sichtbarkeit der kandidaten speichern
	einstellungen->setValue("Scene/sichtbarkeit_kandidaten", action_zeige_kandidaten->isChecked());
	
	// die sichtbarkeit von zeit_label speichern
	einstellungen->setValue("MainWindow/sichtbarkeit_zeit_label", action_zeige_timer->isChecked());
	
	// die sichtbarkeit des status speichern
	einstellungen->setValue("MainWindow/sichtbarkeit_status", action_zeige_status->isChecked());
	
	// die sichtbarkeit der zugzahl speichern
	einstellungen->setValue("MainWindow/sichtbarkeit_zugzahl", action_zeige_zugzahl->isChecked());
}


void MainWindow::aktualisiere_status_label(int geloeste, int ungeloeste)
{
	status_label->setText(tr("%1 filled, %2 blank").arg(geloeste).arg(ungeloeste));
}


void MainWindow::filter_1(bool)
{
	scene->filtere(SCENE_FILTER_1);
}


void MainWindow::filter_2(bool)
{
	scene->filtere(SCENE_FILTER_2);
}


void MainWindow::filter_3(bool)
{
	scene->filtere(SCENE_FILTER_3);
}


void MainWindow::filter_4(bool)
{
	scene->filtere(SCENE_FILTER_4);
}


void MainWindow::filter_5(bool)
{
	scene->filtere(SCENE_FILTER_5);
}


void MainWindow::filter_6(bool)
{
	scene->filtere(SCENE_FILTER_6);
}


void MainWindow::filter_7(bool)
{
	scene->filtere(SCENE_FILTER_7);
}


void MainWindow::filter_8(bool)
{
	scene->filtere(SCENE_FILTER_8);
}


void MainWindow::filter_9(bool)
{
	scene->filtere(SCENE_FILTER_9);
}


void MainWindow::filter_red(bool)
{
	scene->filtere(SCENE_FILTER_RED);
}


void MainWindow::filter_green(bool)
{
	scene->filtere(SCENE_FILTER_GREEN);
}


void MainWindow::filter_blue(bool)
{
	scene->filtere(SCENE_FILTER_BLUE);
}


void MainWindow::filter_cyan(bool)
{
	scene->filtere(SCENE_FILTER_CYAN);
}


void MainWindow::filter_magenta(bool)
{
	scene->filtere(SCENE_FILTER_MAGENTA);
}


void MainWindow::filter_yellow(bool)
{
	scene->filtere(SCENE_FILTER_YELLOW);
}


void MainWindow::filter_nichts(bool)
{
	scene->filtere(SCENE_FILTER_NICHTS);
}


void MainWindow::filter_paare(bool)
{
	scene->filtere(SCENE_FILTER_PAARE);
}


void MainWindow::filter_dreier(bool)
{
	scene->filtere(SCENE_FILTER_DREIER);
}


void MainWindow::reset()
{
	action_leer->setChecked(true);
}


void MainWindow::aktualisiere_groesse()
{
#ifndef Q_OS_MAC
	
	setFixedSize(sizeHint());
	
#endif
}


void MainWindow::restart()
{
	bool freigabe = true;
	
	if (scene->laufendes_spiel() == true) freigabe = sicherheitsfrage_spiel_abbruch(tr("Break current game ?"), tr("Did you really want to restart the current game and break the current state ?"));
	
	if (freigabe == true) scene->restart();
}


void MainWindow::mache_raetsel_aus_zustand()
{
	bool freigabe = true;
	
	if (scene->laufendes_spiel() == true) freigabe = sicherheitsfrage_spiel_abbruch(tr("Break current game ?"), tr("Did you really want to make a new game from current state and break the current game ?"));
	
	if (freigabe == true)
	{
		action_save->setEnabled(true);
		action_save_as->setEnabled(true);
		
		Dateiname.clear();
		
		aktualisiere_fenstertitel(tr("Edited game"));
		
		scene->mache_raetsel_aus_zustand();
	}
}


void MainWindow::loese_einzelne()
{
	scene->loese_alle_einzelnen();
}


void MainWindow::vorbereitung_raetsel_speichern()
{
	if (scene->raetsel().size() == 81)
	{
		if (Dateiname.isEmpty() == true) speicher_dialog->show();
		
		else speichere_raetsel(Dateiname);
	}
}


void MainWindow::speichere_raetsel(const QString& dateiname)
{
	QFile datenstrom(dateiname);
	
	if (datenstrom.open(QIODevice::WriteOnly | QIODevice::Text) == true)
	{
		aktualisiere_fenstertitel(dateiname);
		
		if (Dateiname != dateiname) Dateiname = dateiname;
		
		QString speicher_string, tmp_string(scene->raetsel());
		
		if (tmp_string.size() == 81)
		{
			// drei zeichen aus tmp_string entnehmen
			speicher_string += tmp_string.at(0);
			speicher_string += tmp_string.at(1);
			speicher_string += tmp_string.at(2);
			
			speicher_string += '|';
			
			// drei zeichen aus tmp_string entnehmen
			speicher_string += tmp_string.at(3);
			speicher_string += tmp_string.at(4);
			speicher_string += tmp_string.at(5);
			
			speicher_string += '|';
			
			// drei zeichen aus tmp_string entnehmen
			speicher_string += tmp_string.at(6);
			speicher_string += tmp_string.at(7);
			speicher_string += tmp_string.at(8);
			
			speicher_string += "\n";
			
			// drei zeichen aus tmp_string entnehmen
			speicher_string += tmp_string.at(9);
			speicher_string += tmp_string.at(10);
			speicher_string += tmp_string.at(11);
			
			speicher_string += '|';
			
			// drei zeichen aus tmp_string entnehmen
			speicher_string += tmp_string.at(12);
			speicher_string += tmp_string.at(13);
			speicher_string += tmp_string.at(14);
			
			speicher_string += '|';
			
			// drei zeichen aus tmp_string entnehmen
			speicher_string += tmp_string.at(15);
			speicher_string += tmp_string.at(16);
			speicher_string += tmp_string.at(17);
			
			speicher_string += "\n";
			
			// drei zeichen aus tmp_string entnehmen
			speicher_string += tmp_string.at(18);
			speicher_string += tmp_string.at(19);
			speicher_string += tmp_string.at(20);
			
			speicher_string += '|';
			
			// drei zeichen aus tmp_string entnehmen
			speicher_string += tmp_string.at(21);
			speicher_string += tmp_string.at(22);
			speicher_string += tmp_string.at(23);
			
			speicher_string += '|';
			
			// drei zeichen aus tmp_string entnehmen
			speicher_string += tmp_string.at(24);
			speicher_string += tmp_string.at(25);
			speicher_string += tmp_string.at(26);
			
			speicher_string += "\n-----------\n";
			
			// drei zeichen aus tmp_string entnehmen
			speicher_string += tmp_string.at(27);
			speicher_string += tmp_string.at(28);
			speicher_string += tmp_string.at(29);
			
			speicher_string += '|';
			
			// drei zeichen aus tmp_string entnehmen
			speicher_string += tmp_string.at(30);
			speicher_string += tmp_string.at(31);
			speicher_string += tmp_string.at(32);
			
			speicher_string += '|';
			
			// drei zeichen aus tmp_string entnehmen
			speicher_string += tmp_string.at(33);
			speicher_string += tmp_string.at(34);
			speicher_string += tmp_string.at(35);
			
			speicher_string += "\n";
			
			// drei zeichen aus tmp_string entnehmen
			speicher_string += tmp_string.at(36);
			speicher_string += tmp_string.at(37);
			speicher_string += tmp_string.at(38);
			
			speicher_string += '|';
			
			// drei zeichen aus tmp_string entnehmen
			speicher_string += tmp_string.at(39);
			speicher_string += tmp_string.at(40);
			speicher_string += tmp_string.at(41);
			
			speicher_string += '|';
			
			// drei zeichen aus tmp_string entnehmen
			speicher_string += tmp_string.at(42);
			speicher_string += tmp_string.at(43);
			speicher_string += tmp_string.at(44);
			
			speicher_string += "\n";
			
			// drei zeichen aus tmp_string entnehmen
			speicher_string += tmp_string.at(45);
			speicher_string += tmp_string.at(46);
			speicher_string += tmp_string.at(47);
			
			speicher_string += '|';
			
			// drei zeichen aus tmp_string entnehmen
			speicher_string += tmp_string.at(48);
			speicher_string += tmp_string.at(49);
			speicher_string += tmp_string.at(50);
			
			speicher_string += '|';
			
			// drei zeichen aus tmp_string entnehmen
			speicher_string += tmp_string.at(51);
			speicher_string += tmp_string.at(52);
			speicher_string += tmp_string.at(53);
			
			speicher_string += "\n-----------\n";
			
			// drei zeichen aus tmp_string entnehmen
			speicher_string += tmp_string.at(54);
			speicher_string += tmp_string.at(55);
			speicher_string += tmp_string.at(56);
			
			speicher_string += '|';
			
			// drei zeichen aus tmp_string entnehmen
			speicher_string += tmp_string.at(57);
			speicher_string += tmp_string.at(58);
			speicher_string += tmp_string.at(59);
			
			speicher_string += '|';
			
			// drei zeichen aus tmp_string entnehmen
			speicher_string += tmp_string.at(60);
			speicher_string += tmp_string.at(61);
			speicher_string += tmp_string.at(62);
			
			speicher_string += "\n";
			
			// drei zeichen aus tmp_string entnehmen
			speicher_string += tmp_string.at(63);
			speicher_string += tmp_string.at(64);
			speicher_string += tmp_string.at(65);
			
			speicher_string += '|';
			
			// drei zeichen aus tmp_string entnehmen
			speicher_string += tmp_string.at(66);
			speicher_string += tmp_string.at(67);
			speicher_string += tmp_string.at(68);
			
			speicher_string += '|';
			
			// drei zeichen aus tmp_string entnehmen
			speicher_string += tmp_string.at(69);
			speicher_string += tmp_string.at(70);
			speicher_string += tmp_string.at(71);
			
			speicher_string += "\n";
			
			// drei zeichen aus tmp_string entnehmen
			speicher_string += tmp_string.at(72);
			speicher_string += tmp_string.at(73);
			speicher_string += tmp_string.at(74);
			
			speicher_string += '|';
			
			// drei zeichen aus tmp_string entnehmen
			speicher_string += tmp_string.at(75);
			speicher_string += tmp_string.at(76);
			speicher_string += tmp_string.at(77);
			
			speicher_string += '|';
			
			// drei zeichen aus tmp_string entnehmen
			speicher_string += tmp_string.at(78);
			speicher_string += tmp_string.at(79);
			speicher_string += tmp_string.at(80);
			
			speicher_string += "\n";
			
			// das zum speichern aufbereitete raetsel speichern
			datenstrom.write(speicher_string.toUtf8());
			
			// den datenstrom speichern
			datenstrom.close();
		}
	}
	
	// wenn das oeffnen des datenstroms fehlschlaegt eine fehlermeldung ausgeben
	else qDebug() << tr("Unable to save \"%1\"").arg(dateiname);
}


void MainWindow::vorbereitung_raetsel_speichern_unter()
{
	speicher_dialog->show();
}


void MainWindow::reaktionen_auf_timer()
{
	// synchronisierung des timers sicherstellen
	if (timer->isActive() == true && QTime::currentTime().msec() > TIMER_TOLLERANZ_MS && QTime::currentTime().msec() < (1000 - TIMER_TOLLERANZ_MS)) timer->setInterval(1000 - QTime::currentTime().msec());
	else if (timer->isActive() == true && timer->interval() != 1000) timer->setInterval(1000);
	
	int tage = scene->spieldauer() / SEKUNDEN_PRO_TAG;
	
	QString zeitstring;
	
	if (tage > 0) zeitstring += QString::number(tage) + ", ";
	zeitstring += QTime(0, 0, 0).addSecs(scene->spieldauer() % SEKUNDEN_PRO_TAG).toString("hh::mm:ss");
	
	zeit_label->setText(zeitstring);
}


void MainWindow::starte_timer()
{
	timer->setInterval(1000 - QTime::currentTime().msec());
	timer->start();
}


void MainWindow::aktualisiere_anzahl_zuege(int wert)
{
	if (wert == 0) zugzahl_label->setText(tr("No moves"));
	else if (wert == 1) zugzahl_label->setText(tr("%1 move").arg(wert));
	else zugzahl_label->setText(tr("%1 moves").arg(wert));
}


void MainWindow::stoppe_timer()
{
	timer->stop();
}


void MainWindow::vorbereitung_laden()
{
	bool freigabe = true;
	
	if (scene->laufendes_spiel() == true) freigabe = sicherheitsfrage_spiel_abbruch(tr("Break current game ?"), tr("Did you really want to load a new game and break the current game ?"));
	
	if (freigabe == true) lade_dialog->show();
}


bool MainWindow::sicherheitsfrage_spiel_abbruch(const QString& txt1, const QString& txt2)
{
	sicherheitsfrage->setText(txt1);
	sicherheitsfrage->setInformativeText(txt2);
	
	int box_erg = sicherheitsfrage->exec();
	
	bool erg = false;
	
	switch (box_erg)
	{
		case QMessageBox::Ok:
			
			erg = true;
			
			break;
			
		case QMessageBox::Cancel:
			
			erg = false;
			
			break;
			
		default: 
			
			erg = false;
			
			break;
	}
	
	return erg;
}


void MainWindow::closeEvent(QCloseEvent* event)
{
	bool freigabe = true;
	
	if (scene->laufendes_spiel() == true) freigabe = sicherheitsfrage_spiel_abbruch(tr("Break current game ?"), tr("Did you really want quit the application and break the current game ?"));
	
	if (freigabe == true) event->accept();
	else event->ignore();
}


QString MainWindow::filtere_raetselstring(const QString& txt) const
{
	QString erg(txt);
	
	// enter zeichen entfernen
	erg.remove('\n');
	
	// verschiedene unnuetze zeichen entfernen
	erg.remove('|');
	erg.remove('_');
	erg.remove('-');
	erg.remove(' ');
	erg.remove('*');
	erg.remove('+');
	
	// ersetzungen durchfuehren
	erg.replace('0', 'X');
	
	return erg;
}


void MainWindow::kopieren()
{
	zwischenablage->setText(scene->raetsel());
}


void MainWindow::einfuegen()
{
	if (zwischenablage->mimeData()->hasText() == true)
	{
		bool freigabe = true;
		
		QString txt(filtere_raetselstring(zwischenablage->text()));
		
		if (txt.size() != 81) freigabe = false;
		
		if (freigabe == true && scene->laufendes_spiel() == true) freigabe = sicherheitsfrage_spiel_abbruch(tr("Break current game ?"), tr("Did you really want to paste a new game from clipboardand break the current game ?"));
		
		if (freigabe == true)
		{
			Dateiname.clear();
			action_save->setEnabled(true);
			action_save_as->setEnabled(true);
			
			if (scene->raetsel_annehmen(txt) == true) aktualisiere_fenstertitel(tr("Pasted game"));
			
			else aktualisiere_fenstertitel(tr("Empty game"));
		}
	}
}


void MainWindow::steuerung_freigabe_einfuegen()
{
	if (zwischenablage->mimeData()->hasText() == true && filtere_raetselstring(zwischenablage->text()).size() == 81) action_einfuegen->setEnabled(true);
	else action_einfuegen->setEnabled(false);
}


void MainWindow::neues_leichtes_zufallsspiel()
{
	bool freigabe = true;
	
	if (scene->laufendes_spiel() == true) freigabe = sicherheitsfrage_spiel_abbruch(tr("Break current game ?"), tr("Did you really want to start a new game and break the current game ?"));
	
	if (freigabe == true)
	{
		aktualisiere_fenstertitel(tr("Easy random generated game"));
		
		scene->neues_leichtes_zufallsspiel();
		
		Dateiname.clear();
	}
}


void MainWindow::sperre()
{
	viewer->setEnabled(false);
	filter_bar->setEnabled(false);
}


void MainWindow::entsperre()
{
	viewer->setEnabled(true);
	filter_bar->setEnabled(true);
}


void MainWindow::neues_mittleres_spiel()
{
	bool freigabe = true;
	
	if (scene->laufendes_spiel() == true) freigabe = sicherheitsfrage_spiel_abbruch(tr("Break current game ?"), tr("Did you really want to start a new game and break the current game ?"));
	
	if (freigabe == true)
	{
		aktualisiere_fenstertitel(tr("Medium random generated game"));
		
		scene->neues_mittleres_spiel();
		
		Dateiname.clear();
	}
}


void MainWindow::starte_easy_01()
{
	bool freigabe = true;
	
	if (scene->laufendes_spiel() == true) freigabe = sicherheitsfrage_spiel_abbruch(tr("Break current game ?"), tr("Did you really want to start a new game and break the current game ?"));
	
	if (freigabe == true)
	{
		aktualisiere_fenstertitel(tr("Easy 01"));
		
		raetsel_laden(":/raetsel/raetsel/easy_01.txt");
		
		Dateiname.clear();
	}
}


void MainWindow::reaktionen_auf_spielende()
{
	action_loese_einzelne->setEnabled(false);
	action_neues_von_zustand->setEnabled(false);
}


void MainWindow::reaktionen_auf_spielbeginn()
{
	action_loese_einzelne->setEnabled(true);
	action_neues_von_zustand->setEnabled(true);
}


void MainWindow::starte_easy_02()
{
	bool freigabe = true;
	
	if (scene->laufendes_spiel() == true) freigabe = sicherheitsfrage_spiel_abbruch(tr("Break current game ?"), tr("Did you really want to start a new game and break the current game ?"));
	
	if (freigabe == true)
	{
		aktualisiere_fenstertitel(tr("Easy 02"));
		
		raetsel_laden(":/raetsel/raetsel/easy_02.txt");
		
		Dateiname.clear();
	}
}


void MainWindow::aktualisiere_fenstertitel(const QString& txt)
{
	// wenn txt leer ist einfach den namen der anwendung in den fenstertitel schreiben
	if (txt.isEmpty() == true) setWindowTitle("Suqoku");
	
	// ansonsten
	else
	{
		QString tmp(txt);
		
		// wenn txt zu lang fuer den fenstertitel ist
		if (tmp.size() > GRENZE_FENSTERTITEL)
		{
			// den ueberfluessigen teil entfernen
			tmp.remove(0, tmp.size() - GRENZE_FENSTERTITEL);
			
			// darauf hinweisen, das nicht der komplette text angezeigt wird
			tmp.prepend("...");
		}
		
		setWindowTitle(QString("Suqoku - %1").arg(tmp));
	}
}


void MainWindow::speichern_button_nach_zufallsspiel_erzeugung_freigeben()
{
	action_save->setEnabled(true);
	action_save_as->setEnabled(true);
}


void MainWindow::annahme_von_extern(const QStringList liste)
{
	if (externe_annahme_frei == true && liste.isEmpty() == false && QFile::exists(liste.first()))
	{
		bool freigabe = true;
		
		if (scene->laufendes_spiel() == true) freigabe = sicherheitsfrage_spiel_abbruch(tr("Break current game ?"), tr("Did you really want to open a new game and break the current state ?"));
		
		if (freigabe == true) raetsel_laden(liste.first());
	}
}


void MainWindow::sperre_annahme_von_extern()
{
	externe_annahme_frei = false;
}


void MainWindow::entsperre_annahme_von_extern()
{
	externe_annahme_frei = true;
}


void MainWindow::lade_fenstergeometrie()
{
	restoreGeometry(einstellungen->value("MainWindow/geometrie").toByteArray());
}
