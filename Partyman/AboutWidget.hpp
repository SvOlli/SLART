/**
 * AboutWidget.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef ABOUTWIDGET_HPP
#define ABOUTWIDGET_HPP 1

#include <QWidget>
class QUrl;
class ConfigDialog;

class AboutWidget : public QWidget
{
Q_OBJECT

public:
   AboutWidget( ConfigDialog *config, QWidget *parent );

public slots:
   /* handle links from about tab (wrapper) */
   void handleLink( const QUrl &url );
   /* handle links from about tab */
   void handleLink( const QString &url );

signals:

private:
   AboutWidget( const AboutWidget &other );
   AboutWidget &operator=( const AboutWidget &other );

   ConfigDialog         *mpConfig;
};

#endif
