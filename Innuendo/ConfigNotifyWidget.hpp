/**
 * ConfigNotifyWidget.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef CONFIGNOTIFYWIDGET_HPP
#define CONFIGNOTIFYWIDGET_HPP CONFIGNOTIFYWIDGET_HPP

#include <QWidget>
#include <QStringList>

class QGridLayout;
class QLabel;
class QPushButton;
class QLineEdit;
class QListWidget;
class QSpinBox;
class QHttp;
class ConfigNotifyApplicationWidget;
class QCheckBox;
class QTabWidget;

class ConfigNotifyWidget : public QWidget
{
Q_OBJECT
   
public:
   ConfigNotifyWidget( QWidget *parent = 0 );
   virtual ~ConfigNotifyWidget();
   int getUDPListenerPort( int index );
   void setAllAtOnce( bool enable );

public slots:
   void readSettings();
   void writeSettings();
   void enableFullCommunication();
   void disableFullCommunication();

signals:

private:
   QTabWidget   *mpTabWidget;
   ConfigNotifyApplicationWidget **mpTabs;
   QStringList  mApplications;
};

#endif
