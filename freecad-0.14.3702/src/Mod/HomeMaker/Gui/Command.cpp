/***************************************************************************
 *   Copyright (c) YEAR YOUR NAME         <Your e-mail address>            *
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License as published by the Free Software Foundation; either          *
 *   version 2 of the License, or (at your option) any later version.      *
 *                                                                         *
 *   This library  is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this library; see the file COPYING.LIB. If not,    *
 *   write to the Free Software Foundation, Inc., 59 Temple Place,         *
 *   Suite 330, Boston, MA  02111-1307, USA                                *
 *                                                                         *
 ***************************************************************************/


#include "PreCompiled.h"
#ifndef _PreComp_
#include <QAction>
#include <QFileDialog>
#include <QImage>
#include <QImageReader>
#include <QMessageBox>
#include <QTextStream>
#endif

#include <time.h>
#include <sys/timeb.h>

#include <string.h>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

// A enlever lorsque toutes les traitements auront été ajouté pour les commandes

#include <Base/Console.h>

#include <Base/Exception.h>
#include <Base/Interpreter.h>
#include <Base/Tools.h>
#include <Base/Vector3D.h>
#include <App/Document.h>
#include <Gui/Application.h>
#include <Gui/MainWindow.h>
#include <Gui/Command.h>
#include <Gui/BitmapFactory.h>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"

using namespace cv;
using namespace std;

//#include "../../Image/Gui/ImageOrientationDialog.h"
//#include <Mod/Image/Gui/ImageView.h>
void makeBox(std::string name, std::vector<Base::Vector3d> list, float height);
void makeExtrude(std::string name, std::vector<Base::Vector3d> list, float height);
void makeBooleanOperation(std::string extrudeName1, std::string extrudeName2, std::string operation);
std::vector<Base::Vector3d> prepareWallFromLine(Base::Vector3d start, Base::Vector3d end, float width, std::string align);

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//===========================================================================
// CmdHomeMakerOpenImage
//===========================================================================
DEF_STD_CMD(CmdHomeMakerOpenImage);

CmdHomeMakerOpenImage::CmdHomeMakerOpenImage()
	:Command("HomeMaker_Open")
{
    sAppModule    = "HomeMaker";
    sGroup        = QT_TR_NOOP("HomeMaker");
    sMenuText     = QT_TR_NOOP("Open an image");
    sToolTipText  = QT_TR_NOOP("Open an image from files");
    sWhatsThis    = sToolTipText;
    sStatusTip    = sToolTipText;
    sPixmap       = "HomeMaker_Open";
    sAccel        = "CTRL+O";
}

void CmdHomeMakerOpenImage::activated(int iMsg)
{
	QString formats;
	QTextStream str(&formats);
	str << QObject::tr("Images") << " (";
	QList<QByteArray> qtformats = QImageReader::supportedImageFormats();
	for (QList<QByteArray>::Iterator it = qtformats.begin(); it != qtformats.end(); ++it)
	{
		str << "*." << it->toLower() << " ";
	}
	str << ");;" << QObject::tr("All files") << "(*.*)";
	
	QString s = QFileDialog::getOpenFileName(Gui::getMainWindow(), QObject::tr("Choose an image file to open"), QString::null, formats);
	
	if (!s.isEmpty())
	{
		try 
		{
			//TODO: Réussir à mettre l'image sur un plan -> s'inpirer de ce qui est fait pour le module Image
			/*
			QImage impQ(s);
			if (impQ.isNull())
			{
				QMessageBox::warning(Gui::getMainWindow(), QObject::tr("Error open image"),
					QObject::tr("Could not load the chosen image"));
				return;
			}
			
			// ImageOrientationDialog Dlg;
			
			// if (Dlg.exec() != QDialog::Accepted)
			// 	return;
			Base::Vector3d p = Base::Vector3d(0, 0, 0);
			Base::Rotation r = Base::Rotation(Base::Vector3d(1, 0, 0), M_PI / 2.0);

			std::string FeatName = getUniqueObjectName("ImagePlane");

			Command::openCommand("Create ImagePlane");
			Command::doCommand(Doc,"App.activeDocument().addObject('Image::ImagePlane','%s\')",FeatName.c_str());
			Command::doCommand(Doc,"App.activeDocument().%s.ImageFile = '%s'",FeatName.c_str(),(const char*)s.toUtf8());
			Command::doCommand(Doc,"App.activeDocument().%s.XSize = %d",FeatName.c_str(),impQ.width () );
			Command::doCommand(Doc,"App.activeDocument().%s.YSize = %d",FeatName.c_str(),impQ.height() );
			Command::doCommand(Doc,"App.activeDocument().%s.Placement = App.Placement(App.Vector(%f,%f,%f),App.Rotation(%f,%f,%f,%f))"
						 ,FeatName.c_str(),p.x,p.y,p.z,r[0],r[1],r[2],r[3]);
			Command::commitCommand();				
			*/
			Command::doCommand(Command::Gui, "import Image, ImageGui");
			Command::doCommand(Command::Gui, "ImageGui.open(\"%s\")", (const char*)s.toUtf8());
			Base::Console().Message((const char*)s.toUtf8());
		}
		catch (const Base::PyException& e)
		{
			e.ReportException();
		}
	}	
}
/*
bool CmdHomeMakerOpenImage::isActive()
{
	return App::GetApplication().getActiveDocument();
}*/

