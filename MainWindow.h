/*
 
 Name: Suqoku
 Autor: Andreas Konarski
 Lizenz: GPL v3 or later
 Plattformen: Alle Systeme, auf denen QT 4.6 verfuegbar ist. Kompatibel mit QT 5.0.0.
 
 Kontakt: programmieren@konarski-wuppertal.de
 home: www.konarski-wuppertal.de
 
 Falls ich mit diesem Programm die Rechte von irgend jemand verletzen sollte, bitte ich um einen Hinweis. Wenn dies Tatsaechlich der Fall ist, entferne ich es schnellstmoeglich von meiner Homepage.
 
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define VERSION "0.69"

#include <QMainWindow>
#include "ui_MainWindow.h"

class QSettings;
class Scene;
class QFileDialog;
class QMessageBox;
class QLabel;
class QAction;
class QTimer;
class QClipboard;

class MainWindow : public QMainWindow, private Ui::MainWindow
{
	Q_OBJECT
	
public:
	MainWindow(QWidget *parent = 0);
	virtual ~MainWindow();
	
	void aktualisiere_fenstertitel(const QString&);
	void lade_fenstergeometrie();
	
public slots:
	void annahme_von_extern(const QStringList);
	
private slots:
	void about();
	void starte_leeres_spiel();
	void einstellungen_laden();
	void raetsel_laden(const QString&);
	void einstellungen_speichern();
	void aktualisiere_status_label(int, int);
	void filter_1(bool);
	void filter_2(bool);
	void filter_3(bool);
	void filter_4(bool);
	void filter_5(bool);
	void filter_6(bool);
	void filter_7(bool);
	void filter_8(bool);
	void filter_9(bool);
	void filter_red(bool);
	void filter_green(bool);
	void filter_blue(bool);
	void filter_cyan(bool);
	void filter_magenta(bool);
	void filter_yellow(bool);
	void filter_paare(bool);
	void filter_dreier(bool);
	void filter_nichts(bool);
	void reset();
	void aktualisiere_groesse();
	void restart();
	void mache_raetsel_aus_zustand();
	void loese_einzelne();
	void vorbereitung_raetsel_speichern();
	void speichere_raetsel(const QString&);
	void vorbereitung_raetsel_speichern_unter();
	void reaktionen_auf_timer();
	void starte_timer();
	void aktualisiere_anzahl_zuege(int);
	void stoppe_timer();
	void vorbereitung_laden();
	void kopieren();
	void einfuegen();
	void steuerung_freigabe_einfuegen();
	void neues_leichtes_zufallsspiel();
	void neues_mittleres_spiel();
	void sperre();
	void entsperre();
	void starte_easy_01();
	void starte_easy_02();
	void reaktionen_auf_spielende();
	void reaktionen_auf_spielbeginn();
	void speichern_button_nach_zufallsspiel_erzeugung_freigeben();
	void sperre_annahme_von_extern();
	void entsperre_annahme_von_extern();
	
private:
	QSettings *einstellungen;
	Scene *scene;
	QFileDialog *lade_dialog, *speicher_dialog;
	QMessageBox *siegmeldung, *sicherheitsfrage, *ladefehler;
	QLabel *status_label, *zeit_label, *zugzahl_label;
	QAction *action_zeige_filter_toolbar;
	QString Dateiname;
	QTimer *timer;
	QClipboard *zwischenablage;
	bool externe_annahme_frei;
	
	bool sicherheitsfrage_spiel_abbruch(const QString&, const QString&);
	void closeEvent(QCloseEvent*);
	QString filtere_raetselstring(const QString&) const;
};

#endif
