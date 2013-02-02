/*
 * src/apps/Underpass/UnderpassMainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef UNDERPASSMAINWIDGET_HPP
#define UNDERPASSMAINWIDGET_HPP UNDERPASSMAINWIDGET_HPP

/* base class */
#include <QWidget>

/* system headers */
/* Qt headers */
#include <QStringList>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QComboBox;
class QIcon;
class QProcess;
class QPushButton;
class QLineEdit;
class QListWidget;

/* forward declaration of local classes */
class UnderpassConfigDialog;
class GenericSatelliteHandler;
class Satellite;


/*!
  \addtogroup Underpass Underpass: central application

  @{
*/

/*!
 \brief \todo complete documentation

 \dotfile "graphs/apps/Underpass/UnderpassMainWidget_connect.dot" "Connect Graph"
*/
class UnderpassMainWidget : public QWidget
{
   Q_OBJECT

public:
   UnderpassMainWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   virtual ~UnderpassMainWidget();

public slots:
   /*!
    \brief re-read config

   */
   void readConfig();
   /*!
    \brief handle SLART message

   */
   void handleSatellite( const QByteArray &msg );

   void readData( const QString &name );

   void startProcess( bool start );

   void readProcessOutput();

private slots:

signals:
   void requestChangeTitle( const QIcon &icon, const QString &title );

protected:
   /*!
    \brief for implementing dropping

   */
   virtual void dragEnterEvent( QDragEnterEvent *event );
   /*!
    \brief for implementing dropping

   */
   virtual void dropEvent( QDropEvent *event );

private:
   Q_DISABLE_COPY( UnderpassMainWidget )

   Satellite               *mpSatellite;
   GenericSatelliteHandler *mpGenericSatelliteHandler;
   UnderpassConfigDialog   *mpConfig;
   QProcess                *mpProcess;
   QPushButton             *mpSettingsButton;
   QPushButton             *mpStartButton;
   QComboBox               *mpStation;
   QLineEdit               *mpUrl;
   QComboBox               *mpPlayer;
   QListWidget             *mpMessageBuffer;
};

/*! @} */

#endif
