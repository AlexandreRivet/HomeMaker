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
#include <App/Document.h>
#include <Gui/Application.h>
#include <Gui/MainWindow.h>
#include <Gui/Command.h>
#include <Gui/BitmapFactory.h>

//#include "../../Image/Gui/ImageOrientationDialog.h"
//#include <Mod/Image/Gui/ImageView.h>


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
	// Ajoutez le traitement pour extruder une image
	Base::Console().Message("Extrude an image!\n");
}



void CreateHomeMakerCommands(void)
{
    Gui::CommandManager &rcCmdMgr = Gui::Application::Instance->commandManager();
    
	rcCmdMgr.addCommand(new CmdHomeMakerOpenImage());
	rcCmdMgr.addCommand(new CmdHomeMakerEditImage());
	rcCmdMgr.addCommand(new CmdHomeMakerExtrudeImage());
}
