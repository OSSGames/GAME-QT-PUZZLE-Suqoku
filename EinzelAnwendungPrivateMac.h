/*
 
 Name: EinzelAnwendung
 Autor: Andreas Konarski
 Lizenz: GPL v3 or later
 Plattformen: Alle Systeme, auf denen QT 4.6 verfuegbar ist. Kompatibel mit QT 5.0.0.
 
 Kontakt: programmieren@konarski-wuppertal.de
 home: www.konarski-wuppertal.de
 
 Falls ich mit diesem Programm die Rechte von irgend jemand verletzen sollte, bitte ich um einen Hinweis. Wenn dies Tatsaechlich der Fall ist, entferne ich es schnellstmoeglich von meiner Homepage.
 
 */

#ifndef EINZELANWENDUNGPRIVATEMAC_H
#define EINZELANWENDUNGPRIVATEMAC_H

#include "EinzelAnwendungPrivateBasis.h"
#include <QApplication>
#include <QStringList>
#include <QString>

class EinzelAnwendung;
class Eingangspuffer;

class EinzelAnwendungPrivateMac : public QApplication, public EinzelAnwendungPrivateBasis
{
	Q_OBJECT
	
public:
	EinzelAnwendungPrivateMac(int& argc, char **argv, const QString& id, EinzelAnwendung* parent);
	virtual ~EinzelAnwendungPrivateMac();
	
	virtual void installTranslator(QTranslator*);
	virtual bool exec();
	virtual QStringList argumente() const;
	QStringList arguments() const;
	
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
	
signals:
	void oeffne_datei(const QString);
	void oeffne_dateien(const QStringList);
	
private:
	Eingangspuffer *puffer;
	
	virtual bool event(QEvent*);
};

#endif
