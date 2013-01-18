/*
 * src/apps/Notorious/NotoriousMainWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "NotoriousMainWidget.hpp"

/* system headers */

/* Qt headers */
#include <QPushButton>
#include <QSplitter>
#include <QVBoxLayout>

/* local library headers */
#include <WidgetShot.hpp>

/* local headers */
#include "ConfigDialog.hpp"
#include "FreeDB.hpp"
#include "Foundlist.hpp"
#include "SearchWidget.hpp"


NotoriousMainWidget::NotoriousMainWidget( QWidget *parent, Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpFreeDB( new FreeDB( this ) )
, mpSearch( new SearchWidget( this ) )
, mpFound( new Foundlist( QThread::NormalPriority, this ) )
, mpCDInfo( new Foundlist( QThread::HighestPriority, this ) )
, mpConfig( new ConfigDialog( this ) )
, mpSettingsButton( new QPushButton( tr("Settings / Import / Log"), this ) )
{
   setMinimumSize( 600, 400 );
   QBoxLayout *mainLayout   = new QVBoxLayout( this );
   QSplitter  *splitter     = new QSplitter( this );

   splitter->setOrientation( Qt::Vertical );

   mainLayout->setContentsMargins( 3, 3, 3, 3 );
   parent->setWindowIcon( QIcon( ":/SLART.png" ) );

   splitter->addWidget( mpFound );
   splitter->addWidget( mpCDInfo );

   mainLayout->addWidget( mpSearch );
   mainLayout->addWidget( splitter );
   mainLayout->addWidget( mpSettingsButton );

   setLayout( mainLayout );

   connect( mpSearch, SIGNAL(submit(QString)),
            this, SLOT(runQuery(QString)) );
   connect( mpFound, SIGNAL(clicked(QString,QString)),
            this, SLOT(handleClicked(QString,QString)) );
   connect( mpFound, SIGNAL(queryRunning(bool)),
            mpSearch, SLOT(query1Running(bool)) );
   connect( mpCDInfo, SIGNAL(queryRunning(bool)),
            mpSearch, SLOT(query2Running(bool)) );
   connect( mpSettingsButton, SIGNAL(clicked()),
            mpConfig, SLOT(exec()) );

   mpSettingsButton->setObjectName( QString("SettingsButton") );

   WidgetShot::addWidget( "Main", this );
}


void NotoriousMainWidget::runQuery( const QString &query )
{
   mpFound->runQuery( query );
}


void NotoriousMainWidget::handleClicked( const QString &category, const QString &id )
{
   QString query( "id = '" );
   query.append( id );
   query.append( "'" );
   mpCDInfo->runQuery( query, category );
}
