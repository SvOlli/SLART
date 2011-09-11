/**
 * src/tools/SettingsGenerator/SettingsGeneratorHandler.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef SETTINGSGENERATORHANDLER_HPP
#define SETTINGSGENERATORHANDLER_HPP SETTINGSGENERATORHANDLER_HPP

/* system headers */

/* Qt headers */
#include <QStringList>

/* local library headers */

/* local headers */
#include "SettingsGeneratorStorage.hpp"

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
class SettingsGeneratorHandler
{
public:
   /*!
    \brief constructor

    \param fileName name of ini file to load
   */
   SettingsGeneratorHandler( const QString &fileName );

   /*!
    \brief destructor

   */
   virtual ~SettingsGeneratorHandler();

   /*!
    \brief read the ini file

   */
   void read();

   /*!
    \brief generate set the number of spaces per indention

   */
   void setIndention( int spaces );

   /*!
    \brief generate enums from internal data

    \param start start level of indention
   */
   QString enums( int start ) const;

   /*!
    \brief generate declarations from internal data

    \param start start level of indention
   */
   QString declarations( int start ) const;

   /*!
    \brief generate main C++ code from internal data

   */
   QString definitions() const;

   /*!
    \brief generate includes from value specified in ini file

   */
   QString includes() const;

   /*!
    \brief the hpp file name as specified in ini file

   */
   QString hppFileName() const;

   /*!
    \brief the cpp file name as specified in ini file

   */
   QString cppFileName() const;

private:
   Q_DISABLE_COPY( SettingsGeneratorHandler )

   int                              mSpaces; /*!< \brief number of spaces per indention */
   QString                          mFileName; /*!< \brief file name of source ini file */
   QString                          mHppFileName; /*!< \brief name of generated hpp file */
   QString                          mCppFileName; /*!< \brief name of generated hpp file */
   QStringList                      mIncludes; /*!< \brief list of includes */
   QList<SettingsGeneratorStorage>  mStorageList; /*!< \brief internal data storage */
};

/*! @} */

#endif

