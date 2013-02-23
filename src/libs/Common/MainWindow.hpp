/*
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

  \brief library: common subroutines

  randomly thrown together collection of more or less useful subroutines
 @{
 */

/*!
 \brief the main window

 \dotfile "graphs/libs/Common/MainWindow_connect.dot" "Connect Graph"
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
    \brief set icon and/or title of all main windows (usually just one)

    \param icon icon to set (if not to be set, pass null QIcon())
    \param title icon to set (if not to be set, pass null QString())
   */
   static void setIconAndTitle( const QIcon &icon, const QString &title );

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

   bool          mProhibitCloseWindow; /*!< \brief \todo TODO */
   bool          mSaveWindow; /*!< \brief \todo TODO */
   int           mForbidMove; /*!< \brief \todo TODO */
   QWidget       *mpMainWidget; /*!< \brief \todo TODO */

   static MainWindow  *cpMainWindow; /*!< \brief handle for setIconAndTitle() */
};

/*! @} */

#endif
