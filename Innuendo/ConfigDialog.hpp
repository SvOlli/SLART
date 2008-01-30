/**
 * ConfigDialog.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef CONFIGDIALOG_HPP
#define CONFIGDIALOG_HPP CONFIGDIALOG_HPP

#include <QDialog>
class QSpinBox;
class QLabel;
class QLineEdit;
class QCheckBox;
class QComboBox;
class QDoubleSpinBox;
class ProxyWidget;
   

class ConfigDialog : public QDialog
{
Q_OBJECT

public:
   ConfigDialog( QWidget *parent = 0, Qt::WindowFlags flags = 0 );

public slots:
   void readSettings();
   void writeSettings();
   void handleUDPListen( bool checked );

signals:
   void configChanged();

private:
   ConfigDialog( const ConfigDialog &other );
   ConfigDialog &operator=( const ConfigDialog &other );

   ProxyWidget  *mpProxyWidget;
   QCheckBox    *mpSLATCommunication;
   QSpinBox     *mpUDPListenerPort;
   QCheckBox    *mpPartymanNotify;
   QCheckBox    *mpStrippedNotify;
   QCheckBox    *mpRubberbandmanNotify;
   QCheckBox    *mpFunkytownNotify;
};

#endif
