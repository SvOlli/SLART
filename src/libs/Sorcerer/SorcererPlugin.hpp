/**
 * src/libs/Sorcerer/SorcererPlugin.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef PLUGIN_HPP
#define PLUGIN_HPP PLUGIN_HPP

/* base class */
#include <QObject>

/* system headers */

/* Qt headers */
#include <QWidget>

/* local library headers */
#include <MySettings.hpp>

/* local headers */
#include "SorcererInterface.hpp"

/* forward declaration of Qt classes */

/* forward declaration of local classes */
class MySettings;

/*!
  \addtogroup Sorcerer
  @{
  */

/*!
 \brief implementation of the setup wizard plugin

*/
class SorcererPlugin : public QObject, public SorcererInterface
{
   Q_OBJECT
   Q_INTERFACES(SorcererInterface)

public:
   /*!
    \brief constructor

   */
   SorcererPlugin();

   /*!
    \brief destructor

   */
   virtual ~SorcererPlugin();

   /*!
    \brief run the setup wizard

   */
   virtual int setup();

   /*!
    \brief cleanup of obsolete registry settings

   */
   virtual void cleanup();

   /*!
    \brief installation of hidden registry settings

   */
   virtual void hidden();

private:
   Q_DISABLE_COPY( SorcererPlugin )

   /*!
    \brief subroutine that cleans up common settings for a program

    \param pointer to settings to clean up
   */
   void cleanupSettings( MySettings *settings );

   /*!
    \brief subroutine that does not overwrite any settings

    \param settings pointer to settings
    \param name name of parameter
    \param value default value to set
   */
   void setDefault( MySettings *settings, const QString &name,
                    const QVariant &value );

   bool       mStopDatabase; /*!< TODO */
   bool       mStopSatellite; /*!< TODO */
   MySettings mGlobal; /*!< TODO */
   MySettings mFunkytown; /*!< TODO */
   MySettings mInnuendo; /*!< TODO */
   MySettings mKarmadrome; /*!< TODO */
   MySettings mNotorious; /*!< TODO */
   MySettings mPartyman; /*!< TODO */
   MySettings mRubberbandman; /*!< TODO */
   MySettings mStripped; /*!< TODO */
};

/*! @} */

#endif
