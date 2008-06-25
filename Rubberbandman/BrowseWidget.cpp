/**
 * BrowseWidget.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "BrowseWidget.hpp"

#include <QtGui>
#include <QString>

#include "Database.hpp"
#include "FileSysBrowser.hpp"
#include "InfoEdit.hpp"

BrowseWidget::BrowseWidget( Database *database, QWidget *parent, Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpFileSysBrowser( new FileSysBrowser( database, this ) )
, mpInfoEdit( new InfoEdit( database, this ) )
{
   QVBoxLayout *layout = new QVBoxLayout;
   layout->addWidget( mpFileSysBrowser );
   layout->addWidget( mpInfoEdit );
   setLayout(layout);
   
   connect( mpFileSysBrowser, SIGNAL(clicked(const QString&)),
            mpInfoEdit, SLOT(load(const QString&)) );
}


void BrowseWidget::scrollTo( const QString &fileName )
{
   mpFileSysBrowser->scrollTo( fileName );
}
