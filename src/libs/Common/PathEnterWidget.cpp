/*
 * src/libs/Common/PathEnterWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "PathEnterWidget.hpp"

/* system headers */

/* Qt headers */
#include <QCompleter>
#include <QDirModel>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMimeData>
#include <QPushButton>
#include <QTimer>
#include <QUrl>

/* local library headers */

/* local headers */


PathEnterWidget::PathEnterWidget( QWidget *parent )
: QWidget( parent )
, mpLineEdit( new QLineEdit( this ) )
, mpDirModel( new QDirModel( this ) )
, mDirOnly( true )
, mBrowseMessage()
{
   setup();
}


PathEnterWidget::PathEnterWidget( const QString &content, QWidget *parent )
: QWidget( parent )
, mpLineEdit( new QLineEdit( content, this ) )
, mpDirModel( new QDirModel( this ) )
, mDirOnly( true )
, mBrowseMessage()
{
   setup();
}


void PathEnterWidget::setup()
{
   mpLineEdit->setAcceptDrops( false );
   connect( mpLineEdit, SIGNAL(textChanged(QString)),
            this, SIGNAL(textChanged(QString)) );
   connect( mpLineEdit, SIGNAL(textEdited(QString)),
            this, SIGNAL(textEdited(QString)) );
   connect( mpLineEdit, SIGNAL(returnPressed()),
            this, SIGNAL(returnPressed()) );

   QPushButton *button = new QPushButton( "...", this );
   button->setMaximumWidth( button->height() );
   connect( button, SIGNAL(clicked()),
            this, SLOT(browse()) );
   connect( mpLineEdit, SIGNAL(returnPressed()),
            this, SLOT(sendFileName()) );

   QCompleter *completer = new QCompleter( this );
   completer->setModel( mpDirModel );
   mpDirModel->setFilter( QDir::AllDirs | QDir::NoDotAndDotDot );
   mpDirModel->setSorting( QDir::Name );
   mpLineEdit->setCompleter( completer );

   QBoxLayout *layout = new QHBoxLayout( this );
   layout->setContentsMargins( 0, 0, 0, 0 );
   layout->addWidget( mpLineEdit );
   layout->addWidget( button );
   setLayout( layout );

   setAcceptDrops( true );
}


PathEnterWidget::~PathEnterWidget()
{
}


QLineEdit *PathEnterWidget::lineEdit() const
{
   return mpLineEdit;
}


QDirModel *PathEnterWidget::dirModel() const
{
   return mpDirModel;
}


QString PathEnterWidget::text() const
{
   return mpLineEdit->text();
}


bool PathEnterWidget::dirOnly() const
{
   return mDirOnly;
}


void PathEnterWidget::setDirOnly( bool value )
{
   mDirOnly = value;
   if( value )
   {
      mpDirModel->setFilter( mpDirModel->filter() & ~QDir::Files );
   }
   else
   {
      mpDirModel->setFilter( mpDirModel->filter() | QDir::Files );
   }
}


QString PathEnterWidget::browseMessage() const
{
   return mBrowseMessage;
}


void PathEnterWidget::setBrowseMessage( const QString &text )
{
   mBrowseMessage = text;
}


void PathEnterWidget::setNameFilters( const QStringList &filters )
{
   mpDirModel->setNameFilters( filters );
}


QStringList PathEnterWidget::nameFilters()
{
   return mpDirModel->nameFilters();
}


void PathEnterWidget::setText( const QString &value )
{
   mpLineEdit->setText( value );
}


void PathEnterWidget::browse()
{
   QFileDialog fileDialog( this, mBrowseMessage );

   fileDialog.setNameFilters( mpDirModel->nameFilters() );
   fileDialog.setOption( QFileDialog::ShowDirsOnly, mDirOnly );
   QFileInfo qfi( mpLineEdit->text() );
   if( qfi.isDir() )
   {
      fileDialog.setDirectory( mpLineEdit->text() );
   }
   else
   {
      fileDialog.setDirectory( qfi.absolutePath() );
   }
   fileDialog.setReadOnly( true );

   if( fileDialog.exec() )
   {
      QString result( fileDialog.selectedFiles().at(0) );
      mpLineEdit->setText( result );
      sendFileName();
   }
}


void PathEnterWidget::sendFileName()
{
   emit newFileName( mpLineEdit->text() );
}


void PathEnterWidget::dragEnterEvent( QDragEnterEvent *event )
{
   const QMimeData *mimeData = event->mimeData();

   if( mimeData->hasUrls() && (mimeData->urls().size() > 0) )
   {
      QUrl url( mimeData->urls().at(0) );
      if( url.scheme() == "file" )
      {
         QFileInfo qfi( url.toLocalFile() );
         if( mDirOnly )
         {
            if( qfi.isDir() )
            {
               event->acceptProposedAction();
            }
         }
         else
         {
            QModelIndex qmi( mpDirModel->index( qfi.absoluteFilePath() ) );
            if( qmi.isValid() )
            {
               event->acceptProposedAction();
            }
         }
      }
   }
}


void PathEnterWidget::dropEvent( QDropEvent *event )
{
   const QMimeData *mimeData = event->mimeData();

   if( mimeData->hasUrls() )
   {
      QList<QUrl> urls( mimeData->urls() );
      if( urls.size() > 0 )
      {
         QUrl url( urls.at(0) );
         if( url.scheme() == "file" )
         {
            mpLineEdit->setText( url.toLocalFile() );
            event->acceptProposedAction();
            QTimer::singleShot( 0, this, SLOT(sendFileName()) );
         }
      }
   }
}
