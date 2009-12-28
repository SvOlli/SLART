/**
 * src/libs/Common/AboutWidget.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef ABOUTWIDGET_HPP
#define ABOUTWIDGET_HPP ABOUTWIDGET_HPP

#include <QWidget>

class QUrl;


class AboutWidget : public QWidget
{
Q_OBJECT

public:
   AboutWidget( QWidget *parent, const QString &aboutText = QString() );

public slots:
   /* handle links from about tab (wrapper) */
   void handleLink( const QUrl &url );
   /* handle links from about tab */
   void handleLink( const QString &url );

private:
   AboutWidget( const AboutWidget &other );
   AboutWidget &operator=( const AboutWidget &other );
};

#endif
