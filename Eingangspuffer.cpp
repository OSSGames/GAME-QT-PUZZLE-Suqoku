#include "Eingangspuffer.h"
#include <QTimer>

#ifdef Q_OS_MAC

#define EINGANGSPUFFER_WARTEZEIT 200

#else

#define EINGANGSPUFFER_WARTEZEIT 500

#endif

using namespace std;

Eingangspuffer::Eingangspuffer(QObject *parent) : QObject(parent), timer(0), pufferzeit(EINGANGSPUFFER_WARTEZEIT)
{
	// den timer erstellen
	timer = new QTimer(this);
	timer->setInterval(pufferzeit);
	timer->setSingleShot(true);
	
	// signal - slot verbindungen
	// reaktionen auf timer ermoeglichen
	connect(timer, SIGNAL(timeout()), this, SLOT(reaktion_auf_timer()));
}


Eingangspuffer::~Eingangspuffer()
{
}


void Eingangspuffer::reaktion_auf_timer()
{
	if (puffer.isEmpty() == false)
	{
		emit  befehlsliste(puffer.values());
		
		puffer.clear();
	}
}


void Eingangspuffer::fuege_befehl_hinzu(const QString& befehl)
{
	puffer.insert(befehl.toLower() + "|" + befehl, befehl);
	
	timer->start();
}


void Eingangspuffer::setze_pufferzeit(int wert)
{
	pufferzeit = wert;
}


int Eingangspuffer::puffer_zeit() const
{
	return pufferzeit;
}


void Eingangspuffer::fuege_befehle_hinzu(const QStringList& liste)
{
    for (int idx = 0; idx < liste.size(); idx++) puffer.insert(liste.at(idx).toLower() + "|" + liste.at(idx), liste.at(idx));
	
	timer->start();
}
