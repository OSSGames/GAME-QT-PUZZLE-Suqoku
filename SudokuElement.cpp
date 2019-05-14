#include "SudokuElement.h"
#include "Scene.h"
#include <QPainter>
#include <QDebug>
#include <QGraphicsSceneContextMenuEvent>
#include <QKeyEvent>
#include <QMenu>
#include <QAction>

#ifdef Q_OS_MAC

#define RECT_ZU_FONT 0.8

#else

#define RECT_ZU_FONT 0.625

#endif

#define RINGKLICK_KORREKTUR 3

using namespace std;

SudokuElement::SudokuElement(Scene *oparent, QGraphicsItem *gparent) : QObject(oparent), QGraphicsItem(gparent), Meinescene(oparent), aussenring(boundingRect().adjusted(2, 2, -2, -2)), ring1(aussenring.adjusted(0, 0, -40, -40).adjusted(1, 1, -1, -1)), ring2(aussenring.adjusted(20, 0, -20, -40).adjusted(1, 1, -1, -1)), ring3(aussenring.adjusted(40, 0, 0, -40).adjusted(1, 1, -1, -1)), ring4(aussenring.adjusted(0, 20, -40, -20).adjusted(1, 1, -1, -1)), ring5(aussenring.adjusted(20, 20, -20, -20).adjusted(1, 1, -1, -1)), ring6(aussenring.adjusted(40, 20, 0, -20).adjusted(1, 1, -1, -1)), ring7(aussenring.adjusted(0, 40, -40, 0).adjusted(1, 1, -1, -1)), ring8(aussenring.adjusted(20, 40, -20, 0).adjusted(1, 1, -1, -1)), ring9(aussenring.adjusted(40, 40, 0, 0).adjusted(1, 1, -1, -1)), Zahl(0), Tip(0), Filterung(0), Aktiver_farbmarker(0), meine_gruppe(0), vormals_ausgewaehlt(false), Sichtbarkeit_kandidaten(true)
{
	// das element auswaehlbar machen
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemIsFocusable, true);
	
	// die verfuegbarkeit der tipps aufbauen
	verfuegbarkeit.insert(1, true);
	verfuegbarkeit.insert(2, true);
	verfuegbarkeit.insert(3, true);
	verfuegbarkeit.insert(4, true);
	verfuegbarkeit.insert(5, true);
	verfuegbarkeit.insert(6, true);
	verfuegbarkeit.insert(7, true);
	verfuegbarkeit.insert(8, true);
	verfuegbarkeit.insert(9, true);
	
	// die ausschlussliste ausbauen
	ausgeschlossen.insert(1, false);
	ausgeschlossen.insert(2, false);
	ausgeschlossen.insert(3, false);
	ausgeschlossen.insert(4, false);
	ausgeschlossen.insert(5, false);
	ausgeschlossen.insert(6, false);
	ausgeschlossen.insert(7, false);
	ausgeschlossen.insert(8, false);
	ausgeschlossen.insert(9, false);
}


SudokuElement::~SudokuElement()
{
}


Scene* SudokuElement::meinescene() const
{
	return Meinescene;
}


