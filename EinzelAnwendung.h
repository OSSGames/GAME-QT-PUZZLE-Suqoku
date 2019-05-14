/*
 
 Name: EinzelAnwendung
 Autor: Andreas Konarski
 Lizenz: GPL v3 or later
 Plattformen: Alle Systeme, auf denen QT 4.6 verfuegbar ist. Kompatibel mit QT 5.0.0.
 
 Kontakt: programmieren@konarski-wuppertal.de
 home: www.konarski-wuppertal.de
 
 Falls ich mit diesem Programm die Rechte von irgend jemand verletzen sollte, bitte ich um einen Hinweis. Wenn dies Tatsaechlich der Fall ist, entferne ich es schnellstmoeglich von meiner Homepage.
 
 */

#ifndef EINZELANWENDUNG_H
#define EINZELANWENDUNG_H

#include <QObject>
#include <QStringList>

class EinzelAnwendungPrivateBasis;
class QTranslator;
class QApplication;
class QtSingleApplication;

class EinzelAnwendung : public QObject
{
	Q_OBJECT
	
public:
	EinzelAnwendung(int& argc, char **argv, const QString& id, QObject *parent = 0);
	virtual ~EinzelAnwendung();
	
	void installTranslator(QTranslator*);
	bool exec();
	bool isRunning();
	QStringList argumente() const;
	QStringList arguments() const;
	void sende_dateien();
	bool ist_allgemein() const;
	bool ist_linux() const;
	bool ist_windows() const;
	bool ist_mac() const;
	bool ist_qapplication() const;
	bool ist_qtsingleapplication() const;
	QApplication* qapplication();
	QtSingleApplication* qtsingleapplication();
	QString id() const;
	void setActivationWindow(QWidget* aw, bool activateOnMessage = true);
	QWidget* activationWindow() const;
	void activateWindow();
	
public slots:
	void datei_hinzufuegen(const QString&);
	void dateien_hinzufuegen(const QStringList&);
	
signals:
	void oeffne_dateien(const QStringList);
	
private:
	EinzelAnwendungPrivateBasis *anwendung_private;
};

#endif
