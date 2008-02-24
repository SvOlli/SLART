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
//class QTabBar;
class QTabWidget;

class ConfigNotifyApplicationWidget : public QWidget
{
Q_OBJECT
   
public:
   ConfigNotifyApplicationWidget( int index, const QStringList &apps, ConfigNotifyWidget *parent );
   virtual ~ConfigNotifyApplicationWidget();

   void allowNotify( int index, bool allow = true );
   int getUDPListenerPort();
   void setAllAtOnce( bool enable );

public slots:
   void readSettings();
   void writeSettings();
   void handleUDPListen( bool checked );

signals:

private:
   ConfigNotifyWidget   *mpNotifyWidget;
   QCheckBox            *mpSLATCommunication;
   QLabel               *mpUDPListenerPortLabel;
   QSpinBox             *mpUDPListenerPort;
   QCheckBox            **mpSendsTo;
   int                  mIndex;
   QStringList          mApplications;
};

#endif