void SudokuElement::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	QPen stift(painter->pen());
	
	
	
	/* *** *** *** farbmarkierungen malen *** *** *** */
	switch (Aktiver_farbmarker)
	{
		case SCENE_FILTER_RED:
			
			stift.setColor(Qt::red);
			painter->setPen(stift);
			painter->setBrush(Qt::red);
			
			painter->drawRect(aussenring);
			
			break;
			
		case SCENE_FILTER_GREEN:
			
			stift.setColor(Qt::green);
			painter->setPen(stift);
			painter->setBrush(Qt::green);
			
			painter->drawRect(aussenring);
			
			break;
			
		case SCENE_FILTER_BLUE:
			
			stift.setColor(Qt::blue);
			painter->setPen(stift);
			painter->setBrush(Qt::blue);
			
			painter->drawRect(aussenring);
			
			break;
			
		case SCENE_FILTER_CYAN:
			
			stift.setColor(Qt::cyan);
			painter->setPen(stift);
			painter->setBrush(Qt::cyan);
			
			painter->drawRect(aussenring);
			
			break;
			
		case SCENE_FILTER_MAGENTA:
			
			stift.setColor(Qt::magenta);
			painter->setPen(stift);
			painter->setBrush(Qt::magenta);
			
			painter->drawRect(aussenring);
			
			break;
			
		case SCENE_FILTER_YELLOW:
			
			stift.setColor(Qt::yellow);
			painter->setPen(stift);
			painter->setBrush(Qt::yellow);
			
			painter->drawRect(aussenring);
			
			break;
	}
	
	// den stift wieder zuruecksetzen
	stift.setColor(Qt::black);
	stift.setWidth(1);
	painter->setPen(stift);
	painter->setBrush(Qt::NoBrush);
	
	
	
	/* *** *** *** filter malen *** *** *** */
	// den noetigen bushtyp erfassen
	Qt::BrushStyle brushtyp = Qt::SolidPattern;
	if (Aktiver_farbmarker != SCENE_FILTER_NICHTS) brushtyp = Qt::Dense3Pattern;
	
	QList<int> moeglichkeiten(verfuegbarkeit.keys(true));
	QList<int> ausschluesse(ausgeschlossen.keys(true));
	
    for (int idx = 0; idx < ausschluesse.size(); idx++) moeglichkeiten.removeAll(ausschluesse.at(idx));
	
	switch (Filterung)
	{
		case SCENE_FILTER_1:
			
			if (moeglichkeiten.contains(1) == true && ist_zahl() == false && ist_tip() == false)
			{
				stift.setColor(Qt::lightGray);
				painter->setPen(stift);
				painter->setBrush(QBrush(Qt::lightGray, brushtyp));
				
				painter->drawRect(aussenring);
			}
			
			break;
			
		case SCENE_FILTER_2:
			
			if (moeglichkeiten.contains(2) == true && ist_zahl() == false && ist_tip() == false)
			{
				stift.setColor(Qt::lightGray);
				painter->setPen(stift);
				painter->setBrush(QBrush(Qt::lightGray, brushtyp));
				
				painter->drawRect(aussenring);
			}
			
			break;
			
		case SCENE_FILTER_3:
			
			if (moeglichkeiten.contains(3) == true && ist_zahl() == false && ist_tip() == false)
			{
				stift.setColor(Qt::lightGray);
				painter->setPen(stift);
				painter->setBrush(QBrush(Qt::lightGray, brushtyp));
				
				painter->drawRect(aussenring);
			}
			
			break;
			
		case SCENE_FILTER_4:
			
			if (moeglichkeiten.contains(4) == true && ist_zahl() == false && ist_tip() == false)
			{
				stift.setColor(Qt::lightGray);
				painter->setPen(stift);
				painter->setBrush(QBrush(Qt::lightGray, brushtyp));
				
				painter->drawRect(aussenring);
			}
			
			break;
			
		case SCENE_FILTER_5:
			
			if (moeglichkeiten.contains(5) == true && ist_zahl() == false && ist_tip() == false)
			{
				stift.setColor(Qt::lightGray);
				painter->setPen(stift);
				painter->setBrush(QBrush(Qt::lightGray, brushtyp));
				
				painter->drawRect(aussenring);
			}
			
			break;
			
		case SCENE_FILTER_6:
			
			if (moeglichkeiten.contains(6) == true && ist_zahl() == false && ist_tip() == false)
			{
				stift.setColor(Qt::lightGray);
				painter->setPen(stift);
				painter->setBrush(QBrush(Qt::lightGray, brushtyp));
				
				painter->drawRect(aussenring);
			}
			
			break;
			
		case SCENE_FILTER_7:
			
			if (moeglichkeiten.contains(7) == true && ist_zahl() == false && ist_tip() == false)
			{
				stift.setColor(Qt::lightGray);
				painter->setPen(stift);
				painter->setBrush(QBrush(Qt::lightGray, brushtyp));
				
				painter->drawRect(aussenring);
			}
			
			break;
			
		case SCENE_FILTER_8:
			
			if (moeglichkeiten.contains(8) == true && ist_zahl() == false && ist_tip() == false)
			{
				stift.setColor(Qt::lightGray);
				painter->setPen(stift);
				painter->setBrush(QBrush(Qt::lightGray, brushtyp));
				
				painter->drawRect(aussenring);
			}
			
			break;
			
		case SCENE_FILTER_9:
			
			if (moeglichkeiten.contains(9) == true && ist_zahl() == false && ist_tip() == false)
			{
				stift.setColor(Qt::lightGray);
				painter->setPen(stift);
				painter->setBrush(QBrush(Qt::lightGray, brushtyp));
				
				painter->drawRect(aussenring);
			}
			
			break;
			
		case SCENE_FILTER_RED:
			
			if (Aktiver_farbmarker == SCENE_FILTER_RED)
			{
				stift.setColor(Qt::red);
				painter->setPen(stift);
				painter->setBrush(QBrush(Qt::lightGray, brushtyp));
				
				painter->drawRect(aussenring);
			}
			
			break;
			
		case SCENE_FILTER_GREEN:
			
			if (Aktiver_farbmarker == SCENE_FILTER_GREEN)
			{
				stift.setColor(Qt::green);
				painter->setPen(stift);
				painter->setBrush(QBrush(Qt::lightGray, brushtyp));
				
				painter->drawRect(aussenring);
			}
			
			break;
			
		case SCENE_FILTER_BLUE:
			
			if (Aktiver_farbmarker == SCENE_FILTER_BLUE)
			{
				stift.setColor(Qt::blue);
				painter->setPen(stift);
				painter->setBrush(QBrush(Qt::lightGray, brushtyp));
				
				painter->drawRect(aussenring);
			}
			
			break;
			
		case SCENE_FILTER_CYAN:
			
			if (Aktiver_farbmarker == SCENE_FILTER_CYAN)
			{
				stift.setColor(Qt::cyan);
				painter->setPen(stift);
				painter->setBrush(QBrush(Qt::lightGray, brushtyp));
				
				painter->drawRect(aussenring);
			}
			
			break;
			
		case SCENE_FILTER_MAGENTA:
			
			if (Aktiver_farbmarker == SCENE_FILTER_MAGENTA)
			{
				stift.setColor(Qt::magenta);
				painter->setPen(stift);
				painter->setBrush(QBrush(Qt::lightGray, brushtyp));
				
				painter->drawRect(aussenring);
			}
			
			break;
			
		case SCENE_FILTER_YELLOW:
			
			if (Aktiver_farbmarker == SCENE_FILTER_YELLOW)
			{
				stift.setColor(Qt::yellow);
				painter->setPen(stift);
				painter->setBrush(QBrush(Qt::lightGray, brushtyp));
				
				painter->drawRect(aussenring);
			}
			
			break;
			
		case SCENE_FILTER_PAARE:
			
			if (zahl_bekannt() == false && ist_tip() == false && verfuegbarkeit.keys(true).size() >= 2)
			{
				QList<int> paare(verfuegbarkeit.keys(true));
				QList<int> tmp_ausgeschlossen(ausgeschlossen.keys(true));
				
                for (int idx = 0; idx < tmp_ausgeschlossen.size(); idx++) paare.removeAll(tmp_ausgeschlossen.at(idx));
				
				if (paare.size() == 2)
				{
					stift.setColor(Qt::lightGray);
					painter->setPen(stift);
					painter->setBrush(QBrush(Qt::lightGray, brushtyp));
					
					painter->drawRect(aussenring);
				}
			}
			
			break;
			
		case SCENE_FILTER_DREIER:
			
			if (zahl_bekannt() == false && ist_tip() == false && verfuegbarkeit.keys(true).size() >= 3)
			{
				QList<int> paare(verfuegbarkeit.keys(true));
				QList<int> tmp_ausgeschlossen(ausgeschlossen.keys(true));
				
                for (int idx = 0; idx < tmp_ausgeschlossen.size(); idx++) paare.removeAll(tmp_ausgeschlossen.at(idx));
				
				if (paare.size() == 3)
				{
					stift.setColor(Qt::lightGray);
					painter->setPen(stift);
					painter->setBrush(QBrush(Qt::lightGray, brushtyp));
					
					painter->drawRect(aussenring);
				}
			}
			
			break;
	}
	
	
	
	/* *** *** *** aussenring malen *** *** *** */
	stift.setColor(Qt::darkGray);
	stift.setWidth(1);
	painter->setPen(stift);
	painter->setBrush(Qt::NoBrush);
	painter->drawRect(aussenring);
	
	// den stift wieder zuruecksetzen
	stift.setColor(Qt::black);
	painter->setPen(stift);
	
	
	
	/* *** *** *** bekannte zahlen malen *** *** *** */
	if (zahl_bekannt() == true)
	{
		QFont font(painter->font());
		stift.setColor(Qt::darkGreen);
		font.setPointSizeF(aussenring.height() * RECT_ZU_FONT);
		painter->setPen(stift);
		painter->setFont(font);
		
		painter->drawText(aussenring, Qt::AlignCenter, QString::number(Zahl));
	}
	
	// wenn ein tip abgegeben wurde den tip malen
	else if (ist_tip() == true)
	{
		QFont font(painter->font());
		stift.setColor(Qt::black);
		font.setPointSizeF(aussenring.height() * RECT_ZU_FONT);
		painter->setPen(stift);
		painter->setFont(font);
		
		painter->drawText(aussenring, Qt::AlignCenter, QString::number(Tip));
	}
	
	
	
	/* *** *** *** kandidaten malen *** *** *** */
	else
	{
		// den stift wieder zuruecksetzen
		painter->setBrush(Qt::NoBrush);
		
		if (Sichtbarkeit_kandidaten == true)
		{
			QFont font(painter->font());
			stift.setColor(Qt::black);
			font.setPointSizeF(ring1.height() * RECT_ZU_FONT);
			painter->setPen(stift);
			painter->setFont(font);
			
			bool noch_etwas_moeglich = false;
			
			// wenn die 1 verfuegbar ist
			if (tip_verfuegbar(1) == true)
			{
				painter->drawText(ring1, Qt::AlignCenter, "1");
				
				// wenn der kandidat gerade gefiltert wird einen dunkel gruenen ring um den kandidaten malen
				if (Filterung == SCENE_FILTER_1)
				{
					stift.setColor(Qt::darkGreen);
					painter->setPen(stift);
					
					painter->drawRect(ring1);
					
					stift.setColor(Qt::black);
					painter->setPen(stift);
				}
				
				if (tip_ausgeschlossen(1) == false) noch_etwas_moeglich = true;
				else
				{
					stift.setColor(Qt::darkRed);
					stift.setWidth(2);
					painter->setPen(stift);
					
					painter->drawLine(ring1.adjusted(2, 2, -2, -2).topLeft(), ring1.adjusted(2, 2, -2, -2).bottomRight());
					painter->drawLine(ring1.adjusted(2, 2, -2, -2).topRight(), ring1.adjusted(2, 2, -2, -2).bottomLeft());
					
					stift.setColor(Qt::black);
					painter->setPen(stift);
				}
			}
			
			// wenn die 2 verfuegbar ist
			if (tip_verfuegbar(2) == true)
			{
				painter->drawText(ring2, Qt::AlignCenter, "2");
				
				// wenn der kandidat gerade gefiltert wird einen dunkel gruenen ring um den kandidaten malen
				if (Filterung == SCENE_FILTER_2)
				{
					stift.setColor(Qt::darkGreen);
					painter->setPen(stift);
					
					painter->drawRect(ring2);
					
					stift.setColor(Qt::black);
					painter->setPen(stift);
				}
				
				if (tip_ausgeschlossen(2) == false) noch_etwas_moeglich = true;
				else
				{
					stift.setColor(Qt::darkRed);
					stift.setWidth(2);
					painter->setPen(stift);
					
					painter->drawLine(ring2.adjusted(2, 2, -2, -2).topLeft(), ring2.adjusted(2, 2, -2, -2).bottomRight());
					painter->drawLine(ring2.adjusted(2, 2, -2, -2).topRight(), ring2.adjusted(2, 2, -2, -2).bottomLeft());
					
					stift.setColor(Qt::black);
					painter->setPen(stift);
				}
			}
			
			// wenn die 3 verfuegbar ist
			if (tip_verfuegbar(3) == true)
			{
				painter->drawText(ring3, Qt::AlignCenter, "3");
				
				// wenn der kandidat gerade gefiltert wird einen dunkel gruenen ring um den kandidaten malen
				if (Filterung == SCENE_FILTER_3)
				{
					stift.setColor(Qt::darkGreen);
					painter->setPen(stift);
					
					painter->drawRect(ring3);
					
					stift.setColor(Qt::black);
					painter->setPen(stift);
				}
				
				if (tip_ausgeschlossen(3) == false) noch_etwas_moeglich = true;
				else
				{
					stift.setColor(Qt::darkRed);
					stift.setWidth(2);
					painter->setPen(stift);
					
					painter->drawLine(ring3.adjusted(2, 2, -2, -2).topLeft(), ring3.adjusted(2, 2, -2, -2).bottomRight());
					painter->drawLine(ring3.adjusted(2, 2, -2, -2).topRight(), ring3.adjusted(2, 2, -2, -2).bottomLeft());
					
					stift.setColor(Qt::black);
					painter->setPen(stift);
				}
			}
			
			// wenn die 4 verfuegbar ist
			if (tip_verfuegbar(4) == true)
			{
				painter->drawText(ring4, Qt::AlignCenter, "4");
				
				// wenn der kandidat gerade gefiltert wird einen dunkel gruenen ring um den kandidaten malen
				if (Filterung == SCENE_FILTER_4)
				{
					stift.setColor(Qt::darkGreen);
					painter->setPen(stift);
					
					painter->drawRect(ring4);
					
					stift.setColor(Qt::black);
					painter->setPen(stift);
				}
				
				if (tip_ausgeschlossen(4) == false) noch_etwas_moeglich = true;
				else
				{
					stift.setColor(Qt::darkRed);
					stift.setWidth(2);
					painter->setPen(stift);
					
					painter->drawLine(ring4.adjusted(2, 2, -2, -2).topLeft(), ring4.adjusted(2, 2, -2, -2).bottomRight());
					painter->drawLine(ring4.adjusted(2, 2, -2, -2).topRight(), ring4.adjusted(2, 2, -2, -2).bottomLeft());
					
					stift.setColor(Qt::black);
					painter->setPen(stift);
				}
			}
			
			// wenn die 5 verfuegbar ist
			if (tip_verfuegbar(5) == true)
			{
				painter->drawText(ring5, Qt::AlignCenter, "5");
				
				// wenn der kandidat gerade gefiltert wird einen dunkel gruenen ring um den kandidaten malen
				if (Filterung == SCENE_FILTER_5)
				{
					stift.setColor(Qt::darkGreen);
					painter->setPen(stift);
					
					painter->drawRect(ring5);
					
					stift.setColor(Qt::black);
					painter->setPen(stift);
				}
				
				if (tip_ausgeschlossen(5) == false) noch_etwas_moeglich = true;
				else
				{
					stift.setColor(Qt::darkRed);
					stift.setWidth(2);
					painter->setPen(stift);
					
					painter->drawLine(ring5.adjusted(2, 2, -2, -2).topLeft(), ring5.adjusted(2, 2, -2, -2).bottomRight());
					painter->drawLine(ring5.adjusted(2, 2, -2, -2).topRight(), ring5.adjusted(2, 2, -2, -2).bottomLeft());
					
					stift.setColor(Qt::black);
					painter->setPen(stift);
				}
			}
			
			// wenn die 6 verfuegbar ist
			if (tip_verfuegbar(6) == true)
			{
				painter->drawText(ring6, Qt::AlignCenter, "6");
				
				// wenn der kandidat gerade gefiltert wird einen dunkel gruenen ring um den kandidaten malen
				if (Filterung == SCENE_FILTER_6)
				{
					stift.setColor(Qt::darkGreen);
					painter->setPen(stift);
					
					painter->drawRect(ring6);
					
					stift.setColor(Qt::black);
					painter->setPen(stift);
				}
				
				if (tip_ausgeschlossen(6) == false) noch_etwas_moeglich = true;
				else
				{
					stift.setColor(Qt::darkRed);
					stift.setWidth(2);
					painter->setPen(stift);
					
					painter->drawLine(ring6.adjusted(2, 2, -2, -2).topLeft(), ring6.adjusted(2, 2, -2, -2).bottomRight());
					painter->drawLine(ring6.adjusted(2, 2, -2, -2).topRight(), ring6.adjusted(2, 2, -2, -2).bottomLeft());
					
					stift.setColor(Qt::black);
					painter->setPen(stift);
				}
			}
			
			// wenn die 7 verfuegbar ist
			if (tip_verfuegbar(7) == true)
			{
				painter->drawText(ring7, Qt::AlignCenter, "7");
				
				// wenn der kandidat gerade gefiltert wird einen dunkel gruenen ring um den kandidaten malen
				if (Filterung == SCENE_FILTER_7)
				{
					stift.setColor(Qt::darkGreen);
					painter->setPen(stift);
					
					painter->drawRect(ring7);
					
					stift.setColor(Qt::black);
					painter->setPen(stift);
				}
				
				if (tip_ausgeschlossen(7) == false) noch_etwas_moeglich = true;
				else
				{
					stift.setColor(Qt::darkRed);
					stift.setWidth(2);
					painter->setPen(stift);
					
					painter->drawLine(ring7.adjusted(2, 2, -2, -2).topLeft(), ring7.adjusted(2, 2, -2, -2).bottomRight());
					painter->drawLine(ring7.adjusted(2, 2, -2, -2).topRight(), ring7.adjusted(2, 2, -2, -2).bottomLeft());
					
					stift.setColor(Qt::black);
					painter->setPen(stift);
				}
			}
			
			// wenn die 8 verfuegbar ist
			if (tip_verfuegbar(8) == true)
			{
				painter->drawText(ring8, Qt::AlignCenter, "8");
				
				// wenn der kandidat gerade gefiltert wird einen dunkel gruenen ring um den kandidaten malen
				if (Filterung == SCENE_FILTER_8)
				{
					stift.setColor(Qt::darkGreen);
					painter->setPen(stift);
					
					painter->drawRect(ring8);
					
					stift.setColor(Qt::black);
					painter->setPen(stift);
				}
				
				if (tip_ausgeschlossen(8) == false) noch_etwas_moeglich = true;
				else
				{
					stift.setColor(Qt::darkRed);
					stift.setWidth(2);
					painter->setPen(stift);
					
					painter->drawLine(ring8.adjusted(2, 2, -2, -2).topLeft(), ring8.adjusted(2, 2, -2, -2).bottomRight());
					painter->drawLine(ring8.adjusted(2, 2, -2, -2).topRight(), ring8.adjusted(2, 2, -2, -2).bottomLeft());
					
					stift.setColor(Qt::black);
					painter->setPen(stift);
				}
			}
			
			// wenn die 9 verfuegbar ist
			if (tip_verfuegbar(9) == true)
			{
				painter->drawText(ring9, Qt::AlignCenter, "9");
				
				// wenn der kandidat gerade gefiltert wird einen dunkel gruenen ring um den kandidaten malen
				if (Filterung == SCENE_FILTER_9)
				{
					stift.setColor(Qt::darkGreen);
					painter->setPen(stift);
					
					painter->drawRect(ring9);
					
					stift.setColor(Qt::black);
					painter->setPen(stift);
				}
				
				if (tip_ausgeschlossen(9) == false) noch_etwas_moeglich = true;
				else
				{
					stift.setColor(Qt::darkRed);
					stift.setWidth(2);
					painter->setPen(stift);
					
					painter->drawLine(ring9.adjusted(2, 2, -2, -2).topLeft(), ring9.adjusted(2, 2, -2, -2).bottomRight());
					painter->drawLine(ring9.adjusted(2, 2, -2, -2).topRight(), ring9.adjusted(2, 2, -2, -2).bottomLeft());
					
					stift.setColor(Qt::black);
					painter->setPen(stift);
				}
			}
			
			
			
			/* *** *** *** fehler malen *** *** *** */
			if (noch_etwas_moeglich == false)
			{
				stift.setColor(Qt::darkRed);
				stift.setWidth(2);
				painter->setPen(stift);
				painter->setBrush(Qt::NoBrush);
				
				QLineF vorbereitung_linie_01(aussenring.topLeft(), aussenring.bottomRight());
				vorbereitung_linie_01.setLength(vorbereitung_linie_01.length()  - (stift.width() / 2.0));
				QLineF linie_01(vorbereitung_linie_01.p2(), vorbereitung_linie_01.p1());
				linie_01.setLength(linie_01.length()  - (stift.width() / 2.0));
				
				painter->drawLine(linie_01);
				
				QLineF vorbereitung_linie_02(aussenring.topRight(), aussenring.bottomLeft());
				vorbereitung_linie_02.setLength(vorbereitung_linie_02.length()  - (stift.width() / 2.0));
				QLineF linie_02(vorbereitung_linie_02.p2(), vorbereitung_linie_02.p1());
				linie_02.setLength(linie_02.length()  - (stift.width() / 2.0));
				
				painter->drawLine(linie_02);
			}
		}
	}
	
	
	
	/* *** *** *** markierung wenn aktiviert malen *** *** *** */
	if (isSelected() == true)
	{
		stift.setColor(Qt::darkRed);
		stift.setWidth(2);
		painter->setPen(stift);
		painter->setBrush(Qt::NoBrush);
		
		painter->drawRect(aussenring);
	}
}


