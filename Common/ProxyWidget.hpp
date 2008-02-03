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
   virtual ~ProxyWidget();
   void setProxy( QHttp *http );
   void readSettings();
   void writeSettings();

public slots:
   void updateWidgets( bool disabled = false );

signals:
   void closing( bool closing );

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
