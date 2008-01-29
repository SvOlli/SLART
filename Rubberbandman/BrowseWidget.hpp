/**
 * BrowseWidget.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef BROWSEWIDGET_HPP
#define BROWSEWIDGET_HPP BROWSEWIDGET_HPP

#include <QWidget>
#include <QDir>

class QString;
class FileSysBrowser;
class InfoEdit;
class QPushButton;

class BrowseWidget : public QWidget
{
Q_OBJECT

public:
   BrowseWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );

public slots:

private:
   BrowseWidget( const BrowseWidget &other );
   BrowseWidget &operator=( const BrowseWidget &other );

   FileSysBrowser *mpFileSysBrowser;
   InfoEdit       *mpInfoEdit;
   QPushButton    *mpSetButton;
};

#endif
