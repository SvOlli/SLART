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
class QNetworkAccessManager;
class QPushButton;
class QSpinBox;

/* forward declaration of local classes */


/*!
  \addtogroup Common

  @{
*/

/*!
 \brief TODO

*/
class ProxyWidget : public QWidget
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param parent
   */
   ProxyWidget( QWidget *parent = 0 );

   /*!
    \brief destructor

   */
   virtual ~ProxyWidget();

   /*!
    \brief set proxy in QHttp class

    \param http
   */
   static void setProxy( QHttp *http );

   /*!
    \brief set proxy in QNetworkAccessManager class

    \param nam
   */
   static void setProxy( QNetworkAccessManager *nam );

   /*!
    \brief read settings from storage

   */
   void readSettings();

   /*!
    \brief write settings from storage

   */
   void writeSettings();

public slots:
   /*!
    \brief enable/disable widget according to settings

   */
   void updateWidgets( bool disabled = false );

private:
   Q_DISABLE_COPY( ProxyWidget )

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

/*! @} */

#endif
