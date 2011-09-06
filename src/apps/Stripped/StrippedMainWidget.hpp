/**
 * src/apps/Stripped/StrippedMainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef STRIPPEDMAINWIDGET_HPP
#define STRIPPEDMAINWIDGET_HPP STRIPPEDMAINWIDGET_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */
#include <QList>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QComboBox;
class QLabel;
class QPushButton;
class QHBoxLayout;

/* forward declaration of local classes */
class CDDBClient;
class CDEdit;
class CDInfo;
class CDList;
class CDReader;
class ConfigDialog;
class GenericSatMsgHandler;
class Satellite;


/*!
  \addtogroup Stripped Stripped: CD-ripping
  @{
  */

/*!
 \brief the main widget

*/
class StrippedMainWidget : public QWidget
{
   Q_OBJECT

public:
   /*!
    \brief default constructor

    \param parent
    \param flags
   */
   StrippedMainWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   virtual ~StrippedMainWidget();

public slots:
   /*!
    \brief handle eject button

   */
   void eject();

   /*!
    \brief set buttons active according to state "no drive found"

   */
   void stateNoDrive();

   /*!
    \brief set buttons active according to state "no disc inserted"

   */
   void stateNoDisc();

   /*!
    \brief set buttons active according to state "disc read"

   */
   void stateDisc();

   /*!
    \brief set buttons active according to state "scanning the disc"

   */
   void stateScan();

   /*!
    \brief set buttons active according to state "connecting to freedb"

   */
   void stateNet();

   /*!
    \brief set buttons active according to state "ripping tracks"

   */
   void stateRip();

signals:
   /*!
    \brief request a new icon and title

    \param icon
    \param title
   */
   void requestChangeTitle( const QIcon &icon, const QString &title );

private:
   Q_DISABLE_COPY( StrippedMainWidget )

public:
private:
   Satellite            *mpSatellite;
   GenericSatMsgHandler *mpGenericSatMsgHandler;
   CDInfo               *mpCDInfo;
   CDDBClient           *mpCDDBClient;
   CDEdit               *mpCDEdit;
   CDReader             *mpCDReader;
   QLabel               *mpMessage;
   ConfigDialog         *mpConfigDialog;
   QHBoxLayout          *mpButtonLayout;
   QPushButton          *mpSettingsButton;
   QPushButton          *mpCancelButton;
   QPushButton          *mpScanButton;
   QPushButton          *mpRipButton;
   QPushButton          *mpEjectButton;
};

/*! @} */

#endif