QRectF SudokuElement::boundingRect() const
{
	return QRectF(0, 0, ELEMENT_GROESSE, ELEMENT_GROESSE);
}


int SudokuElement::zahl() const
{
	return Zahl;
}


int SudokuElement::tip() const
{
	return Tip;
}


bool SudokuElement::setze_zahl(int wert)
{
	bool erg = false;
	
	if (wert >= 1 && wert <=9 && ist_tip() == false && ist_zahl() == false && verfuegbarkeit.value(wert) == true && ausgeschlossen.value(wert) == false && Meinescene->spiel_gewonnen() == false)
	{
		// das element nicht laenger auswaehlbar machen
		setFlag(QGraphicsItem::ItemIsSelectable, false);
		setFlag(QGraphicsItem::ItemIsFocusable, false);
		
		Zahl = wert;
		
		Meinescene->aktualisiere_verfuegbarkeiten(this, Zahl);
		
		emit zahl_gesetzt(this, Zahl);
		
		update();
		
		erg = true;
	}
	
	else qDebug() << tr("Error in \"bool SudokuElement::setze_zahl(int wert)\"");
	
	return erg;
}

bool SudokuElement::setze_tip(int wert, bool senden)
{
	bool erg = false;
	
	if (wert >= 1 && wert <=9 && ist_zahl() == false && verfuegbarkeit.value(wert) == true && ausgeschlossen.value(wert) == false && ist_tip() == false && Meinescene->spiel_gewonnen() == false)
	{
		Tip = wert;
		
		Meinescene->aktualisiere_verfuegbarkeiten(this, Tip);
		
		if (senden == true) emit tipp_abgegeben(this, Tip);
		
		update();
		
		erg = true;
	}
	
	else qDebug() << tr("Error in \"void SudokuElement::setze_tip(int wert)\"");
	
	return erg;
}