//===========================================================================
// CmdHomeMakerEditImage
//===========================================================================
DEF_STD_CMD(CmdHomeMakerEditImage);

CmdHomeMakerEditImage::CmdHomeMakerEditImage()
	:Command("HomeMaker_Edit")
{
    sAppModule    = "HomeMaker";
    sGroup        = QT_TR_NOOP("HomeMaker");
    sMenuText     = QT_TR_NOOP("Edit an image");
    sToolTipText  = QT_TR_NOOP("Edit an image from blank page");
    sWhatsThis    = sToolTipText;
    sStatusTip    = sToolTipText;
    sPixmap       = "HomeMaker_Edit";
    sAccel        = "CTRL+E";
}

void CmdHomeMakerEditImage::activated(int iMsg)
{
	// Ajoutez le traitement pour éditer une image
	Base::Console().Message("Edit an image!\n");
}


//===========================================================================
// CmdHomeMakerExtrudeImage
//===========================================================================
DEF_STD_CMD(CmdHomeMakerExtrudeImage);

CmdHomeMakerExtrudeImage::CmdHomeMakerExtrudeImage()
	:Command("HomeMaker_Extrude")
{
    sAppModule    = "HomeMaker";
    sGroup        = QT_TR_NOOP("HomeMaker");
    sMenuText     = QT_TR_NOOP("Extrude an opened image");
    sToolTipText  = QT_TR_NOOP("Extrude an opened image");
    sWhatsThis    = sToolTipText;
    sStatusTip    = sToolTipText;
    sPixmap       = "HomeMaker_Extrude";
    sAccel        = "CTRL+D";
}

