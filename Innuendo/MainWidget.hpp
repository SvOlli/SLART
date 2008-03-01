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

#include "SLATCom.hpp"

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
   void readConfig();
   void setBufferSize( int size );
   void handleSLAT( const QStringList &message );
   void handleSettings();
   void listWidgetItemToClipboard( QListWidgetItem *item );

signals:
   void requestChangeTitle( const QIcon& ,const QString& );

protected:
   virtual void keyPressEvent( QKeyEvent *event );
   virtual void keyReleaseEvent( QKeyEvent *event );

private:
   MainWidget( const MainWidget &other );
   MainWidget &operator=( const MainWidget &other );

   QListWidget  *mpMessageBuffer;
   QPushButton  *mpSettingsButton;
   QLabel       *mpBufferSizeLabel;
   QSpinBox     *mpBufferSize;
   ConfigDialog *mpConfig;
   int          mBufferSize;
   SLATCom      mSLATCom;
};

#endif
