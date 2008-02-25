/**
 * MainWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "MainWidget.hpp"
#include "ButtonsWidget.hpp"
#include "MySettings.hpp"

#include <QtGui>

#include "Trace.hpp"


MainWidget::MainWidget( QWidget *parent , Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpFileName( new QLineEdit( this ) )
, mpListButtons( new ButtonsWidget( this ) )
, mSLATCom()
{
   MySettings settings;
   
   QGridLayout *mainLayout   = new QGridLayout( this );
   
#if QT_VERSION < 0x040300
   mainLayout->setMargin( 3 );
#else
   mainLayout->setContentsMargins( 3, 3, 3, 3 );
#endif
   
   QLabel *mpLogo = new QLabel( this );
   mpLogo->setText( QApplication::applicationName() );
   mpLogo->setAlignment( Qt::AlignCenter );
   mpLogo->setFrameShadow( QFrame::Raised );
   mpLogo->setFrameShape( QFrame::Box );
   
   mpFileName->setReadOnly( true );
   
   QPushButton *settingsButton = new QPushButton( tr("Settings"), this );

   mainLayout->addWidget( mpLogo,         0, 0, 1, 3 );
   mainLayout->addWidget( mpFileName,     1, 0, 1, 3 );
   mainLayout->addWidget( mpListButtons,  2, 0, 1, 3 );
   mainLayout->addWidget( settingsButton, 3, 0, 1, 1 );
   
   setLayout( mainLayout );
   
   mpListButtons->updateButtons( settings.value( "Playlists", QStringList() ).toStringList() );

   connect( settingsButton, SIGNAL(clicked()),
            this, SLOT(handleSettings()) );
   connect( &mSLATCom, SIGNAL(packageRead(QStringList)),
            this, SLOT(handleSLAT(QStringList)) );
#if 0
   connect( &mSLATCom, SIGNAL(updateConfig()),
            mpConfig, SLOT(readSettings()) );
#endif
   
   connect( mpListButtons, SIGNAL(clicked(const QString &)),
            this, SLOT(addToList(const QString &)) );
   
   mSLATCom.resetReceiver();
}


void MainWidget::addToList( const QString &listfilename )
{
   int i;
   QFile m3uFile( listfilename );
   QStringList list( mpFileName->text() );
   
   m3uFile.open( QIODevice::ReadOnly | QIODevice::Text );
   while (!m3uFile.atEnd())
   {
      QByteArray line( m3uFile.readLine() );
      
      QString filename(QString::fromLocal8Bit(line));
      if( filename.left(1) != "#" )
      {
         if( filename.right(1) == QChar('\n') )
         {
            filename.chop(1);
         }
         if( !filename.startsWith( "/" ) )
         {
            /* a bit of an ugly trick, but gets the job done better than most
               other solutions */
            filename = QFileInfo( listfilename + "/../" + filename ).absoluteFilePath();
         }
         list << filename;
      }
   }
   m3uFile.close();
   
   list.sort();
   
   m3uFile.open( QIODevice::WriteOnly | QIODevice::Text );
   
   for( i = 0; i < list.count(); i++ )
   {
      QByteArray line( list.at(i).toLocal8Bit() );
      line.append('\n');
      
      m3uFile.write( line );
   }
   
   m3uFile.close();
}


void MainWidget::handleSettings()
{
#if 0
   mpConfig->readSettings();
   mpConfig->exec();
#endif
}


void MainWidget::handleSLAT( const QStringList &message )
{
   if( message.count() > 1 )
   {
      if( message.at(0) == "p0p" )
      {
         mpFileName->setText( message.at(1) );
         mpFileName->setCursorPosition( 0 );
         mpFileName->setCursorPosition( message.at(1).size() );
      }
   }
}
