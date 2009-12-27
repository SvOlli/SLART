/**
 * FreeDBImport.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef FREEDBIMPORT_HPP
#define FREEDBIMPORT_HPP FREEDBIMPORT_HPP

#include <QThread>
#include <QList>
#include "TarEntry.hpp"


class QString;

class FreeDBImport : public QThread
{
Q_OBJECT
   
public:
   FreeDBImport( QObject *parent = 0 );
   ~FreeDBImport();

   void run();
   void stop() { mBreak = true; };
   void setFileName( const QString &fileName ) { mFileName = fileName; };

signals:
   void processed( unsigned count, const char *filename );

private:
   bool     mBreak;
   TarEntry mTarEntry;
   QString  mFileName;
};


#endif
