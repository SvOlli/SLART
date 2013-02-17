/*
 * src/libs/Common/SingleInstance.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef SINGLEINSTANCE_HPP
#define SINGLEINSTANCE_HPP SINGLEINSTANCE_HPP

/* base class */
#include <QObject>

/* system headers */

/* Qt headers */
#include <QCoreApplication>
#include <QMap>
#include <QMutex>
#include <QString>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


/*!
  \addtogroup Common

  @{
  */

/*!
 \brief some kind of global singleton QObject* storage

 \dotfile "graphs/libs/Common/SingleInstance_connect.dot" "Connect Graph"
*/
class SingleInstance : public QObject
{
   Q_OBJECT

public:
   /*!
    \brief get (or create) the single instance of an object by name

    \param typeName name of type to get
    \return T* pointer to object
   */
   template<typename T>
   static T* get( const QString &typeName, bool useParent = true )
   {
      if( !cpSingleInstance )
      {
         cpSingleInstance = new SingleInstance( QCoreApplication::instance() );
      }
      QMutexLocker locker( &cpSingleInstance->mMapMutex );
      QObject *object = cpSingleInstance->value( typeName );
      if( !object )
      {
         if( useParent )
         {
            object = new T( cpSingleInstance );
         }
         else
         {
            object = new T( static_cast<QObject*>(0) );
         }
         cpSingleInstance->insert( typeName, object );
      }
      return qobject_cast<T*>( object );
   }

private slots:
   /*!
    \brief remove a destroyed object from map

    \param object registered object
   */
   void remove( QObject *object );

private:
   /*!
    \brief constructor (only used internally)

    \param parent parent object
   */
   SingleInstance( QObject *parent = 0 );

   /*!
    \brief destructor will be called by destruction of Q(Core)Application object

   */
   virtual ~SingleInstance();

   /*!
    \brief for internal use: get an object from map

    \param typeName name of registered object
    \return QObject* registered object
   */
   QObject *value( const QString &typeName );

   /*!
    \brief for internal use: insert an object into map

    \param typeName name of object to register
    \param object object to register
   */
   void insert( const QString &typeName, QObject *object );

   Q_DISABLE_COPY( SingleInstance )

   QMutex                  mMapMutex; /*!< \brief lock access to mObjectMap */
   QMap<QString,QObject*>  mObjectMap; /*!< \brief map of registered objects */
   static SingleInstance   *cpSingleInstance; /*!< \brief pointer of global SingleInstance */
};

/*! @} */

#endif
