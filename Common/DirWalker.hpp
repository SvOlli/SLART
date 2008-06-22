/**
 * DirWalker.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef DIRWALKER_HPP
#define DIRWALKER_HPP DIRWALKER_HPP

#include <QObject>

class QString;
class QDir;
class QFileInfo;
   
class DirWalker : public QObject
{
Q_OBJECT

public:
   DirWalker();
   virtual ~DirWalker();
   void run( const QString &directoryPath, bool recursive );
   
public slots:
   
signals:
   void foundFile( const QFileInfo &fileInfo );
   void foundDir( const QFileInfo &fileInfo );
   
private:
   DirWalker( const DirWalker &other );
   DirWalker &operator=( const DirWalker &other );
};

#endif
