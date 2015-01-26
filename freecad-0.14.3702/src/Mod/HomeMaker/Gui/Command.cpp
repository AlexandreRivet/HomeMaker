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

//#include "../../Image/Gui/ImageOrientationDialog.h"
//#include <Mod/Image/Gui/ImageView.h>
void makeBox(std::string name, std::vector<Base::Vector3d> list, float height);
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
	// Ajouter le traitement opencv ici et retourner un ground + une liste wall
	
	// Ici un exemple de la manière dont on va créer la structure
	std::vector<Base::Vector3d> list;
	list.push_back(Base::Vector3d(-2.5, -2.5, 0.0));
	list.push_back(Base::Vector3d(2.5, -2.5, 0.0));
	list.push_back(Base::Vector3d(2.5, 2.5, 0.0));
	list.push_back(Base::Vector3d(-2.5, 2.5, 0.0));
	
	makeExtrude("wall1", prepareWallFromLine(Base::Vector3d(-2.5, -2.5, 0.0), Base::Vector3d(2.1, -2.5, 0.0), 0.4f, "Right"), 5.0f);
	/*makeBox("ground", list, 0.5);
	makeBox("wall1", prepareWallFromLine(Base::Vector3d(-2.5, -2.5, 0.0), Base::Vector3d(2.1, -2.5, 0.0), 0.4f, "Right"), 5.0f);
	makeBox("wall2", prepareWallFromLine(Base::Vector3d(2.5, -2.5, 0.0), Base::Vector3d(2.5, -0.3, 0.0), 0.4f, "Right"), 5.0f);
	makeBox("wall3", prepareWallFromLine(Base::Vector3d(2.5, 0.5, 0.0), Base::Vector3d(2.5, 2.1, 0.0), 0.4f, "Right"), 5.0f);
	makeBox("wall4", prepareWallFromLine(Base::Vector3d(2.5, 2.5, 0), Base::Vector3d(-2.1, 2.5, 0), 0.4f, "Right"), 5.0f);
	makeBox("wall5", prepareWallFromLine(Base::Vector3d(-2.5, 2.5, 0), Base::Vector3d(-2.5, -2.1, 0), 0.4f, "Right"), 5.0f);
	makeBox("wall6", prepareWallFromLine(Base::Vector3d(2.1, -0.3, 0), Base::Vector3d(-1.5, -0.3, 0), 0.4f, "Right"), 5.0f);
	makeBox("wall7", prepareWallFromLine(Base::Vector3d(2.1, 0.9, 0), Base::Vector3d(0.3, 0.9, 0), 0.4f, "Right"), 5.0f);
	makeBox("wall8", prepareWallFromLine(Base::Vector3d(-0.5, 0.9, 0), Base::Vector3d(-2.1, 0.9, 0), 0.4f, "Right"), 5.0f);*/
}



void CreateHomeMakerCommands(void)
{
    Gui::CommandManager &rcCmdMgr = Gui::Application::Instance->commandManager();
    
	rcCmdMgr.addCommand(new CmdHomeMakerOpenImage());
	rcCmdMgr.addCommand(new CmdHomeMakerEditImage());
	rcCmdMgr.addCommand(new CmdHomeMakerExtrudeImage());
}
void makeExtrude(std::string name, std::vector<Base::Vector3d> list, float height)
{
	std::string sketchName = name + "Sketch";
	std::string extrudeName = name + "Pad";
	
	Gui::Command::doCommand(Gui::Command::Doc, "App.activeDocument().addObject('Sketcher::SketchObject', '%s')", sketchName.c_str());	
	for(unsigned int i = 0; i < list.size() - 1; i++)
		Gui::Command::doCommand(Gui::Command::Doc, "App.activeDocument().%s.addGeometry(Part.Line(App.Vector(%f, %f, 0), App.Vector(%f, %f, 0)))", sketchName.c_str(), list[i].x, list[i].y, list[i + 1].x, list[i + 1].y);
	Gui::Command::doCommand(Gui::Command::Doc, "App.activeDocument().%s.addGeometry(Part.Line(App.Vector(%f, %f, 0), App.Vector(%f, %f, 0)))", sketchName.c_str(), list[0].x, list[0].y, list[list.size() - 1].x, list[list.size() - 1].y);	
	
	Gui::Command::openCommand("Make Extrude");
	Gui::Command::doCommand(Gui::Command::Doc, "App.activeDocument().addObject('PartDesign::Extrusion', '%s')", extrudeName.c_str());
	Gui::Command::doCommand(Gui::Command::Doc, "App.activeDocument().%s.Base = App.activeDocument().%s", extrudeName.c_str(), sketchName.c_str());
	Gui::Command::doCommand(Gui::Command::Doc, "App.activeDocument().%s.Dir = (0,0,%f)", extrudeName.c_str(), height);
	Gui::Command::doCommand(Gui::Command::Doc, "App.activeDocument().%s.Solid =(True)", extrudeName.c_str());
	Gui::Command::doCommand(Gui::Command::Doc, "App.activeDocument().%s.TaperAngle = (0)", extrudeName.c_str());
	Gui::Command::doCommand(Gui::Command::Doc, "App.activeDocument().%s.Sketch = App.activeDocument().%s", extrudeName.c_str(), sketchName.c_str());
	Gui::Command::doCommand(Gui::Command::Doc, "App.activeDocument().%s.Visibility = False", sketchName.c_str());
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