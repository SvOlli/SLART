/**
 * src/libs/Common/MainWindow.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP MAINWINDOW_HPP

/* base class */
#include <QMainWindow>

/* system headers */

/* Qt headers */
#include <QPoint>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */

/*!
  \addtogroup Common

  \brief also contains \ref Satellite
 @{
 */

class MainWindow : public QMainWindow
{
   Q_OBJECT

public:

   /*!
    \brief constructor

    \param saveWindow
    \param parent
    \param flags
   */
   MainWindow( bool saveWindow = true,
               QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   /*!
    \brief destructor

   */
   virtual ~MainWindow();
   /*!
    \brief set the main widget

    \param mainWidget
   */
   void setMainWidget( QWidget *mainWidget );
   /*!
    \brief add the MainWidget to the WidgetShot class

   */
   void enableScreenshot();

public slots:
   /*!
    \brief handle request for new icon and title

    \param icon
    \param title
   */
   void changeTitle( const QIcon &icon, const QString &title );

protected:
   /*!
    \brief intercept for writing the settings

    \param event
   */
   virtual void closeEvent( QCloseEvent *event );
   /*!
    \brief very ugly workaround for wrong position restoration on Ubuntu

    \param event
   */
   virtual bool event( QEvent *event );

private:
   Q_DISABLE_COPY( MainWindow )

   bool          mProhibitCloseWindow;
   bool          mSaveWindow;
   int           mForbidMove;
   QWidget       *mpMainWidget;
};

/*! @} */

#endif
