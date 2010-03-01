/**
 * src/libs/Common/ProxyWidget.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
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
   /* set proxy in QNetworkAccessManager class */
   static void setProxy( QNetworkAccessManager *nam );
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