void CmdHomeMakerExtrudeImage::activated(int iMsg)
{
	// Zone de tests de fonctions
	makeExtrude("ext1", prepareWallFromLine(Base::Vector3d(-2.5, -2.5, 0.0), Base::Vector3d(2.1, -2.5, 0.0), 2.5f, "Right"), 5.0f);
	makeExtrude("ext2", prepareWallFromLine(Base::Vector3d(-1.0, -1.0, 0.0), Base::Vector3d(1.0, -1.0, 0.0), 0.4f, "Right"), 5.0f);
	makeBooleanOperation("ext1Extrude", "ext2Extrude", "Cut");
	
	// Traitement opencv + extrude/cut
	Mat src_gray;
	Mat dst;
	std::vector< vector<Point> > contours;
	
	Mat src = imread("C:/Users/Alexandre/Desktop/plan_bis.png"); // TODO: Comprendre le problème de chargement
	if(!src.data)
	{
		Base::Console().Message("Image file not found!\n");
		return;
	}

	// Traitement opencv pour trouver les contours
	cvtColor(src, src_gray, CV_BGR2GRAY);
	threshold(src_gray, dst, 127, 255, 0);
	findContours(dst, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));	

	// Conversion des contours en liste de Vector3D
	std::vector< vector<Base::Vector3d> > elements;
	for(unsigned int i = 1; i < contours.size(); ++i)
	{
		std::vector<Point> currentContour = contours[i];
		std::vector<Base::Vector3d> tmp;
		for (unsigned int j = 0; j < currentContour.size(); ++j)
		{
			Point p = currentContour[j];
			tmp.push_back(Base::Vector3d(p.x, p.y, 0));
		}
		elements.push_back(tmp);
	}

	// Extrude du contour principal
	if (elements.size() > 0)
		makeExtrude("base", elements[0], 5.0f);

	// Extrude des autres contours et cut operation avec le principal
	std::string name = "ext";
	for (unsigned int i = 1; i < elements.size(); ++i)
	{
		std::stringstream sstm;
		sstm << name << i;
		makeExtrude(sstm.str(), elements[i], 5.0f);
		makeBooleanOperation("baseExtrude", sstm.str() + "Extrude", "Cut");
	}

	/* Zone de tests de fonctions
	makeExtrude("ext1", prepareWallFromLine(Base::Vector3d(-2.5, -2.5, 0.0), Base::Vector3d(2.1, -2.5, 0.0), 2.5f, "Right"), 5.0f);
	makeExtrude("ext2", prepareWallFromLine(Base::Vector3d(-1.0, -1.0, 0.0), Base::Vector3d(1.0, -1.0, 0.0), 0.4f, "Right"), 5.0f);
	makeBooleanOperation("ext1Extrude", "ext2Extrude", "Cut");
	*/
}

void CreateHomeMakerCommands(void)
{
    Gui::CommandManager &rcCmdMgr = Gui::Application::Instance->commandManager();
    
	rcCmdMgr.addCommand(new CmdHomeMakerOpenImage());
	rcCmdMgr.addCommand(new CmdHomeMakerEditImage());
	rcCmdMgr.addCommand(new CmdHomeMakerExtrudeImage());
}

void makeBooleanOperation(std::string extrudeName1, std::string extrudeName2, std::string operation)
{
	Gui::Command::openCommand("Make Boolean Operation");
	Gui::Command::doCommand(Gui::Command::Doc, "App.activeDocument().addObject('Part::Cut', '%s')", operation.c_str());
	Gui::Command::doCommand(Gui::Command::Doc, "App.activeDocument().%s.Base = App.activeDocument().%s", operation.c_str(), extrudeName1.c_str());
	Gui::Command::doCommand(Gui::Command::Doc, "App.activeDocument().%s.Tool = App.activeDocument().%s", operation.c_str(), extrudeName2.c_str());
	Gui::Command::doCommand(Gui::Command::Doc, "Gui.activeDocument().hide('%s')", extrudeName1.c_str());
	Gui::Command::doCommand(Gui::Command::Doc, "Gui.activeDocument().hide('%s')", extrudeName2.c_str());
	Gui::Command::doCommand(Gui::Command::Doc, "Gui.activeDocument().%s.ShapeColor = Gui.activeDocument().%s.ShapeColor", operation.c_str(), extrudeName1.c_str());
	Gui::Command::doCommand(Gui::Command::Doc, "Gui.activeDocument().%s.DisplayMode = Gui.activeDocument().%s.DisplayMode", operation.c_str(), extrudeName1.c_str());

	Gui::Command::updateActive();
}

