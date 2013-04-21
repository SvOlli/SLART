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
#include <QProcess>
#include <QStringList>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QComboBox;
class QIcon;
class QLineEdit;
class QListWidget;
class QPushButton;
class QToolButton;

/* forward declaration of local classes */
class UnderpassConfigDialog;
class GenericSatelliteHandler;
class Satellite;
class StationStorage;


/*!
  \addtogroup Underpass

  \brief application: streamed audio player

  Named after [the song by John Foxx](https://en.wikipedia.org/wiki/John_Foxx#Singles).

  Underpass just is a small frontend intended for mpg123 and ogg123.

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

   void handleStationChange( const QString &name );

   void addStation();

   void startProcess( bool start = true );

   void readProcessOutput();

   void setPlayerText( const QString &text );

   void updateStationList();

   void systemMessage( const QString &text );

   void processError( QProcess::ProcessError error );

   void processFinished( int exitCode, QProcess::ExitStatus status );

private slots:

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

   static void setComboBoxByValue( QComboBox *comboBox, const QString &value );
   static void sortComboBox( QComboBox *comboBox );

   Satellite               *mpSatellite;
   StationStorage          *mpStorage;
   GenericSatelliteHandler *mpGenericSatelliteHandler;
   UnderpassConfigDialog   *mpConfig;
   QProcess                *mpProcess;
   QPushButton             *mpSettingsButton;
   QToolButton             *mpAddStationButton;
   QToolButton             *mpStartButton;
   QComboBox               *mpStation;
   QLineEdit               *mpUrl;
   QComboBox               *mpPlayer;
   QListWidget             *mpMessageBuffer;
   bool                    mDeleteLastLine;
   QString                 mLineBuffer;
   QString                 mLastStation;
};

/*! @} */

#endif
