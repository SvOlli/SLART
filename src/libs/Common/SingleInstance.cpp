/*
 * src/libs/Common/SingleInstance.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "SingleInstance.hpp"

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* class variable instantiation */
SingleInstance *SingleInstance::cpSingleInstance = 0;


SingleInstance::SingleInstance( QObject *parent )
: QObject( parent )
, mObjectMap()
{
}


SingleInstance::~SingleInstance()
{
   mObjectMap.clear();
}


QObject *SingleInstance::value( const QString &typeName )
{
   return mObjectMap.value( typeName, 0 );
}


void SingleInstance::insert( const QString &typeName, QObject *object )
{
   mObjectMap.insert( typeName, object );
   connect( object, SIGNAL(destroyed(QObject*)),
            this, SLOT(remove(QObject*)) );
}


void SingleInstance::remove( QObject *object )
{
   QString typeName( mObjectMap.key( object, QString() ) );
   if( !typeName.isEmpty() )
   {
      mObjectMap.remove( typeName );
   }
}

