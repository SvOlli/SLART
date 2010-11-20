/**
 * src/apps/Notorious/FreeDBImport.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef FREEDBIMPORT_HPP
#define FREEDBIMPORT_HPP FREEDBIMPORT_HPP

/* base class */

/* system headers */

/* Qt headers */
#include <QThread>
#include <QList>

/* local library headers */

/* local headers */
#include "TarEntry.hpp"

/* forward declaration of Qt classes */

/* forward declaration of local classes */



class QString;

class FreeDBImport : public QThread
{
Q_OBJECT

public:
   FreeDBImport( QObject *parent = 0 );
   virtual ~FreeDBImport();

   void run();
   void stop();
   void setFileName( const QString &fileName );

signals:
   void processed( unsigned count, const char *filename );

private:
   bool     mBreak;
   TarEntry mTarEntry;
   QString  mFileName;
};


#endif
