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
                     const QString &directoryPath, RecurseMode recurse )
{
   QDir dir( directoryPath );
   QFileInfoList entries( dir.entryInfoList( QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot ) );
   int i;
   
   for( i = 0; i < entries.count(); i++ )
   {
      if( entries.at(i).isDir() )
      {
         switch( recurse )
         {
            case NoRecurse:
               callbacks->handleDir( entries.at(i) );
               break;
            case RecurseBeforeCallback:
               run( callbacks, entries.at(i).absoluteFilePath(), recurse );
               callbacks->handleDir( entries.at(i) );
               break;
            case RecurseAfterCallback:
               callbacks->handleDir( entries.at(i) );
               run( callbacks, entries.at(i).absoluteFilePath(), recurse );
               break;
            default:
               break;
         }
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
