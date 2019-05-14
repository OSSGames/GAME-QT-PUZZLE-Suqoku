/*
 
 Name: Eingangspuffer
 Autor: Andreas Konarski
 Lizenz: GPL v3 or later
 Plattformen: Alle Systeme, auf denen QT 4.6 verfuegbar ist. Kompatibel mit QT 5.0.0.
 
 Kontakt: programmieren@konarski-wuppertal.de
 home: www.konarski-wuppertal.de
 
 Falls ich mit diesem Programm die Rechte von irgend jemand verletzen sollte, bitte ich um einen Hinweis. Wenn dies Tatsaechlich der Fall ist, entferne ich es schnellstmoeglich von meiner Homepage.
 
 */

#ifndef EINGANGSPUFFER_H
#define EINGANGSPUFFER_H

#include <QObject>
#include <QStringList>
#include <QMap>

class QTimer;

class Eingangspuffer : public QObject
{
	Q_OBJECT
	
public:
	Eingangspuffer(QObject *parent = 0);
	virtual ~Eingangspuffer();
	
	void setze_pufferzeit(int);
	int puffer_zeit() const;
	
public slots:
	void fuege_befehl_hinzu(const QString&);
	void fuege_befehle_hinzu(const QStringList&);
	
signals:
	void befehlsliste(const QStringList&);
	
	private slots:
	void reaktion_auf_timer();
	
private:
	QTimer *timer;
	QMap<QString, QString> puffer;
	int pufferzeit;
};

#endif
