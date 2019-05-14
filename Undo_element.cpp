#include "Undo_element.h"

using namespace std;

Undo_element::Undo_element(int art_, const Position& position_, int zahl_, bool aktivitaet_) : art(art_), position(position_), zahl(zahl_), aktivitaet(aktivitaet_)
{
}


Undo_element::~Undo_element()
{
}


int Undo_element::get_art() const
{
	return art;
}


const Position& Undo_element::get_position() const
{
	return position;
}


int Undo_element::get_zahl() const
{
	return zahl;
}


bool Undo_element::get_aktivitaet() const
{
	return aktivitaet;
}