bool SudokuElement::zahl_bekannt() const
{
	bool erg = false;
	
	if (Zahl != 0) erg = true;
	
	return erg;
}


bool SudokuElement::tip_verfuegbar(int zahl_) const
{
	bool erg = false;
	
	if (verfuegbarkeit.contains(zahl_) == true) erg = verfuegbarkeit.value(zahl_);
	
	else qDebug() << tr("Error in \"bool SudokuElement::zahl_verfuegbar(int zahl_) const\"");
	
	return erg;
}


void SudokuElement::setze_verfuegbarkeit_tip(int zahl_, bool zustand)
{
	if (zahl_ >= 1 && zahl_ <= 9)
	{
		// wenn die verfuegbarkeit eines kandidaten geloescht wird oder wenn der kandidat wirklich moeglich ist
		if (zustand == false || (Meinescene->zahlen_und_tips_in_reihe(this).contains(zahl_) == false && Meinescene->zahlen_und_tips_in_spalte(this).contains(zahl_) == false && Meinescene->zahlen_und_tips_in_gruppe(this).contains(zahl_) == false))
		{
			verfuegbarkeit.insert(zahl_, zustand);
			
			if (zustand == false && ausgeschlossen.value(zahl_) == true) ausgeschlossen.insert(zahl_, zustand);
			
			update();
		}
	}
	
	else qDebug() << tr("Impossible value in \"void SudokuElement::setze_verfuegbarkeit_tip(int zahl_, bool zustand)\"");
}


