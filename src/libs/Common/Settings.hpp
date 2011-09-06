/**
 * src/libs/Common/Settings.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef SETTINGS_HPP
#define SETTINGS_HPP SETTINGS_HPP

/* base class */

/* system headers */

/* Qt headers */
#include <QPointer>

/* local library headers */
#include "MySettings.hpp"

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */

/*!
  \addtogroup Common
  @{
  */

/*!
 \brief

*/
class Settings
{
public:
   /*!
    \brief parameter for value() / setValue (for boolean types)

   */
   enum StrippedBool
   {
      StrippedAutoEject, StrippedAutoEnqueue, StrippedAutoFreeDB,
      StrippedCDTextLatin1, StrippedDirectoryOverride, StrippedShowStats
   };
   /*!
    \brief parameter for value() / setValue (for string types)

   */
   enum StrippedString
   {
      StrippedCreatePattern, StrippedDevice, StrippedDirectory,
      ParameterStyleSheet
   };

   /*!
    \brief create and get global instance

    \param cdreader
    \param parent
    \param flags
   */
   static void createGlobal( QObject *parent = 0 );

   /*!
    \brief destroy global instance

   */
   static void destroyGlobal();

   /*!
    \brief get value from settings storage

    \param id key to get value for
   */
   static bool value( enum StrippedBool id );
   /*!
    \brief set value from settings storage

    \param id key to set value for
    \param value value to set
   */
   static void setValue( enum StrippedBool id, bool value );
   /*!
    \brief get value from settings storage

    \param id key to get value for
   */
   static QString value( enum StrippedString id );
   /*!
    \brief set value from settings storage

    \param id key to set value for
    \param value value to set
   */
   static void setValue( enum StrippedString id, const QString &value );

private:
   static QPointer<MySettings>      cpSettings; /*!< TODO */
};

/*! @} */

#endif
