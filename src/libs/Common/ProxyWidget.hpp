/**
 * src/libs/Common/ProxyWidget.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef PROXYWIDGET_HPP
#define PROXYWIDGET_HPP PROXYWIDGET_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QHttp;
class QLabel;
class QLineEdit;
#if QT_VERSION >= 0x040400
class QNetworkAccessManager;
#endif
class QPushButton;
class QSpinBox;

/* forward declaration of local classes */


class ProxyWidget : public QWidget
{
Q_OBJECT
   
public:
   ProxyWidget( QWidget *parent = 0 );
   
   /* set proxy in QHttp class */
   static void setProxy( QHttp *http );
#if QT_VERSION >= 0x040400
   /* set proxy in QNetworkAccessManager class */
   static void setProxy( QNetworkAccessManager *nam );
#endif
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
