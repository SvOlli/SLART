/**
 * MainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP MAINWIDGET_HPP

#include <QWidget>
#include <QUdpSocket>
#include <QStringList>

#include "SLARTCom.hpp"

class QListWidget;
class QLabel;
class QSpinBox;
class QIcon;
class QString;
class QPushButton;
class QListWidgetItem;
class ConfigDialog;
class QKeyEvent;

class MainWidget : public QWidget
{
Q_OBJECT

public:
   MainWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );

public slots:
   /* re-read config */
   void readConfig();
   /* handle input of buffer size */
   void setBufferSize( int size );
   /* handle the ping button */
   void handlePingButton();
   /* handle SLART message */
   void handleSLART( const QStringList &message );
   /* copy clicked line of message buffer to clipboard */
   void listWidgetItemToClipboard( QListWidgetItem *item );

signals:
   void requestChangeTitle( const QIcon&, const QString& );

private:
   MainWidget( const MainWidget &other );
   MainWidget &operator=( const MainWidget &other );

   QListWidget  *mpMessageBuffer;
   QPushButton  *mpSettingsButton;
   QPushButton  *mpPingButton;
   QLabel       *mpBufferSizeLabel;
   QSpinBox     *mpBufferSize;
   ConfigDialog *mpConfig;
   int          mBufferSize;
   SLARTCom     mSLARTCom;
   QStringList  mApplications;
};

#endif
