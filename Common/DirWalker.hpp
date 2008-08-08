/**
 * DirWalker.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef DIRWALKER_HPP
#define DIRWALKER_HPP DIRWALKER_HPP

#include <QFileInfo>

class QString;


class DirWalkerCallbacks
{
public:
   virtual ~DirWalkerCallbacks(){}
   virtual void handleFile( const QFileInfo &fileInfo ) = 0;
   virtual void handleDir( const QFileInfo &fileInfo )  = 0;
};


class DirWalker
{
public:
   DirWalker();
   virtual ~DirWalker();
   void run( DirWalkerCallbacks *callbacks, 
             const QString &directoryPath, bool recursive );
   
private:
   DirWalker( const DirWalker &other );
   DirWalker &operator=( const DirWalker &other );
};

#endif