void SudokuElement::setze_verfuegbarkeit_tip_zurueck()
{
	verfuegbarkeit.insert(1, true);
	verfuegbarkeit.insert(2, true);
	verfuegbarkeit.insert(3, true);
	verfuegbarkeit.insert(4, true);
	verfuegbarkeit.insert(5, true);
	verfuegbarkeit.insert(6, true);
	verfuegbarkeit.insert(7, true);
	verfuegbarkeit.insert(8, true);
	verfuegbarkeit.insert(9, true);
	
	update();
}


bool SudokuElement::ist_zahl() const
{
	bool erg = false;
	
	if (Zahl != 0) erg = true;
	
	return erg;
}


bool SudokuElement::ist_tip() const
{
	bool erg = false;
	
	if (Tip != 0) erg = true;
	
	return erg;
}


bool SudokuElement::tip_ausgeschlossen(int wert) const
{
	bool erg = false;
	
	if (ausgeschlossen.contains(wert) == true) erg = ausgeschlossen.value(wert);
	
	else qDebug() << tr("Error in \"bool SudokuElement::zahl_ausgeschlossen(int wert) const\"");
	
	return erg;
}


void SudokuElement::setze_ausschluss_tip(int zahl_, bool zustand, bool senden)
{
	if (ausgeschlossen.contains(zahl_) == true && Meinescene->spiel_gewonnen() == false)
	{
		ausgeschlossen.insert(zahl_, zustand);
		
		if (senden == true)
		{
			if (zustand == true) emit kandidat_gesperrt(this, zahl_);
			else emit kandidat_entsperrt(this, zahl_);
		}
	}
	
	else qDebug() << tr("Error in \"void SudokuElement::setze_ausschluss_tip(int zahl_, bool zustand)\"");
	
	update();
}


int SudokuElement::zahl_und_tip() const
{
	int erg = 0;
	
	if (ist_zahl() == true) erg = Zahl;
	else if (ist_tip() == true) erg = Tip;
	
	return erg;
}


void SudokuElement::registriere_gruppe(QMap<Position, SudokuElement*>* liste)
{
	meine_gruppe = liste;
}


QMap<Position, SudokuElement*>* SudokuElement::gruppe() const
{
	return meine_gruppe;
}


void SudokuElement::setze_zahl_zurueck()
{
	// das element auswaehlbar machen
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemIsFocusable, true);
	
	int tmp = Zahl;
	Zahl = 0;
	
	Meinescene->setze_verfuegbarkeiten_zurueck(this, tmp);
	emit zahl_zurueckgesetzt(this, tmp);
	
	update();
}


