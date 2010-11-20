/**
 * src/apps/Rubberbandman/BrowseWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */


/* class declaration */
#include "BrowseWidget.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>
#include <QString>

/* local library headers */
#include <Database.hpp>

/* local headers */
#include "FileSysBrowser.hpp"
#include "InfoEdit.hpp"


BrowseWidget::BrowseWidget( Database *database, QWidget *parent, Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpFileSysBrowser( new FileSysBrowser( database, this ) )
, mpInfoEdit( new InfoEdit( database, this ) )
{
   QBoxLayout *layout;
   if( QApplication::desktop()->screenGeometry().height() < 600 )
   {
      layout = new QHBoxLayout( this );
   }
   else
   {
      layout = new QVBoxLayout( this );
   }
   layout->addWidget( mpFileSysBrowser );
   layout->addWidget( mpInfoEdit );
   setLayout( layout );

   connect( mpFileSysBrowser, SIGNAL(clicked(const QString&)),
            mpInfoEdit, SLOT(load(const QString&)) );
   connect( mpInfoEdit, SIGNAL(updated()),
            mpFileSysBrowser, SLOT(handleRootDir()) );
}


void BrowseWidget::scrollTo( const QString &fileName )
{
   mpFileSysBrowser->scrollTo( fileName );
}
