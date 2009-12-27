/**
 * src/apps/Innuendo/DropDialog.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#include "DropDialog.hpp"
#include "MySettings.hpp"
#include "ProxyWidget.hpp"
#include "ConfigNotifyWidget.hpp"
#include "GlobalConfigWidget.hpp"
#include "AboutWidget.hpp"


#include <QtGui>
#include "SLARTCom.hpp"

#include "Trace.hpp"


DropDialog::DropDialog( QWidget *parent, Qt::WindowFlags flags )
: QDialog( parent, flags )
, mpMimeTypes( new QComboBox( this ) )
, mpColor( new QPushButton( tr("Color"), this ) )
, mpHtml( new QPushButton( tr("Html"), this ) )
, mpImage( new QPushButton( tr("Image"), this ) )
, mpText( new QPushButton( tr("Text"), this ) )
, mpUrls( new QPushButton( tr("Urls"), this ) )
, mpTextBrowser( new QTextBrowser( this ) )
, mpSignalMapper( new QSignalMapper( this ) )
, mMimeDataCache()
{
   setWindowTitle( QApplication::applicationName()+tr(" Drop Info") );
   QPushButton *okButton = new QPushButton( tr("OK"), this );
   connect( mpColor, SIGNAL(clicked()),
            mpSignalMapper, SLOT(map()) );
   connect( mpHtml, SIGNAL(clicked()),
            mpSignalMapper, SLOT(map()) );
   connect( mpText, SIGNAL(clicked()),
            mpSignalMapper, SLOT(map()) );
   connect( mpUrls, SIGNAL(clicked()),
            mpSignalMapper, SLOT(map()) );
   
   QBoxLayout *buttonLayout = new QHBoxLayout;
   buttonLayout->addWidget( mpColor );
   buttonLayout->addWidget( mpHtml );
   buttonLayout->addWidget( mpImage );
   buttonLayout->addWidget( mpText );
   buttonLayout->addWidget( mpUrls );
   
   QBoxLayout *layout = new QVBoxLayout( this );
   layout->addWidget( mpMimeTypes );
   layout->addLayout( buttonLayout );
   layout->addWidget( mpTextBrowser );
   layout->addWidget( okButton );
   
   connect( okButton, SIGNAL(clicked()),
            this, SLOT(accept()) );
   connect( mpMimeTypes, SIGNAL(currentIndexChanged(int)),
            this, SLOT(mimeDataByIndex(int)) );
   connect( mpSignalMapper, SIGNAL(mapped(int)),
            mpMimeTypes, SLOT(setCurrentIndex(int)));
   
   setMinimumSize( 600, 400 );
   setLayout( layout );
   
   setAcceptDrops( true );
}


void DropDialog::mimeDataByIndex( int index )
{
   if( (index < 0) || (index >= mMimeDataCache.size()) )
   {
      mpTextBrowser->clear();
   }
   else
   {
      mpTextBrowser->setPlainText( mMimeDataCache.at(index) );
   }
}


void DropDialog::dragEnterEvent( QDragEnterEvent *event )
{
   event->acceptProposedAction();
}


void DropDialog::dropEvent( QDropEvent *event )
{
   int i;
   QByteArray mimeData;
   mpMimeTypes->clear();
   mMimeDataCache.clear();
   if( event->mimeData()->hasUrls() )
   {
      mpMimeTypes->addItem( tr("Urls List") );
   }
   mpMimeTypes->addItems( event->mimeData()->formats() );
   
   QList<QUrl> urls = event->mimeData()->urls();
   QString text;
   for( i = 0; i < urls.size(); i++ )
   {
      text.append( urls.at(i).toString() );
      text.append( "\n" );
   }
   mMimeDataCache.append( text );

   for( i = 1; i < mpMimeTypes->count(); i++ )
   {
      mimeData = event->mimeData()->data( mpMimeTypes->itemText(i) );
      int size = mimeData.size();
      if( !size || (size % 2) || mimeData.at(1) )
      {
         mMimeDataCache.append( QString( mimeData ) );
      }
      else
      {
         mMimeDataCache.append( QString("UTF16:\n")+QString::fromUtf16( (const ushort*)mimeData.constData(), size/2 ) );
      }
   }
   mpColor->setDisabled( !event->mimeData()->hasColor() );
   mpHtml->setDisabled( !event->mimeData()->hasHtml() );
   mpImage->setDisabled( !event->mimeData()->hasImage() );
   mpText->setDisabled( !event->mimeData()->hasText() );
   mpUrls->setDisabled( !event->mimeData()->hasUrls() );

   mpSignalMapper->setMapping( mpColor, mpMimeTypes->findText( "application/x-color" ) );
   mpSignalMapper->setMapping( mpHtml,  mpMimeTypes->findText( "text/html" ) );
   mpSignalMapper->setMapping( mpText,  mpMimeTypes->findText( "text/plain" ) );
   mpSignalMapper->setMapping( mpUrls,  mpMimeTypes->findText( tr("Urls List") ) );

   mimeDataByIndex( 0 );
   
   event->acceptProposedAction();
}