void SudokuElement::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
	// nur, wenn noch nicht gewonnen
	if (Meinescene->spiel_gewonnen() == false)
	{
		QMenu kontextmenu;
		
		// ein zur situation passendes kontextmenu erstellen und anzeigen
		
		// wenn die zahl bekannt ist nur die markierung und unmarkierung anbieten
		if (zahl_bekannt() == true)
		{
			QAction *action_red = 0, *action_green = 0, *action_blue = 0, *action_cyan = 0, *action_magenta = 0, *action_yellow = 0, *action_unmark = 0;
			
			if (Aktiver_farbmarker != SCENE_FILTER_RED) action_red = kontextmenu.addAction(tr("Mark red"));
			if (Aktiver_farbmarker != SCENE_FILTER_GREEN) action_green = kontextmenu.addAction(tr("Mark green"));
			if (Aktiver_farbmarker != SCENE_FILTER_BLUE) action_blue = kontextmenu.addAction(tr("Mark blue"));
			if (Aktiver_farbmarker != SCENE_FILTER_CYAN) action_cyan = kontextmenu.addAction(tr("Mark cyan"));
			if (Aktiver_farbmarker != SCENE_FILTER_MAGENTA) action_magenta = kontextmenu.addAction(tr("Mark magenta"));
			if (Aktiver_farbmarker != SCENE_FILTER_YELLOW) action_yellow = kontextmenu.addAction(tr("Mark yellow"));
			
			if (action_red == 0 || action_green == 0 || action_blue == 0 || action_cyan == 0 || action_magenta == 0 || action_yellow == 0)
			{
				kontextmenu.addSeparator();
				
				action_unmark = kontextmenu.addAction(tr("Remove color mark"));
			}
			
			// das kontextmenu anzeigen und die gewaehlte aktion speichern
			QAction *gewaelte_action = kontextmenu.exec(event->screenPos());
			
			// die gewaehlte aktion verarbeiten
			if (gewaelte_action != 0)
			{
				if (gewaelte_action == action_red) setze_farbmarkierung(SCENE_FILTER_RED);
				else if (gewaelte_action == action_green) setze_farbmarkierung(SCENE_FILTER_GREEN);
				else if (gewaelte_action == action_blue) setze_farbmarkierung(SCENE_FILTER_BLUE);
				else if (gewaelte_action == action_cyan) setze_farbmarkierung(SCENE_FILTER_CYAN);
				else if (gewaelte_action == action_magenta) setze_farbmarkierung(SCENE_FILTER_MAGENTA);
				else if (gewaelte_action == action_yellow) setze_farbmarkierung(SCENE_FILTER_YELLOW);
				else if (gewaelte_action == action_unmark) setze_farbmarkierung(SCENE_FILTER_NICHTS);
				else qDebug() << tr("Unknown color action in \"void SudokuElement::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)\"");
			}
		}
		
		// wenn es sich um einen tipp handelt das zuruecknehmen des tipps und markierungen und unmarkierungen ermoeglichen
		else if (ist_tip() == true)
		{
			QAction *tip_zurueck_action = kontextmenu.addAction(tr("Take back"));
			
			kontextmenu.addSeparator();
			
			QAction *action_red = 0, *action_green = 0, *action_blue = 0, *action_cyan = 0, *action_magenta = 0, *action_yellow = 0, *action_unmark = 0;
			
			if (Aktiver_farbmarker != SCENE_FILTER_RED) action_red = kontextmenu.addAction(tr("Mark red"));
			if (Aktiver_farbmarker != SCENE_FILTER_GREEN) action_green = kontextmenu.addAction(tr("Mark green"));
			if (Aktiver_farbmarker != SCENE_FILTER_BLUE) action_blue = kontextmenu.addAction(tr("Mark blue"));
			if (Aktiver_farbmarker != SCENE_FILTER_CYAN) action_cyan = kontextmenu.addAction(tr("Mark cyan"));
			if (Aktiver_farbmarker != SCENE_FILTER_MAGENTA) action_magenta = kontextmenu.addAction(tr("Mark magenta"));
			if (Aktiver_farbmarker != SCENE_FILTER_YELLOW) action_yellow = kontextmenu.addAction(tr("Mark yellow"));
			
			if (action_red == 0 || action_green == 0 || action_blue == 0 || action_cyan == 0 || action_magenta == 0 || action_yellow == 0)
			{
				kontextmenu.addSeparator();
				
				action_unmark = kontextmenu.addAction(tr("Remove color mark"));
			}
			
			// das kontextmenu anzeigen und die gewaehlte aktion speichern
			QAction *gewaelte_action = kontextmenu.exec(event->screenPos());
			
			if (gewaelte_action != 0)
			{
				// die gewaehlte aktion verarbeiten
				if (gewaelte_action == tip_zurueck_action) setze_tip_zurueck();
				else if (gewaelte_action == action_red) setze_farbmarkierung(SCENE_FILTER_RED);
				else if (gewaelte_action == action_green) setze_farbmarkierung(SCENE_FILTER_GREEN);
				else if (gewaelte_action == action_blue) setze_farbmarkierung(SCENE_FILTER_BLUE);
				else if (gewaelte_action == action_cyan) setze_farbmarkierung(SCENE_FILTER_CYAN);
				else if (gewaelte_action == action_magenta) setze_farbmarkierung(SCENE_FILTER_MAGENTA);
				else if (gewaelte_action == action_yellow) setze_farbmarkierung(SCENE_FILTER_YELLOW);
				else if (gewaelte_action == action_unmark) setze_farbmarkierung(SCENE_FILTER_NICHTS);
				else qDebug() << tr("Unknown color action in \"void SudokuElement::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)\"");
			}
		}
		
		// wenn noch getippt werden muss
		else if (zahl_bekannt() == false && ist_tip() == false)
		{
			QMap<QAction*, int> tip_aktionen;
			QMap<QAction*, int> ausschluss_setzen_actionen;
			QMap<QAction*, int> ausschluss_zuruecksetzen_actionen;
			QMap<QAction*, int> farbmarkierung_actionen;
			
			// tipps anbieten
			for (int idx = 1; idx <= 9; idx++)
			{
				if (verfuegbarkeit.value(idx) == true && ausgeschlossen.value(idx) == false)
				{
					QAction *tmp_action = kontextmenu.addAction(tr("Make %1").arg(idx));
					
					tip_aktionen.insert(tmp_action, idx);
				}
			}
			
			kontextmenu.addSeparator();
			
			// ausschluesse anbieten
			for (int idx = 1; idx <= 9; idx++)
			{
				if (ausgeschlossen.value(idx) == false && verfuegbarkeit.value(idx) == true)
				{
					QAction *tmp_action = kontextmenu.addAction(tr("Exclude %1").arg(idx));
					
					ausschluss_setzen_actionen.insert(tmp_action, idx);
				}
			}
			
			kontextmenu.addSeparator();
			
			// ruecknahme ausschluesse anbieten
			for (int idx = 1; idx <= 9; idx++)
			{
				if (ausgeschlossen.value(idx) == true && verfuegbarkeit.value(idx) == true)
				{
					QAction *tmp_action = kontextmenu.addAction(tr("Undo Exclude %1").arg(idx));
					
					ausschluss_zuruecksetzen_actionen.insert(tmp_action, idx);
				}
			}
			
			kontextmenu.addSeparator();
			
			// aktionen fuer farbmarkierungen aufbauen
			QAction *action_red = 0, *action_green = 0, *action_blue = 0, *action_cyan = 0, *action_magenta = 0, *action_yellow = 0, *action_unmark = 0;
			
			if (Aktiver_farbmarker != SCENE_FILTER_RED) action_red = kontextmenu.addAction(tr("Mark red"));
			if (Aktiver_farbmarker != SCENE_FILTER_GREEN) action_green = kontextmenu.addAction(tr("Mark green"));
			if (Aktiver_farbmarker != SCENE_FILTER_BLUE) action_blue = kontextmenu.addAction(tr("Mark blue"));
			if (Aktiver_farbmarker != SCENE_FILTER_CYAN) action_cyan = kontextmenu.addAction(tr("Mark cyan"));
			if (Aktiver_farbmarker != SCENE_FILTER_MAGENTA) action_magenta = kontextmenu.addAction(tr("Mark magenta"));
			if (Aktiver_farbmarker != SCENE_FILTER_YELLOW) action_yellow = kontextmenu.addAction(tr("Mark yellow"));
			
			if (action_red == 0 || action_green == 0 || action_blue == 0 || action_cyan == 0 || action_magenta == 0 || action_yellow == 0)
			{
				kontextmenu.addSeparator();
				
				action_unmark = kontextmenu.addAction(tr("Remove color mark"));
			}
			
			if (action_red != 0) farbmarkierung_actionen.insert(action_red, SCENE_FILTER_RED);
			if (action_green != 0) farbmarkierung_actionen.insert(action_green, SCENE_FILTER_GREEN);
			if (action_blue != 0) farbmarkierung_actionen.insert(action_blue, SCENE_FILTER_BLUE);
			if (action_cyan != 0) farbmarkierung_actionen.insert(action_cyan, SCENE_FILTER_CYAN);
			if (action_magenta != 0) farbmarkierung_actionen.insert(action_magenta, SCENE_FILTER_MAGENTA);
			if (action_yellow != 0) farbmarkierung_actionen.insert(action_yellow, SCENE_FILTER_YELLOW);
			if (action_unmark != 0) farbmarkierung_actionen.insert(action_unmark, SCENE_FILTER_NICHTS);
			
			// das kontextmenu anzeigen und die gewaehlte aktion speichern
			QAction *gewaelte_action = kontextmenu.exec(event->screenPos());
			
			// die gewaehlte aktion verarbeiten
			// wenn ueberhaupt wae gewaehlt wurde
			if (gewaelte_action != 0)
			{
				// wenn es sich um einen tipp handelt
				if (tip_aktionen.contains(gewaelte_action) == true) setze_tip(tip_aktionen.value(gewaelte_action));
				
				// wenn es sich um eine ausschluss anweisung handelt
				else if (ausschluss_setzen_actionen.contains(gewaelte_action) == true) setze_ausschluss_tip(ausschluss_setzen_actionen.value(gewaelte_action), true);
				
				// wenn es sich um die ruecknahme eines ausschlusses handelt
				else if (ausschluss_zuruecksetzen_actionen.contains(gewaelte_action) == true) setze_ausschluss_tip(ausschluss_zuruecksetzen_actionen.value(gewaelte_action), false);
				
				// wenn es sich um eine farbmarkierungs action handelt
				else if (farbmarkierung_actionen.contains(gewaelte_action) == true) setze_farbmarkierung(farbmarkierung_actionen.value(gewaelte_action));
				
				// ansonsten fehklermeldung ausgeben
				else qDebug() << tr("Unknown action in \"void SudokuElement::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)\"");
			}
			
			// das element soll auch ausgewaehlt werden
			setSelected(true);
			Meinescene->verarbeite_auswahl(this);
			
			update();
		}
	}
}


