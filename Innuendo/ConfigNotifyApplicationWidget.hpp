/**
 * ConfigNotifyApplicationWidget.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef ConfigNotifyApplicationWidget_HPP
#define ConfigNotifyApplicationWidget_HPP ConfigNotifyApplicationWidget_HPP

#include <QWidget>
#include <QStringList>
#include "ConfigNotifyWidget.hpp"

class QGridLayout;
class QLabel;
class QCheckBox;
class QLineEdit;
class QListWidget;
class QSpinBox;
class QHttp;
class QTabWidget;

class ConfigNotifyApplicationWidget : public QWidget
{
Q_OBJECT
   
public:
   ConfigNotifyApplicationWidget( int index, const QStringList &apps, ConfigNotifyWidget *parent );
   virtual ~ConfigNotifyApplicationWidget();

   /* allow setting of a notification to another application */
   void allowNotify( int index, bool allow = true );
   /* get the SLART UDP listener port for config'ed application */
   int getUDPListenerPort();
   /* set all notifications at once */
   void setAllAtOnce( bool enable );

public slots:
   /* read settings from storage */
   void readSettings();
   /* write settings to storage */
   void writeSettings();
   /* handle UDP listen checkbox */
   void handleUDPListen( bool checked );

signals:

private:
   ConfigNotifyWidget   *mpNotifyWidget;
   QCheckBox            *mpSLARTCommunication;
   QLabel               *mpUDPListenerPortLabel;
   QSpinBox             *mpUDPListenerPort;
   QCheckBox            **mpSendsTo;
   int                  mIndex;
   QStringList          mApplications;
};

#endif
