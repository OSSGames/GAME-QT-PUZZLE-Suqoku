#include "EinzelAnwendungPrivateMac.h"
#include "EinzelAnwendung.h"
#include "Eingangspuffer.h"
#include <QFileOpenEvent>

using namespace std;

EinzelAnwendungPrivateMac::EinzelAnwendungPrivateMac(int& argc, char **argv, const QString& id, EinzelAnwendung* parent) : QApplication(argc, argv), EinzelAnwendungPrivateBasis(argc, argv, id, parent), puffer(0)
{
	// den puffer fuer zu oeffnende dateien erzeugen
	puffer = new Eingangspuffer(this);
	
	// signal slot verbindungen
	// den puffer anschliessen
	connect(this, SIGNAL(oeffne_datei(const QString)), puffer, SLOT(fuege_befehl_hinzu(const QString&)));
	connect(puffer, SIGNAL(befehlsliste(const QStringList&)), this, SIGNAL(oeffne_dateien(const QStringList)));
}


EinzelAnwendungPrivateMac::~EinzelAnwendungPrivateMac()
{
}


bool EinzelAnwendungPrivateMac::event(QEvent *ev)
{
	bool angenommen = false;
	
	switch (ev->type())
	{
		case QEvent::FileOpen:
			
			emit oeffne_datei(static_cast<QFileOpenEvent *>(ev)->file());
			
			angenommen = true;
			
			break;
			
		default:
			
			angenommen = QApplication::event(ev);
			
			break;
	}
	
	return angenommen;
}


void EinzelAnwendungPrivateMac::installTranslator(QTranslator* uebersetzer)
{
	QApplication::installTranslator(uebersetzer);
}


bool EinzelAnwendungPrivateMac::exec()
{
	return QApplication::exec();
}


QStringList EinzelAnwendungPrivateMac::argumente() const
{
	QStringList erg(QApplication::arguments());
	
	if (erg.isEmpty() == false) erg.removeFirst();
	
	return erg;
}


QStringList EinzelAnwendungPrivateMac::arguments() const
{
	return QApplication::arguments();
}


bool EinzelAnwendungPrivateMac::ist_allgemein() const
{
	return false;
}


bool EinzelAnwendungPrivateMac::ist_linux() const
{
	return false;
}


bool EinzelAnwendungPrivateMac::ist_windows() const
{
	return false;
}


bool EinzelAnwendungPrivateMac::ist_mac() const
{
	return true;
}


bool EinzelAnwendungPrivateMac::ist_qapplication() const
{
	return false;
}


bool EinzelAnwendungPrivateMac::ist_qtsingleapplication() const
{
	return true;
}


QApplication* EinzelAnwendungPrivateMac::qapplication()
{
	return this;
}


QtSingleApplication* EinzelAnwendungPrivateMac::qtsingleapplication()
{
	return 0;
}


QString EinzelAnwendungPrivateMac::id() const
{
	return QString();
}


void EinzelAnwendungPrivateMac::setActivationWindow(QWidget*, bool)
{
}


QWidget* EinzelAnwendungPrivateMac::activationWindow() const
{
	return 0;
}


void EinzelAnwendungPrivateMac::activateWindow()
{
}


void EinzelAnwendungPrivateMac::datei_hinzufuegen(const QString&)
{
}


void EinzelAnwendungPrivateMac::dateien_hinzufuegen(const QStringList&)
{
}
