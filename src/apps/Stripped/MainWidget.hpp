/**
 * src/apps/Stripped/MainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP MAINWIDGET_HPP

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
 \brief the main widget

 \class MainWidget MainWidget.hpp "apps/Stripped/MainWidget.hpp"
*/
class MainWidget : public QWidget
{
Q_OBJECT

public:
   /*!
    \brief default constructor

    \fn MainWidget
    \param parent
    \param flags
   */
   MainWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   virtual ~MainWidget();

public slots:
   /*!
    \brief handle eject button

    \fn eject
   */
   void eject();

   /*!
    \brief set buttons active according to state "no drive found"

    \fn stateNoDrive
   */
   void stateNoDrive();

   /*!
    \brief set buttons active according to state "no disc inserted"

    \fn stateNoDisc
   */
   void stateNoDisc();

   /*!
    \brief set buttons active according to state "disc read"

    \fn stateDisc
   */
   void stateDisc();

   /*!
    \brief set buttons active according to state "scanning the disc"

    \fn stateScan
   */
   void stateScan();

   /*!
    \brief set buttons active according to state "connecting to freedb"

    \fn stateNet
   */
   void stateNet();

   /*!
    \brief set buttons active according to state "ripping tracks"

    \fn stateRip
   */
   void stateRip();

signals:
   /*!
    \brief request a new icon and title

    \fn requestChangeTitle
    \param icon
    \param title
   */
   void requestChangeTitle( const QIcon &icon, const QString &title );

private:
   MainWidget( const MainWidget &that );
   MainWidget &operator=( const MainWidget &that );

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

#endif