void SudokuElement::keyPressEvent(QKeyEvent* event)
{
	if (isSelected() == true && event->isAutoRepeat() == false && zahl_bekannt() == false && Meinescene->spiel_gewonnen() == false)
	{
		int tip_ = 0;
		
		switch (event->key())
		{
			case Qt::Key_1:
				
				tip_ = 1;
				
				break;
				
			case Qt::Key_2:
				
				tip_ = 2;
				
				break;
				
			case Qt::Key_3:
				
				tip_ = 3;
				
				break;
				
			case Qt::Key_4:
				
				tip_ = 4;
				
				break;
				
			case Qt::Key_5:
				
				tip_ = 5;
				
				break;
				
			case Qt::Key_6:
				
				tip_ = 6;
				
				break;
				
			case Qt::Key_7:
				
				tip_ = 7;
				
				break;
				
			case Qt::Key_8:
				
				tip_ = 8;
				
				break;
				
			case Qt::Key_9:
				
				tip_ = 9;
				
				break;
				
			default:
				
				tip_ = 0;
				
				break;
		}
		
		// gueltige tipps zur weiteren verarbeitung zulassen
		if (tip_ != 0)
		{
			// wenn der tipp zulaessig ist den tipp annehmen
			if (verfuegbarkeit.contains(tip_) && verfuegbarkeit.value(tip_) == true && ausgeschlossen.contains(tip_) == true && ausgeschlossen.value(tip_) == false && ist_zahl() == false && ist_tip() == false)
			{
				setze_tip(tip_);
				
				event->accept();
				
				update();
			}
			
			// ungueltige tipps ignorieren
			else event->ignore();
		}
		
		// ungueltige tipps ignorieren
		else event->ignore();
	}
	
	else if (Meinescene->spiel_gewonnen() == true && isSelected() == true && event->isAutoRepeat() == false && zahl_bekannt() == false) event->ignore();
}


void SudokuElement::setze_tip_zurueck(bool senden)
{
	if (Meinescene->spiel_gewonnen() == false)
	{
		int tmp = Tip;
		Tip = 0;
		
		Meinescene->setze_verfuegbarkeiten_zurueck(this, tmp);
		if (senden == true) emit tipp_zurueckgenommen(this, tmp);
		
		update();
	}
}


void SudokuElement::reset()
{
	setSelected(false);
	
	if (Meinescene->mouseGrabberItem() != 0 && Meinescene->mouseGrabberItem() == this) ungrabMouse();
	
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemIsFocusable, true);
	
	Tip = 0;
	
	Zahl = 0;
	
	// die verfuegbarkeit der tipps resetten
	verfuegbarkeit.insert(1, true);
	verfuegbarkeit.insert(2, true);
	verfuegbarkeit.insert(3, true);
	verfuegbarkeit.insert(4, true);
	verfuegbarkeit.insert(5, true);
	verfuegbarkeit.insert(6, true);
	verfuegbarkeit.insert(7, true);
	verfuegbarkeit.insert(8, true);
	verfuegbarkeit.insert(9, true);
	
	// die ausschlussliste resetten
	ausgeschlossen.insert(1, false);
	ausgeschlossen.insert(2, false);
	ausgeschlossen.insert(3, false);
	ausgeschlossen.insert(4, false);
	ausgeschlossen.insert(5, false);
	ausgeschlossen.insert(6, false);
	ausgeschlossen.insert(7, false);
	ausgeschlossen.insert(8, false);
	ausgeschlossen.insert(9, false);
	
	// die aktive farbmarkierung zuruecksetzen
	Aktiver_farbmarker = SCENE_FILTER_NICHTS;
	
	// den filter zuruecksetzen
	Filterung = SCENE_FILTER_NICHTS;
	
	vormals_ausgewaehlt = false;
	
	update();
}


void SudokuElement::setze_ausschluss_tip_zurueck()
{
	ausgeschlossen.insert(1, false);
	ausgeschlossen.insert(2, false);
	ausgeschlossen.insert(3, false);
	ausgeschlossen.insert(4, false);
	ausgeschlossen.insert(5, false);
	ausgeschlossen.insert(6, false);
	ausgeschlossen.insert(7, false);
	ausgeschlossen.insert(8, false);
	ausgeschlossen.insert(9, false);
}


