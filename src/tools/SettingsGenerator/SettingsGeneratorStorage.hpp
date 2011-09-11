/**
 * src/tools/SettingsGenerator/SettingsGeneratorStorage.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef SETTINGSGENERATORSTORAGE_HPP
#define SETTINGSGENERATORSTORAGE_HPP SETTINGSGENERATORSTORAGE_HPP

/* system headers */

/* Qt headers */
#include <QStringList>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */

/*!
  \addtogroup SettingsGenerator SettingsGenerator: helper tool to generate
  settings handling class

 @{
 */

/*!
 \brief handling of command line parameters

*/
class SettingsGeneratorStorage
{
public:
   /*!
    \brief constructor

    \param applicationName
    \param type
   */
   SettingsGeneratorStorage( const QString &applicationName,
                             const QString &type );

   /*!
    \brief destructor

   */
   virtual ~SettingsGeneratorStorage();

   /*!
    \brief copy constructor

    \param that the object to copy from
   */
   SettingsGeneratorStorage( const SettingsGeneratorStorage &that );

   /*!
    \brief assignment operator

    \param that the object to copy from
   */
   SettingsGeneratorStorage &operator=( const SettingsGeneratorStorage &that );

   /*!
    \brief add a parameter to internal data

    \param name name of the parameter
    \param defaultValue the default value of the parameter
   */
   void addParameter( const QString &name, const QString &defaultValue );

   /*!
    \brief generate enums from internal data

   */
   QStringList enums() const;

   /*!
    \brief generate declations from internal data

   */
   QStringList declaration() const;

   /*!
    \brief generate main cpp code from internal data

   */
   QStringList definition() const;

   /*!
    \brief generate cleanup code

   */
   QStringList cleanup() const;

private:
   /*!
    \brief generate enum type name

    \return the generated enum type name
   */
   QString enumTypeName( int index = -1 ) const;

   QString           mApplicationName; /*!< \brief name of application */
   QStringList       mParts; /*!< \brief the parts of the registry path, except for first */
   QString           mTopPart; /*!< \brief first part of the registry path */
   QString           mType; /*!< \brief type like "bool" or "QString" */
   QString           mTypeName; /*!< \brief extended type name like "bool " or "const QString &" */
   QStringList       mParameterNames; /*!< \brief name of parameters */
   QStringList       mDefaultValues; /*!< \brief default values */
   QStringList       mDeleteNames; /*!< \brief name of obsolete parameters for deletion */
};

/*! @} */

#endif

