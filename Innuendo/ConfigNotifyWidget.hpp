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

class QSignalMapper;
class QPushButton;
class ConfigNotifyApplicationWidget;

class ConfigNotifyWidget : public QWidget
{
Q_OBJECT
   
public:
   ConfigNotifyWidget( QWidget *parent = 0 );

   /* get the SLART UDP listener port for application */
   int getUDPListenerPort( int index );
   /* set all notifications at once */
   void setAllAtOnce( bool enable );

public slots:
   /* read settings from storage */
   void readSettings();
   /* write settings to storage */
   void writeSettings();
   /* handle "Full Communication" button */
   void enableFullCommunication();
   /* handle "No Communication" button */
   void disableFullCommunication();
   /* handle click of application button */
   void handleAppButton( int index );

signals:
   /*  */
   void fullNoCommunnicationClicked();

private:
   QSignalMapper                 *mpSignalMapper;
   QPushButton                   **mpApps;
   ConfigNotifyApplicationWidget **mpTabs;
   QStringList                   mApplications;
};

#endif
