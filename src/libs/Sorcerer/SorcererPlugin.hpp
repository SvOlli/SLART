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


class SorcererPlugin : public QObject, public SorcererInterface
{
Q_OBJECT
Q_INTERFACES(SorcererInterface)

public:
   SorcererPlugin();
   virtual ~SorcererPlugin();
   /* run main setup */
   virtual int setup( QApplication *app );
   /* run cleanup of obsolete registry settings */
   virtual void cleanup( QApplication *app );
   /* run installation of hidden registry settings */
   virtual void hidden();

private:
   SorcererPlugin( const SorcererPlugin &that );
   SorcererPlugin &operator=( const SorcererPlugin &that );

   /* subroutine that cleans up common settings for a program */
   void cleanupSettings( MySettings *settings );
   /* subroutine that does not overwrite any settings */
   void setDefault( MySettings *settings, const QString &name,
                    const QVariant &value );

   MySettings mGlobal;
   MySettings mFunkytown;
   MySettings mInnuendo;
   MySettings mKarmadrome;
   MySettings mNotorious;
   MySettings mPartyman;
   MySettings mRubberbandman;
   MySettings mStripped;
};

#endif
