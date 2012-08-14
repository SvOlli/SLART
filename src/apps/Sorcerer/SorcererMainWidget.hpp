/*
 * src/apps/Sorcerer/SorcererWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef SORCERERMAINWIDGET_HPP
#define SORCERERMAINWIDGET_HPP SORCERERMAINWIDGET_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */
#include <QString>

/* local library headers */
#include <TrackInfo.hpp>

/* local headers */

/* forward declaration of Qt classes */
class QIcon;
class QLabel;
class QPushButton;
class QTabWidget;

/* forward declaration of local classes */
class DatabaseInterface;
class DatabaseWidget;
class ProxyWidget;
class SatelliteConfigWidget;

/*!
  \addtogroup Sorcerer

  @{
  */


/*!
 \brief

 \dotfile "graphs/libs/Sorcerer/SorcererWidget_connect.dot" "Connect Graph"
*/
class SorcererMainWidget : public QWidget
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param parent parent widget
    \param flags window flags
   */
   SorcererMainWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   /*!
    \brief

   */
   virtual ~SorcererMainWidget();
   /*!
    \brief bitmask of errors occured

   */
   int errors();

public slots:
   /*!
    \brief handle the change of a tab

    \param newTab
   */
   void handleTabChange( int newTab );

   /*!
    \brief handle the next button

   */
   void handleNextButton();

   /*!
    \brief unlock the option to leave database tab when everything is ok

   */
   void unlockDatabase();

   /*!
    \brief handler for unlockDatabase() reply

    \param list
   */
   void countTracks( const TrackInfoList &list );

   /*!
    \brief unlock the option to leave communication tab

   */
   void unlockCommunication();

signals:
   /*!
    \brief request a new icon and title

    \param icon
    \param title
   */
   void requestChangeTitle( const QIcon &icon, const QString &title );

private:
   Q_DISABLE_COPY( SorcererMainWidget )

   DatabaseInterface       *mpDatabase; /*!< \todo */
   QTabWidget              *mpTabs; /*!< \todo */
   QLabel                  *mpHint; /*!< \todo */
   QPushButton             *mpNext; /*!< \todo */

   DatabaseWidget          *mpDatabaseWidget; /*!< \todo */
   SatelliteConfigWidget   *mpSatelliteConfigWidget; /*!< \todo */
   ProxyWidget             *mpProxyWidget; /*!< \todo */

   int                     mLastTab; /*!< \todo */
   bool                    mDatabaseOk; /*!< \todo */
   bool                    mCommunicationOk; /*!< \todo */
   bool                    mProxyOk; /*!< \todo */
   const QString           mQuit; /*!< \todo */
   const QString           mStart; /*!< \todo */
};

/*! @} */

#endif