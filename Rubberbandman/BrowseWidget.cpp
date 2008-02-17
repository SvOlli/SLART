/**
 * BrowseWidget.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "BrowseWidget.hpp"

#include <QtGui>
#include <QString>

#include "FileSysBrowser.hpp"
#include "InfoEdit.hpp"

BrowseWidget::BrowseWidget( QWidget *parent, Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpFileSysBrowser( new FileSysBrowser( tr("/media/share/Media/Music") ) )
, mpInfoEdit( new InfoEdit() )
{
   QVBoxLayout *layout = new QVBoxLayout;
   layout->addWidget( mpFileSysBrowser );
   layout->addWidget( mpInfoEdit );
   setLayout(layout);
   
   connect( mpFileSysBrowser, SIGNAL(clicked(const QString &)), mpInfoEdit, SLOT(load(const QString &)) );
}