void SudokuElement::filtere(int wert)
{
	Filterung = wert;
	
	update();
}


void SudokuElement::setze_farbmarkierung(int wert)
{
	if ((wert == 0 || (wert >= 10 && wert <= 15)) && Meinescene->spiel_gewonnen() == false)
	{
		Aktiver_farbmarker = wert;
		
		update();
	}
	
	else qDebug() << tr("Unknown color mark in \"void SudokuElement::setze_farbmarkierung(int wert)\"");
}


int SudokuElement::farbmarkierung() const
{
	return Aktiver_farbmarker;
}


void SudokuElement::focusInEvent(QFocusEvent* event)
{
	QGraphicsItem::focusInEvent(event);
}


void SudokuElement::focusOutEvent(QFocusEvent* event)
{
	QGraphicsItem::focusOutEvent(event);
	
	setSelected(false);
}


void SudokuElement::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	QGraphicsItem::mousePressEvent(event);
	
	if (Sichtbarkeit_kandidaten == true && Meinescene->spiel_gewonnen() == false)
	{
		// nur, wenn mit der linken maustaste geklickt wurde und das element bereits ausgewaehlt ist. die vorauswahl ist noetig, damit man das element auswaehlen kann ohne dabei aus versehen einen tipp abzugeben. das zuruecknehmen der vorauswahl wird von der scene erledigt
		if (event->button() == Qt::LeftButton && vormals_ausgewaehlt == true)
		{
			// testen, welches element angeklick wurde
			if (ring1.adjusted(RINGKLICK_KORREKTUR, RINGKLICK_KORREKTUR, -RINGKLICK_KORREKTUR, -RINGKLICK_KORREKTUR).contains(event->pos()) == true)
			{
				if (verfuegbarkeit.value(1) == true && ausgeschlossen.value(1) == false && ist_tip() == false && ist_zahl() == false) setze_tip(1);
			}
			
			else if (ring2.adjusted(RINGKLICK_KORREKTUR, RINGKLICK_KORREKTUR, -RINGKLICK_KORREKTUR, -RINGKLICK_KORREKTUR).contains(event->pos()) == true)
			{
				if (verfuegbarkeit.value(2) == true && ausgeschlossen.value(2) == false && ist_tip() == false && ist_zahl() == false) setze_tip(2);
			}
			
			else if (ring3.adjusted(RINGKLICK_KORREKTUR, RINGKLICK_KORREKTUR, -RINGKLICK_KORREKTUR, -RINGKLICK_KORREKTUR).contains(event->pos()) == true)
			{
				if (verfuegbarkeit.value(3) == true && ausgeschlossen.value(3) == false && ist_tip() == false && ist_zahl() == false) setze_tip(3);
			}
			
			else if (ring4.adjusted(RINGKLICK_KORREKTUR, RINGKLICK_KORREKTUR, -RINGKLICK_KORREKTUR, -RINGKLICK_KORREKTUR).contains(event->pos()) == true)
			{
				if (verfuegbarkeit.value(4) == true && ausgeschlossen.value(4) == false && ist_tip() == false && ist_zahl() == false) setze_tip(4);
			}
			
			else if (ring5.adjusted(RINGKLICK_KORREKTUR, RINGKLICK_KORREKTUR, -RINGKLICK_KORREKTUR, -RINGKLICK_KORREKTUR).contains(event->pos()) == true)
			{
				if (verfuegbarkeit.value(5) == true && ausgeschlossen.value(5) == false && ist_tip() == false && ist_zahl() == false) setze_tip(5);
			}
			
			else if (ring6.adjusted(RINGKLICK_KORREKTUR, RINGKLICK_KORREKTUR, -RINGKLICK_KORREKTUR, -RINGKLICK_KORREKTUR).contains(event->pos()) == true)
			{
				if (verfuegbarkeit.value(6) == true && ausgeschlossen.value(6) == false && ist_tip() == false && ist_zahl() == false) setze_tip(6);
			}
			
			else if (ring7.adjusted(RINGKLICK_KORREKTUR, RINGKLICK_KORREKTUR, -RINGKLICK_KORREKTUR, -RINGKLICK_KORREKTUR).contains(event->pos()) == true)
			{
				if (verfuegbarkeit.value(7) == true && ausgeschlossen.value(7) == false && ist_tip() == false && ist_zahl() == false) setze_tip(7);
			}
			
			else if (ring8.adjusted(RINGKLICK_KORREKTUR, RINGKLICK_KORREKTUR, -RINGKLICK_KORREKTUR, -RINGKLICK_KORREKTUR).contains(event->pos()) == true)
			{
				if (verfuegbarkeit.value(8) == true && ausgeschlossen.value(8) == false && ist_tip() == false && ist_zahl() == false) setze_tip(8);
			}
			
			else if (ring9.adjusted(RINGKLICK_KORREKTUR, RINGKLICK_KORREKTUR, -RINGKLICK_KORREKTUR, -RINGKLICK_KORREKTUR).contains(event->pos()) == true)
			{
				if (verfuegbarkeit.value(9) == true && ausgeschlossen.value(9) == false && ist_tip() == false && ist_zahl() == false) setze_tip(9);
			}
		}
		
		else vormals_ausgewaehlt = true;
		
		// die auswahl fuer die vorauswahl in der scene verarbeiten
		Meinescene->verarbeite_auswahl(this);
		
		// in einem signal ueber die auswahl informieren
		emit ausgewaehlt(this);
	}
}


int SudokuElement::ist_vorausgewaehlt() const
{
	return vormals_ausgewaehlt;
}


void SudokuElement::setze_vorauswahl(bool wert)
{
	vormals_ausgewaehlt = wert;
}


void SudokuElement::loese_wenn_einzeln()
{
	if (ist_tip() == false && ist_zahl() == false && Meinescene->spiel_gewonnen() == false)
	{
		QList<int> moeglichkeiten(verfuegbarkeit.keys(true));
		QList<int> ausschluesse(ausgeschlossen.keys(true));
		
        for (int idx = 0; idx < ausschluesse.size(); idx++) moeglichkeiten.removeAll(ausschluesse.at(idx));
		
		if (moeglichkeiten.size() == 1) setze_tip(moeglichkeiten.first());
	}
}


bool SudokuElement::sichtbarkeit_kandidaten() const
{
	return Sichtbarkeit_kandidaten;
}


void SudokuElement::setze_sichtbarkeit_kandidaten(bool zustand)
{
	Sichtbarkeit_kandidaten = zustand;
	
	update();
}


void SudokuElement::mouseDoubleClickEvent(QGraphicsSceneMouseEvent*)
{
	// wenn nur noch eine moeglichkeit, dann loesen
	loese_wenn_einzeln();
}
