#include "EinzelAnwendungPrivateLinux.h"
#include "EinzelAnwendung.h"

using namespace std;

EinzelAnwendungPrivateLinux::EinzelAnwendungPrivateLinux(int& argc, char **argv, const QString& id, EinzelAnwendung* parent) : EinzelAnwendungPrivateAllgemein(argc, argv, id, parent)
{
}


EinzelAnwendungPrivateLinux::~EinzelAnwendungPrivateLinux()
{
}


bool EinzelAnwendungPrivateLinux::ist_allgemein() const
{
	return false;
}


bool EinzelAnwendungPrivateLinux::ist_linux() const
{
	return true;
}


bool EinzelAnwendungPrivateLinux::ist_windows() const
{
	return false;
}


bool EinzelAnwendungPrivateLinux::ist_mac() const
{
	return false;
}
