/**
 * src/libs/Common/MainWindow.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP MAINWINDOW_HPP

#include <QMainWindow>

#include <QPoint>

class QApplication;
class QDir;

class MainWindow : public QMainWindow
{
Q_OBJECT
   
public:
   MainWindow( bool saveWindow = true,
               QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   virtual ~MainWindow();
   /* set the main widget */
   void setMainWidget( QWidget *mainWidget );
   /* add the MainWidget to the WidgetShot class */
   void enableScreenshot();

public slots:
   /* handle request for new icon and title */
   void changeTitle( const QIcon &icon, const QString &title );

protected:
   /* intercept for writing the settings */
   virtual void closeEvent( QCloseEvent *event );
   /* very ugly workaround for wrong position restoration on Ubuntu */
   virtual bool event( QEvent *event );
   
private:
   MainWindow( const MainWindow &other );
   MainWindow &operator=( const MainWindow &other );
   
   bool          mProhibitCloseWindow;
   bool          mSaveWindow;
   int           mForbidMove;
   QWidget       *mpMainWidget;
};

#endif
