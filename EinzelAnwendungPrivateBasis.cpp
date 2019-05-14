#include "EinzelAnwendungPrivateBasis.h"
#include <QStringList>

using namespace std;

EinzelAnwendungPrivateBasis::EinzelAnwendungPrivateBasis(int&, char**, const QString&, EinzelAnwendung* parent) : meine_anwendung(parent)
{
}


EinzelAnwendungPrivateBasis::~EinzelAnwendungPrivateBasis()
{
}


EinzelAnwendung* EinzelAnwendungPrivateBasis::anwendung() const
{
	return meine_anwendung;
}


bool EinzelAnwendungPrivateBasis::exec()
{
	return false;
}


void EinzelAnwendungPrivateBasis::installTranslator(QTranslator*)
{
}


bool EinzelAnwendungPrivateBasis::isRunning()
{
	return false;
}


QStringList EinzelAnwendungPrivateBasis::argumente() const
{
	return QStringList();
}


void EinzelAnwendungPrivateBasis::sende_dateien()
{
}


QStringList EinzelAnwendungPrivateBasis::arguments() const
{
	return QStringList();
}


bool EinzelAnwendungPrivateBasis::ist_allgemein() const
{
	return false;
}


bool EinzelAnwendungPrivateBasis::ist_linux() const
{
	return false;
}


bool EinzelAnwendungPrivateBasis::ist_windows() const
{
	return false;
}


bool EinzelAnwendungPrivateBasis::ist_mac() const
{
	return false;
}


bool EinzelAnwendungPrivateBasis::ist_qapplication() const
{
	return false;
}


bool EinzelAnwendungPrivateBasis::ist_qtsingleapplication() const
{
	return false;
}


QApplication* EinzelAnwendungPrivateBasis::qapplication()
{
	return 0;
}


QtSingleApplication* EinzelAnwendungPrivateBasis::qtsingleapplication()
{
	return 0;
}


void EinzelAnwendungPrivateBasis::setActivationWindow(QWidget*, bool)
{
}


QWidget* EinzelAnwendungPrivateBasis::activationWindow() const
{
	return 0;
}


QString EinzelAnwendungPrivateBasis::id() const
{
	return QString();
}


void EinzelAnwendungPrivateBasis::activateWindow()
{
}


void EinzelAnwendungPrivateBasis::datei_hinzufuegen(const QString&)
{
}


void EinzelAnwendungPrivateBasis::dateien_hinzufuegen(const QStringList&)
{
}
