/**
 * DirWalker.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef DIRWALKER_HPP
#define DIRWALKER_HPP DIRWALKER_HPP

class QFileInfo;
class QString;


/* interface to provide callbacks */
class DirWalkerCallbacks
{
public:
   virtual ~DirWalkerCallbacks(){}
   virtual void handleFile( const QFileInfo &fileInfo )  = 0;
   virtual void handleDir( const QFileInfo &fileInfo )   = 0;
   virtual void handleOther( const QFileInfo &fileInfo ) = 0;
};


class DirWalker
{
public:
   enum RecurseMode
   {
      NoRecurse,
      RecurseBeforeCallback,
      RecurseAfterCallback
   };
   
   DirWalker();
   virtual ~DirWalker();
   
   /* let the directory walker travel the filesystem */
   void run( DirWalkerCallbacks *callbacks, 
             const QString &directoryPath, enum RecurseMode recurse );
   
private:
   DirWalker( const DirWalker &other );
   DirWalker &operator=( const DirWalker &other );
};

#endif
