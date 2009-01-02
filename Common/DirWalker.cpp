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


void DirWalker::run( DirWalkerCallbacks *callbacks,
                     const QString &directoryPath )
{
   QDir dir( directoryPath );
   QFileInfoList entries( dir.entryInfoList( QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot ) );
   int i;
   
   for( i = 0; i < entries.count(); i++ )
   {
      if( entries.at(i).isDir() )
      {
         callbacks->handleDirEntry( entries.at(i) );
         run( callbacks, entries.at(i).absoluteFilePath() );
         callbacks->handleDirLeave( entries.at(i) );
      }
      else if( entries.at(i).isFile() )
      {
         callbacks->handleFile( entries.at(i) );
      }
      else
      {
         callbacks->handleOther( entries.at(i) );
      }
   }
}
