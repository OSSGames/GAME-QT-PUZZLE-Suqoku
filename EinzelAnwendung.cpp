#include "EinzelAnwendung.h"
#include "EinzelAnwendungPrivateBasis.h"
#include "EinzelAnwendungPrivateAllgemein.h"
#include "EinzelAnwendungPrivateMac.h"
#include "EinzelAnwendungPrivateWindows.h"
#include "EinzelAnwendungPrivateLinux.h"

using namespace std;

EinzelAnwendung::EinzelAnwendung(int& argc, char **argv, const QString& id, QObject *parent) : QObject(parent), anwendung_private(0)
{
#ifdef Q_OS_MAC
	
	anwendung_private = new EinzelAnwendungPrivateMac(argc, argv, id, this);
	
	// signal - slot verbindungen
	connect((EinzelAnwendungPrivateMac*) anwendung_private, SIGNAL(oeffne_dateien(const QStringList)), this, SIGNAL(oeffne_dateien(const QStringList)));
	
#elif defined Q_OS_WIN
	
	anwendung_private = new EinzelAnwendungPrivateWindows(argc, argv, id, this);
	
	// signal - slot verbindungen
	connect((EinzelAnwendungPrivateWindows*) anwendung_private, SIGNAL(oeffne_dateien(const QStringList)), this, SIGNAL(oeffne_dateien(const QStringList)));
	
#elif defined Q_OS_LINUX
	
	anwendung_private = new EinzelAnwendungPrivateLinux(argc, argv, id, this);
	
	// signal - slot verbindungen
	connect((EinzelAnwendungPrivateLinux*) anwendung_private, SIGNAL(oeffne_dateien(const QStringList)), this, SIGNAL(oeffne_dateien(const QStringList)));
	
#else
	
	anwendung_private = new EinzelAnwendungPrivateAllgemein(argc, argv, id, this);
	
	// signal - slot verbindungen
	connect((EinzelAnwendungPrivateAllgemein*) anwendung_private, SIGNAL(oeffne_dateien(const QStringList)), this, SIGNAL(oeffne_dateien(const QStringList)));
	
#endif
	
	
	
}


EinzelAnwendung::~EinzelAnwendung()
{
}


void EinzelAnwendung::installTranslator(QTranslator* uebersetzer)
{
	anwendung_private->installTranslator(uebersetzer);
}


bool EinzelAnwendung::exec()
{
	return anwendung_private->exec();
}


bool EinzelAnwendung::isRunning()
{
	return anwendung_private->isRunning();
}


QStringList EinzelAnwendung::argumente() const
{
	return anwendung_private->argumente();
}


void EinzelAnwendung::sende_dateien()
{
	anwendung_private->sende_dateien();
}


QStringList EinzelAnwendung::arguments() const
{
	return anwendung_private->arguments();
}


bool EinzelAnwendung::ist_allgemein() const
{
	return anwendung_private->ist_allgemein();
}


bool EinzelAnwendung::ist_linux() const
{
	return anwendung_private->ist_linux();
}


bool EinzelAnwendung::ist_windows() const
{
	return anwendung_private->ist_windows();
}


bool EinzelAnwendung::ist_mac() const
{
	return anwendung_private->ist_mac();
}


bool EinzelAnwendung::ist_qapplication() const
{
	return anwendung_private->ist_qapplication();
}


bool EinzelAnwendung::ist_qtsingleapplication() const
{
	return anwendung_private->ist_qtsingleapplication();
}


QApplication* EinzelAnwendung::qapplication()
{
	return anwendung_private->qapplication();
}


QtSingleApplication* EinzelAnwendung::qtsingleapplication()
{
	return anwendung_private->qtsingleapplication();
}


QString EinzelAnwendung::id() const
{
	return anwendung_private->id();
}


void EinzelAnwendung::setActivationWindow(QWidget* aw, bool activateOnMessage)
{
	anwendung_private->setActivationWindow(aw, activateOnMessage);
}


QWidget* EinzelAnwendung::activationWindow() const
{
	return anwendung_private->activationWindow();
}


void EinzelAnwendung::activateWindow()
{
	anwendung_private->activateWindow();
}


void EinzelAnwendung::datei_hinzufuegen(const QString& name)
{
	anwendung_private->datei_hinzufuegen(name);
}


void EinzelAnwendung::dateien_hinzufuegen(const QStringList& namen)
{
	anwendung_private->dateien_hinzufuegen(namen);
}