void makeExtrude(std::string name, std::vector<Base::Vector3d> list, float height)
{
	std::string sketchName = name + "Sketch";
	std::string extrudeName = name + "Extrude";
	
	Gui::Command::doCommand(Gui::Command::Doc, "App.activeDocument().addObject('Sketcher::SketchObject', '%s')", sketchName.c_str());	
	for(unsigned int i = 0; i < list.size() - 1; i++)
		Gui::Command::doCommand(Gui::Command::Doc, "App.activeDocument().%s.addGeometry(Part.Line(App.Vector(%f, %f, 0), App.Vector(%f, %f, 0)))", sketchName.c_str(), list[i].x, list[i].y, list[i + 1].x, list[i + 1].y);
	Gui::Command::doCommand(Gui::Command::Doc, "App.activeDocument().%s.addGeometry(Part.Line(App.Vector(%f, %f, 0), App.Vector(%f, %f, 0)))", sketchName.c_str(), list[0].x, list[0].y, list[list.size() - 1].x, list[list.size() - 1].y);	
	
	Gui::Command::openCommand("Make Extrude");
	Gui::Command::doCommand(Gui::Command::Doc, "App.activeDocument().addObject('Part::Extrusion', '%s')", extrudeName.c_str());
	Gui::Command::doCommand(Gui::Command::Doc, "App.activeDocument().%s.Base = App.activeDocument().%s", extrudeName.c_str(), sketchName.c_str());
	Gui::Command::doCommand(Gui::Command::Doc, "App.activeDocument().%s.Dir = (0,0,%f)", extrudeName.c_str(), height);
	Gui::Command::doCommand(Gui::Command::Doc, "App.activeDocument().%s.Solid =(True)", extrudeName.c_str());
	Gui::Command::doCommand(Gui::Command::Doc, "App.activeDocument().%s.TaperAngle = (0)", extrudeName.c_str());
	// Gui::Command::doCommand(Gui::Command::Doc, "App.activeDocument().%s.Sketch = App.activeDocument().%s", extrudeName.c_str(), sketchName.c_str());
	// Gui::Command::doCommand(Gui::Command::Doc, "App.activeDocument().%s.Visibility = False", sketchName.c_str());
	Gui::Command::doCommand(Gui::Command::Doc, "App.activeDocument().%s.Label = '%s'", extrudeName.c_str(), extrudeName.c_str());
	
	Gui::Command::updateActive();
}

void makeBox(std::string name, std::vector<Base::Vector3d> list, float height)
{
	std::string sketchName = name + "Sketch";
	std::string padName = name + "Pad";
	
	Gui::Command::doCommand(Gui::Command::Doc, "App.activeDocument().addObject('Sketcher::SketchObject', '%s')", sketchName.c_str());	
	for(unsigned int i = 0; i < list.size() - 1; i++)
		Gui::Command::doCommand(Gui::Command::Doc, "App.activeDocument().%s.addGeometry(Part.Line(App.Vector(%f, %f, 0), App.Vector(%f, %f, 0)))", sketchName.c_str(), list[i].x, list[i].y, list[i + 1].x, list[i + 1].y);
	Gui::Command::doCommand(Gui::Command::Doc, "App.activeDocument().%s.addGeometry(Part.Line(App.Vector(%f, %f, 0), App.Vector(%f, %f, 0)))", sketchName.c_str(), list[0].x, list[0].y, list[list.size() - 1].x, list[list.size() - 1].y);	
	
	Gui::Command::openCommand("Make Pad");
	Gui::Command::doCommand(Gui::Command::Doc, "App.activeDocument().addObject('PartDesign::Pad', '%s')", padName.c_str());	
	Gui::Command::doCommand(Gui::Command::Doc, "App.activeDocument().%s.Sketch = App.activeDocument().%s", padName.c_str(), sketchName.c_str());
	Gui::Command::doCommand(Gui::Command::Doc, "App.activeDocument().%s.Length = %f", padName.c_str(), height);
	
	Gui::Command::updateActive();
}

