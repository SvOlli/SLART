/*
 * src/libs/Common/GSettingsWrapper.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef GSETTINGSWRAPPER_HPP
#define GSETTINGSWRAPPER_HPP GSETTINGSWRAPPER_HPP

/* base class */
#include <QMap>

/* system headers */

/* Qt headers */
#include <QStringList>

/* local library headers */
#include <MyLua.hpp>

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


/*!
  \addtogroup Common

  @{
*/

/*!
 \brief \todo complete documentation

*/
class GSettingsWrapper
{
public:
   /*!
    \brief constructor

   */
   GSettingsWrapper( const QString &domain );
   /*!
    \brief destructor

   */
   virtual ~GSettingsWrapper();

   /*!
    \brief test if glib is capable of GSettings

    \return bool gsettings are available
   */
   static bool isAvailable();

   /*!
    \brief test if instance is connected to an GSettings object

    \return bool
   */
   bool isValid();
   /*!
    \brief get a boolean value

    \param key
    \return bool
   */
   bool valueBool( const QString &key );
   /*!
    \brief set a boolean value

    \param key
    \param value
    \return bool
   */
   bool setValueBool( const QString &key, bool value );
   /*!
    \brief get an integer value

    \param key
    \return int
   */
   int valueInt( const QString &key );
   /*!
    \brief set an integer value

    \param key
    \param value
    \return bool
   */
   bool setValueInt(const QString &key, int value );
   /*!
    \brief get a string value

    \param key
    \return QString
   */
   QString valueString( const QString &key );
   /*!
    \brief set a string value

    \param key
    \param value
    \return bool
   */
   bool setValueString( const QString &key, const QString &value );
   /*!
    \brief get a string list value

    \param key
    \return QString
   */
   QStringList valueStringList( const QString &key );
   /*!
    \brief set a string value

    \param key
    \param value
    \return bool
   */
   bool setValueStringList( const QString &key, const QStringList &value );

private:
   void     *mpGSettings; /*!< \brief pointer to GSettings */
};

/*! @} */

#endif
