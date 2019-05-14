#include "EinzelAnwendungPrivateWindows.h"
#include "EinzelAnwendung.h"

using namespace std;

EinzelAnwendungPrivateWindows::EinzelAnwendungPrivateWindows(int& argc, char **argv, const QString& id, EinzelAnwendung* parent) : EinzelAnwendungPrivateAllgemein(argc, argv, id, parent)
{
}


EinzelAnwendungPrivateWindows::~EinzelAnwendungPrivateWindows()
{
}


bool EinzelAnwendungPrivateWindows::ist_allgemein() const
{
	return false;
}


bool EinzelAnwendungPrivateWindows::ist_linux() const
{
	return false;
}


bool EinzelAnwendungPrivateWindows::ist_windows() const
{
	return true;
}


bool EinzelAnwendungPrivateWindows::ist_mac() const
{
	return false;
}
