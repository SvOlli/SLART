/**
 * MainWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "MainWidget.hpp"

#include "ConfigDialog.hpp"
#include "FreeDB.hpp"
#include "Foundlist.hpp"
#include "SearchWidget.hpp"

#include <QtGui>


MainWidget::MainWidget( QWidget *parent, Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpFreeDB( new FreeDB( this ) )
, mpSearch( new SearchWidget( this ) )
, mpFound( new Foundlist( this ) )
, mpCDInfo( new Foundlist( this ) )
, mpConfig( new ConfigDialog( this ) )
{
   setMinimumSize( 600, 400 );
   QBoxLayout *mainLayout   = new QVBoxLayout( this );
   QSplitter  *splitter     = new QSplitter( this );
   QPushButton *settingsButton = new QPushButton( tr("Settings / Import / Log"), this );
   
   splitter->setOrientation( Qt::Vertical );
   
#if QT_VERSION < 0x040300
   mainLayout->setMargin( 3 );
#else
   mainLayout->setContentsMargins( 3, 3, 3, 3 );
#endif
   
   splitter->addWidget( mpFound );
   splitter->addWidget( mpCDInfo );
   
   mainLayout->addWidget( mpSearch );
   mainLayout->addWidget( splitter );
   mainLayout->addWidget( settingsButton );
   
   setLayout( mainLayout );
   
   connect( mpSearch, SIGNAL( submit( const QString &) ),
            this, SLOT( runQuery(const QString &) ) );
   connect( mpFound, SIGNAL( clicked( const QString &, const QString & ) ),
            this, SLOT( handleClicked( const QString &, const QString & ) ) );
   connect( mpFound, SIGNAL( queryRunning(bool) ),
            mpSearch, SLOT(query1Running(bool) ) );
   connect( mpCDInfo, SIGNAL( queryRunning(bool) ),
            mpSearch, SLOT(query2Running(bool) ) );
   connect( settingsButton, SIGNAL( clicked() ),
            mpConfig, SLOT( exec() ) );
}


void MainWidget::runQuery( const QString &query )
{
   mpFound->runQuery( query );
}


void MainWidget::handleClicked( const QString &category, const QString &id )
{
   QString query( "category = '" );
   query.append( category );
   query.append( "' AND id = '");
   query.append( id );
   query.append( "'" );
   mpCDInfo->runQuery( query );
}