std::vector<Base::Vector3d> prepareWallFromLine(Base::Vector3d start, Base::Vector3d end, float width, std::string align)
{
	std::vector<Base::Vector3d> list;

	// Calcul du coefficient directeur
	float m = 0.0f;
	if (end.x != start.x)
	{
		if (end.y > start.y)
			m = (end.y - start.y) / (end.x - start.x);
		else if (end.y < start.y)
			m = (start.y - end.y) / (start.x - end.x);
	}

	// Calcul de la droite normale
	float offset = width / 2.0;
	float coeff = offset / (sqrt((-m) * (-m) + 1));
	float xVectNorm = coeff * (-m);
	float yVectNorm = coeff;

	
	if (start.x < end.x)								// Si le x du start est plus petit que le x du end, on crée dans un sens le rectangle et on se réfère à la normale
	{
		list.push_back(Base::Vector3d(start.x - xVectNorm, start.y - yVectNorm, start.z));
		list.push_back(Base::Vector3d(start.x + xVectNorm, start.y + yVectNorm, start.z));
		list.push_back(Base::Vector3d(end.x + xVectNorm, end.y + yVectNorm, end.z));
		list.push_back(Base::Vector3d(end.x - xVectNorm, end.y - yVectNorm, end.z));
	}
	else if (start.x > end.x)							// Sinon on parcourt dans l'autre sens
	{
		list.push_back(Base::Vector3d(end.x - xVectNorm, end.y - yVectNorm, end.z));
		list.push_back(Base::Vector3d(end.x + xVectNorm, end.y + yVectNorm, end.z));
		list.push_back(Base::Vector3d(start.x + xVectNorm, start.y + yVectNorm, start.z));		
		list.push_back(Base::Vector3d(start.x - xVectNorm, start.y - yVectNorm, start.z));
	}
	else												// Sinon on se réfère à l'offset
	{
		if (start.y < end.y)							// Si le y du start est plus petit que le y du end, on crée dans un sens le rectangle
		{
			list.push_back(Base::Vector3d(end.x - offset, end.y, end.z));
			list.push_back(Base::Vector3d(end.x + offset, end.y, end.z));
			list.push_back(Base::Vector3d(start.x + offset, start.y, start.z));	
			list.push_back(Base::Vector3d(start.x - offset, start.y, start.z));
		}
		else											// Sinon on parcourt dans l'autre sens
		{
			list.push_back(Base::Vector3d(start.x - offset, start.y, start.z));
			list.push_back(Base::Vector3d(start.x + offset, start.y, start.z));	
			list.push_back(Base::Vector3d(end.x + offset, end.y, end.z));	
			list.push_back(Base::Vector3d(end.x - offset, end.y, end.z));
		}
	}

	// Gestion des align (NB: Par défaut, c'est Center)
	if (align.compare("Left") == 0)
	{
		for(unsigned int i = 0; i < list.size(); i++)
		{
			if (start.x != end.x)
			{
				if (start.y < end.y)
				{
					if (start.x < end.x)
					{
						list[i].x += xVectNorm;
						list[i].y += yVectNorm;
					}
					else
					{
						list[i].x -= xVectNorm;
						list[i].y -= yVectNorm;
					}					
				}
				else
				{
					if (start.x < end.x)
					{
						list[i].x -= xVectNorm;
						list[i].y -= yVectNorm;
					}
					else
					{
						list[i].x += xVectNorm;
						list[i].y += yVectNorm;
					}
				}
			}
			else
			{
				if (start.y < end.y)
				{
					list[i].x += offset;
				}
				else
				{
					list[i].x -= offset;
				}
			}
		}
	}
	else if (align.compare("Right") == 0)
	{
		for(unsigned int i = 0; i < list.size(); i++)
		{
			if (start.x != end.x)
			{
				if (start.y < end.y)
				{
					if (start.x < end.x)
					{
						list[i].x -= xVectNorm;
						list[i].y -= yVectNorm;
					}
					else
					{
						list[i].x += xVectNorm;
						list[i].y += yVectNorm;
					}					
				}
				else
				{
					if (start.x < end.x)
					{
						list[i].x += xVectNorm;
						list[i].y += yVectNorm;
					}
					else
					{
						list[i].x -= xVectNorm;
						list[i].y -= yVectNorm;
					}
				}
			}
			else
			{
				if (start.y < end.y)
				{
					list[i].x -= offset;
				}
				else
				{
					list[i].x += offset;
				}				
			}
		}
	}
	else if (align.compare("Center") != 0)
	{
		// Cas d'erreur
	}
	
	return list;
}