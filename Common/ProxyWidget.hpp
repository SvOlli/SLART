/**
 * ProxyWidget.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef PROXYWIDGET_HPP
#define PROXYWIDGET_HPP PROXYWIDGET_HPP

#include <QGroupBox>

class QGridLayout;
class QLabel;
class QPushButton;
class QLineEdit;
class QListWidget;
class QSpinBox;
class QHttp;

class ProxyWidget : public QGroupBox
{
Q_OBJECT
   
public:
   ProxyWidget( QWidget *parent = 0 );

   /* set proxy in QHttp class */
   void setProxy( QHttp *http );
   /* read settings from storage */
   void readSettings();
   /* write settings from storage */
   void writeSettings();

public slots:
   /* enable/disable widget according to settings */
   void updateWidgets( bool disabled = false );

private:
   QLabel       *mpProxyOnLabel;
   QLabel       *mpProxyHostLabel;
   QLabel       *mpProxyPortLabel;
   QLabel       *mpProxyAuthLabel;
   QLabel       *mpProxyLoginLabel;
   QLabel       *mpProxyPasswordLabel;
   QPushButton  *mpProxyOnButton;
   QLineEdit    *mpProxyHostInput;
   QSpinBox     *mpProxyPortInput;
   QPushButton  *mpProxyAuthButton;
   QLineEdit    *mpProxyLoginInput;
   QLineEdit    *mpProxyPasswordInput;
};

#endif
