/**
 * src/libs/Sorcerer/SorcererPlugin.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef PLUGIN_HPP
#define PLUGIN_HPP PLUGIN_HPP

/* base class */
#include <QObject>

/* system headers */

/* Qt headers */
#include <QWidget>

/* local library headers */

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
   /* run main setup */
   virtual int setup( QApplication *app );
   /* run cleanup of obsolete registry settings */
   virtual void cleanup( QApplication *app );
   /* run installation of hidden registry settings */
   virtual void hidden();

private:
   /*  */
   void cleanupSettings( MySettings *settings );
   /*  */
   void setDefault( MySettings *settings, const QString &name,
                    const QVariant &value );
};

#endif
