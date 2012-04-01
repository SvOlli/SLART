/*
 * src/apps/Innuendo/DropDialog.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "DropDialog.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <AboutWidget.hpp>
#include <MySettings.hpp>
#include <ProxyWidget.hpp>

/* local headers */


DropDialog::DropDialog( QWidget *parent, Qt::WindowFlags flags )
: QDialog( parent, flags )
, mpMimeTypes( new QComboBox( this ) )
, mpClipboard( new QPushButton( tr("Get Clipboard"), this ) )
, mpColor( new QPushButton( tr("Color"), this ) )
, mpHtml( new QPushButton( tr("HTML"), this ) )
, mpText( new QPushButton( tr("Text"), this ) )
, mpUrls( new QPushButton( tr("URLs"), this ) )
, mpTextBrowser( new QTextBrowser( this ) )
, mpSignalMapper( new QSignalMapper( this ) )
, mMimeDataCache()
{
   setWindowTitle( QApplication::applicationName() + ": " + tr("Drop Info") );
   QPushButton *okButton = new QPushButton( tr("OK"), this );
   connect( mpClipboard, SIGNAL(clicked()),
            this, SLOT(handleClipboard()) );
   connect( mpColor, SIGNAL(clicked()),
            mpSignalMapper, SLOT(map()) );
   connect( mpHtml, SIGNAL(clicked()),
            mpSignalMapper, SLOT(map()) );
   connect( mpText, SIGNAL(clicked()),
            mpSignalMapper, SLOT(map()) );
   connect( mpUrls, SIGNAL(clicked()),
            mpSignalMapper, SLOT(map()) );
   mpSignalMapper->setMapping( mpColor, mpMimeTypes->findText( "application/x-color" ) );
   mpSignalMapper->setMapping( mpHtml,  mpMimeTypes->findText( "text/html" ) );
   mpSignalMapper->setMapping( mpText,  mpMimeTypes->findText( "text/plain" ) );
   mpSignalMapper->setMapping( mpUrls,  mpMimeTypes->findText( tr("URLs List") ) );

   QBoxLayout *buttonLayout = new QHBoxLayout;
   buttonLayout->addWidget( mpClipboard );
   buttonLayout->addWidget( mpColor );
   buttonLayout->addWidget( mpHtml );
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


void DropDialog::getMimeData( const QMimeData *remoteMimeData )
{
   int i;
   QByteArray mimeData;
   mpMimeTypes->clear();
   mMimeDataCache.clear();

   if( remoteMimeData->hasUrls() )
   {
      mpMimeTypes->addItem( tr("URLs List") );
   }
   mpMimeTypes->addItems( remoteMimeData->formats() );

   QString text;
   foreach( const QUrl &url, remoteMimeData->urls() )
   {
      text.append( url.toString() );
      text.append( "\n" );
   }
   mMimeDataCache.append( text );

   for( i = 1; i < mpMimeTypes->count(); i++ )
   {
      mimeData = remoteMimeData->data( mpMimeTypes->itemText(i) );
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
   mpColor->setDisabled( !remoteMimeData->hasColor() );
   mpHtml->setDisabled( !remoteMimeData->hasHtml() );
   mpText->setDisabled( !remoteMimeData->hasText() );
   mpUrls->setDisabled( !remoteMimeData->hasUrls() );

   mimeDataByIndex( 0 );
}


void DropDialog::dropEvent( QDropEvent *event )
{
   getMimeData( event->mimeData() );

   event->acceptProposedAction();
}


void DropDialog::handleClipboard()
{
   QClipboard *clipboard = QApplication::clipboard();
   if( clipboard->mimeData()->formats().size() > 0 )
   {
      getMimeData( clipboard->mimeData() );
   }
}
