/*
 * src/libs/Common/AboutWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef ABOUTWIDGET_HPP
#define ABOUTWIDGET_HPP ABOUTWIDGET_HPP

#include <QWidget>

class QUrl;


/*!
  \addtogroup Common

  @{
*/

/*!
 \brief \todo complete documentation

 \dotfile "graphs/libs/Common/AboutWidget_connect.dot" "Connect Graph"
*/
class AboutWidget : public QWidget
{
   Q_OBJECT

public:
   AboutWidget( QWidget *parent, const QString &aboutText = QString() );

public slots:
   /*!
    \brief handle links from about tab (wrapper)

   */
   void handleLink( const QUrl &url );
   /*!
    \brief handle links from about tab

   */
   void handleLink( const QString &url );

private:
   Q_DISABLE_COPY( AboutWidget )
};

/*! @} */

#endif
