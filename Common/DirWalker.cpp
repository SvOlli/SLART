/**
 * DirWalker.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "DirWalker.hpp"

#include "Trace.hpp"

#include <stdlib.h>

#include <QDir>
#include <QFileInfoList>

DirWalker::DirWalker()
{
}


DirWalker::~DirWalker()
{
}


void DirWalker::run( const QString &directoryPath, bool recursive )
{
   QDir dir( directoryPath );
   QFileInfoList entries( dir.entryInfoList( QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot ) );
   int i;
   
   for( i = 0; i < entries.count(); i++ )
   {
      if( entries.at(i).isDir() )
      {
         emit foundDir( entries.at(i) );
         if( recursive )
         {
            run( entries.at(i).absoluteFilePath(), recursive );
         }
      }
      if( entries.at(i).isFile() )
      {
         emit foundFile( entries.at(i) );
      }
   }
}
