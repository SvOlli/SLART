/*
 * src/libs/Sorcerer/SorcererWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef SORCERERWIDGET_HPP
#define SORCERERWIDGET_HPP SORCERERWIDGET_HPP

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
class SorcererWidget : public QWidget
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param parent parent widget
    \param flags window flags
   */
   SorcererWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   /*!
    \brief

   */
   virtual ~SorcererWidget();
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
   Q_DISABLE_COPY( SorcererWidget )

   DatabaseInterface       *mpDatabase; /*!< TODO */
   QTabWidget              *mpTabs; /*!< TODO */
   QLabel                  *mpHint; /*!< TODO */
   QPushButton             *mpNext; /*!< TODO */

   DatabaseWidget          *mpDatabaseWidget; /*!< TODO */
   SatelliteConfigWidget   *mpSatelliteConfigWidget; /*!< TODO */
   ProxyWidget             *mpProxyWidget; /*!< TODO */

   int                     mLastTab; /*!< TODO */
   bool                    mDatabaseOk; /*!< TODO */
   bool                    mCommunicationOk; /*!< TODO */
   bool                    mProxyOk; /*!< TODO */
   const QString           mQuit; /*!< TODO */
   const QString           mStart; /*!< TODO */
};

/*! @} */

#endif
