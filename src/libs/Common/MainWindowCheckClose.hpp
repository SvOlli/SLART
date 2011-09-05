/**
 * src/libs/Common/MainWindowCheckClose.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef MAINWINDOWCHECKCLOSE_HPP
#define MAINWINDOWCHECKCLOSE_HPP

/* base class */
#include "MainWindow.hpp"

/* system headers */

/* Qt headers */
class QCloseEvent;

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


/*!
  \addtogroup Common

  @{
*/

/*!
 \brief TODO

*/
class MainWindowCheckClose : public MainWindow
{
   Q_OBJECT

public:
   MainWindowCheckClose( bool saveWindow = true,
                         QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   virtual ~MainWindowCheckClose();

public slots:
   /*!
    \brief prohibit closing of window (Partyman kiosk mode)

   */
   void prohibitClose( bool prohibit ) { mProhibitCloseWindow = prohibit; };

protected:
   /*!
    \brief intercept for writing the settings

   */
   virtual void closeEvent( QCloseEvent *event );

private:
   bool  mProhibitCloseWindow;
};

/*! @} */

#endif
