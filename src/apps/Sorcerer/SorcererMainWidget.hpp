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

  \brief application: setup wizard
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

private:
   Q_DISABLE_COPY( SorcererMainWidget )

   DatabaseInterface       *mpDatabase; /*!< \brief \todo complete documentation */
   QTabWidget              *mpTabs; /*!< \brief \todo complete documentation */
   QLabel                  *mpHint; /*!< \brief \todo complete documentation */
   QPushButton             *mpNext; /*!< \brief \todo complete documentation */

   DatabaseWidget          *mpDatabaseWidget; /*!< \brief \todo complete documentation */
   SatelliteConfigWidget   *mpSatelliteConfigWidget; /*!< \brief \todo complete documentation */
   ProxyWidget             *mpProxyWidget; /*!< \brief \todo complete documentation */

   int                     mLastTab; /*!< \brief \todo complete documentation */
   bool                    mDatabaseOk; /*!< \brief \todo complete documentation */
   bool                    mCommunicationOk; /*!< \brief \todo complete documentation */
   bool                    mProxyOk; /*!< \brief \todo complete documentation */
   const QString           mQuit; /*!< \brief \todo complete documentation */
   const QString           mStart; /*!< \brief \todo complete documentation */
};

/*! @} */

#endif
