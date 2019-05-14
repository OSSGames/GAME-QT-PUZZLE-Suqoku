/*
 
 Name: EinzelAnwendung
 Autor: Andreas Konarski
 Lizenz: GPL v3 or later
 Plattformen: Alle Systeme, auf denen QT 4.6 verfuegbar ist. Kompatibel mit QT 5.0.0.
 
 Kontakt: programmieren@konarski-wuppertal.de
 home: www.konarski-wuppertal.de
 
 Falls ich mit diesem Programm die Rechte von irgend jemand verletzen sollte, bitte ich um einen Hinweis. Wenn dies Tatsaechlich der Fall ist, entferne ich es schnellstmoeglich von meiner Homepage.
 
 */

#ifndef EINZELANWENDUNGPRIVATEBASIS_H
#define EINZELANWENDUNGPRIVATEBASIS_H

class EinzelAnwendung;
class QTranslator;
class QApplication;
class QtSingleApplication;
class QWidget;

#include <QString>

class EinzelAnwendungPrivateBasis
{
public:
	EinzelAnwendungPrivateBasis(int& argc, char **argv, const QString& id, EinzelAnwendung* parent);
	virtual ~EinzelAnwendungPrivateBasis();
	
	EinzelAnwendung* anwendung() const;
	virtual void installTranslator(QTranslator*);
	virtual bool exec();
	virtual bool isRunning();
	virtual QStringList argumente() const;
	virtual QStringList arguments() const;
	virtual void sende_dateien();
	virtual bool ist_allgemein() const;
	virtual bool ist_linux() const;
	virtual bool ist_windows() const;
	virtual bool ist_mac() const;
	virtual bool ist_qapplication() const;
	virtual bool ist_qtsingleapplication() const;
	virtual QApplication* qapplication();
	virtual QtSingleApplication* qtsingleapplication();
	virtual QString id() const;
	virtual void setActivationWindow(QWidget* aw, bool activateOnMessage = true);
	virtual QWidget* activationWindow() const;
	virtual void activateWindow();
	virtual void datei_hinzufuegen(const QString&);
	virtual void dateien_hinzufuegen(const QStringList&);
	
private:
	EinzelAnwendung* meine_anwendung;
};

#endif
