/***************************************************************************
                               sparamfile.cpp
                              ----------------
    begin                : Sat Aug 23 2003
    copyright            : (C) 2003 by Michael Margraf
    email                : michael.margraf@alumni.tu-berlin.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "sparamfile.h"
#include "main.h"

#include <qfileinfo.h>

extern QDir QucsWorkDir;


SParamFile::SParamFile(int No)
{
  Description = QObject::tr("S parameter file");

  int h = 30*((No-1)/2) + 15;
  Lines.append(new Line(-15, -h, 15, -h,QPen(QPen::darkBlue,2)));
  Lines.append(new Line( 15, -h, 15,  h,QPen(QPen::darkBlue,2)));
  Lines.append(new Line(-15,  h, 15,  h,QPen(QPen::darkBlue,2)));
  Lines.append(new Line(-15, -h,-15,  h,QPen(QPen::darkBlue,2)));
  Texts.append(new Text( -9, -6,QObject::tr("file")));


  int i=0, y = 15-h;
  while(i<No) {
    i++;
    Lines.append(new Line(-30,  y,-15,  y,QPen(QPen::darkBlue,2)));
    Ports.append(new Port(-30,  y));
    Texts.append(new Text(-25,y-14,QString::number(i)));

    if(i == No) break;
    i++;
    Lines.append(new Line( 15,  y, 30,  y,QPen(QPen::darkBlue,2)));
    Ports.append(new Port( 30,  y));
    Texts.append(new Text( 19,y-14,QString::number(i)));
    y += 60;
  }

  Lines.append(new Line( 0, h, 0,h+15,QPen(QPen::darkBlue,2)));
  Texts.append(new Text( 4, h,"Ref"));
  Ports.append(new Port( 0,h+15));    // 'Ref' port

  x1 = -30; y1 = -h-2;
  x2 =  30; y2 =  h+15;

  QFontMetrics  metrics(QucsSettings.font);   // get size of text
  tx = x1+4;
  ty = y1 - 2*metrics.lineSpacing() - 4;
  Model = "#SPfile";
  Name  = "X";

  Props.append(new Property("File", "test.s2p", true,
		QObject::tr("name of the s parameter file")));
  Props.append(new Property("Data", "rectangular", false,
		QObject::tr("data type")+" [rectangular, polar]"));
  Props.append(new Property("Interpolator", "linear", false,
		QObject::tr("interpolation type")+" [linear, cubic]"));
}

SParamFile::~SParamFile()
{
}

Component* SParamFile::newOne()
{
  return new SParamFile(Ports.count()-1);
}

Element* SParamFile::info(QString& Name, char* &BitmapFile, bool getNewOne)
{
  Name = QObject::tr("1-port S parameter file");
  BitmapFile = "spfile1";

  if(getNewOne)  return new SParamFile(1);
  return 0;
}

// -------------------------------------------------------
QString SParamFile::NetList()
{
  if(!isActive) return QString("");  // should it be simulated ?

  QString s = Model.mid(1)+":"+Name;

  // output all node names
  for(Port *p1 = Ports.first(); p1 != 0; p1 = Ports.next())
    s += " "+p1->Connection->Name;   // node names

  // output all properties
  Property *p2 = Props.first();
  QFileInfo info(p2->Value);
  if(info.isRelative())  info.setFile(QucsWorkDir, p2->Value);
  s += " "+p2->Name+"=\"{"+info.absFilePath()+"}\"";

  // data type
  p2 = Props.next();
  s += " "+p2->Name+"=\""+p2->Value+"\"";

  // interpolator type
  p2 = Props.next();
  s += " "+p2->Name+"=\""+p2->Value+"\"";

  return s;
}
