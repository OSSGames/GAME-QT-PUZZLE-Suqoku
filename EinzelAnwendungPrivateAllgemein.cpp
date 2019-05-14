#include "EinzelAnwendungPrivateAllgemein.h"
#include "EinzelAnwendung.h"
#include "Eingangspuffer.h"

using namespace std;

EinzelAnwendungPrivateAllgemein::EinzelAnwendungPrivateAllgemein(int& argc, char **argv, const QString& id, EinzelAnwendung* parent) : QtSingleApplication(id, argc, argv), EinzelAnwendungPrivateBasis(argc, argv, id, parent), puffer(0)
{
	setParent(parent);
	setApplicationName(id);
	
	// den puffer fuer zu oeffnende dateien erzeugen
	puffer = new Eingangspuffer(this);
	
	// signal slot verbindungen
	// datei oeffnungsanweisungen ermoeglichen
	connect(this, SIGNAL(messageReceived(const QString&)), puffer, SLOT(fuege_befehl_hinzu(const QString&)));
	
	// das aktivieren des fensters ermoeglichen. dies ist unter macos nicht noetig, da es bereits von selbst geschieht
	connect(this, SIGNAL(oeffne_dateien(const QStringList&)), this, SLOT(activateWindow()));
	
	// den puffer anschliessen
	connect(this, SIGNAL(oeffne_datei(const QString)), puffer, SLOT(fuege_befehl_hinzu(const QString&)));
	connect(puffer, SIGNAL(befehlsliste(const QStringList&)), this, SIGNAL(oeffne_dateien(const QStringList)));
}


EinzelAnwendungPrivateAllgemein::~EinzelAnwendungPrivateAllgemein()
{
}


void EinzelAnwendungPrivateAllgemein::installTranslator(QTranslator* uebersetzer)
{
	QtSingleApplication::installTranslator(uebersetzer);
}


bool EinzelAnwendungPrivateAllgemein::exec()
{
	return QtSingleApplication::exec();
}


bool EinzelAnwendungPrivateAllgemein::isRunning()
{
	return QtSingleApplication::isRunning();
}


QStringList EinzelAnwendungPrivateAllgemein::argumente() const
{
	QStringList erg(QtSingleApplication::arguments());
	
	if (erg.isEmpty() == false) erg.removeFirst();
	
	return erg;
}


void EinzelAnwendungPrivateAllgemein::sende_dateien()
{
	QStringList tmp(argumente());
	
    for (int idx = 0; idx < tmp.size(); idx++) sendMessage(tmp.at(idx));
}


QStringList EinzelAnwendungPrivateAllgemein::arguments() const
{
	return QtSingleApplication::arguments();
}


bool EinzelAnwendungPrivateAllgemein::ist_allgemein() const
{
	return true;
}


bool EinzelAnwendungPrivateAllgemein::ist_linux() const
{
	return false;
}


bool EinzelAnwendungPrivateAllgemein::ist_windows() const
{
	return false;
}


bool EinzelAnwendungPrivateAllgemein::ist_mac() const
{
	return false;
}


bool EinzelAnwendungPrivateAllgemein::ist_qapplication() const
{
	return true;
}


bool EinzelAnwendungPrivateAllgemein::ist_qtsingleapplication() const
{
	return true;
}


QApplication* EinzelAnwendungPrivateAllgemein::qapplication()
{
	return this;
}


QtSingleApplication* EinzelAnwendungPrivateAllgemein::qtsingleapplication()
{
	return this;
}


QString EinzelAnwendungPrivateAllgemein::id() const
{
	return QtSingleApplication::id();
}


void EinzelAnwendungPrivateAllgemein::setActivationWindow(QWidget* aw, bool activateOnMessage)
{
	QtSingleApplication::setActivationWindow(aw, activateOnMessage);
}


QWidget* EinzelAnwendungPrivateAllgemein::activationWindow() const
{
	return QtSingleApplication::activationWindow();
}


void EinzelAnwendungPrivateAllgemein::activateWindow()
{
	QtSingleApplication::activateWindow();
}


void EinzelAnwendungPrivateAllgemein::datei_hinzufuegen(const QString& name)
{
	puffer->fuege_befehl_hinzu(name);
}


void EinzelAnwendungPrivateAllgemein::dateien_hinzufuegen(const QStringList& namen)
{
	puffer->fuege_befehle_hinzu(namen);
}
