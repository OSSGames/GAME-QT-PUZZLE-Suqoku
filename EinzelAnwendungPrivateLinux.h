/*
 
 Name: EinzelAnwendung
 Autor: Andreas Konarski
 Lizenz: GPL v3 or later
 Plattformen: Alle Systeme, auf denen QT 4.6 verfuegbar ist. Kompatibel mit QT 5.0.0.
 
 Kontakt: programmieren@konarski-wuppertal.de
 home: www.konarski-wuppertal.de
 
 Falls ich mit diesem Programm die Rechte von irgend jemand verletzen sollte, bitte ich um einen Hinweis. Wenn dies Tatsaechlich der Fall ist, entferne ich es schnellstmoeglich von meiner Homepage.
 
 */

#ifndef EINZELANWENDUNGPRIVATELINUX_H
#define EINZELANWENDUNGPRIVATELINUX_H

#include "EinzelAnwendungPrivateAllgemein.h"

class EinzelAnwendungPrivateLinux : public EinzelAnwendungPrivateAllgemein
{
	Q_OBJECT
	
public:
	EinzelAnwendungPrivateLinux(int& argc, char **argv, const QString& id, EinzelAnwendung* parent);
	virtual ~EinzelAnwendungPrivateLinux();
	
	virtual bool ist_allgemein() const;
	virtual bool ist_linux() const;
	virtual bool ist_windows() const;
	virtual bool ist_mac() const;
};

#endif
