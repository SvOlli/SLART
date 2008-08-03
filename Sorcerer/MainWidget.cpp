/**
 * MainWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "MainWidget.hpp"
#include "AboutWidget.hpp"
#include "../Rubberbandman/DatabaseWidget.hpp"
#include "../Innuendo/ConfigNotifyWidget.hpp"
#include "ProxyWidget.hpp"
#include "Database.hpp"
#include "MySettings.hpp"

#include <QtGui>


MainWidget::MainWidget( QWidget *parent , Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpDatabase( new Database() )
, mpTabs( new QTabWidget( this ) )
, mpHint( new QLabel( this ) )
, mpNext( new QPushButton( tr("Next"), this ) )
, mpDatabaseWidget( new DatabaseWidget( mpDatabase, this ) )
, mpConfigCommunicationWidget( new ConfigNotifyWidget( this ) )
, mpProxyWidget( new ProxyWidget( this ) )
, mLastTab( 0 )
, mDatabaseOk( Database::exists() )
, mCommunicationOk( MySettings( "Innuendo" ).value( "SLARTCommunication" ).isValid() )
, mProxyOk( MySettings( "Global" ).value( "Enable" ).isValid() )
{
   int i;
   if( mDatabaseOk )
   {
      mDatabaseOk = mpDatabase->getTrackInfoList( 0 ) > 2;
   }
   
   QVBoxLayout *mainLayout = new QVBoxLayout( this );
#if QT_VERSION < 0x040300
   mainLayout->setMargin( 3 );
#else
   mainLayout->setContentsMargins( 3, 3, 3, 3 );
#endif
   
   mpConfigCommunicationWidget->readSettings();
   mpProxyWidget->readSettings();
   
   AboutWidget *about = new AboutWidget( this );
   QLabel *welcome    = new QLabel( tr("Some spiffy welcome text..."), this );
   QLabel *welldone   = new QLabel( tr("Some spiffy well done text..."), this );
   welcome->setAlignment( Qt::AlignCenter );
   welldone->setAlignment( Qt::AlignCenter );
   mpHint->setFrameShadow( QFrame::Raised );
   mpHint->setFrameShape( QFrame::Box );
   
   mpTabs->addTab( welcome,                     QString(tr("Welcome")) );
   mpTabs->addTab( mpDatabaseWidget,            QString(tr("Database")) );
   mpTabs->addTab( mpConfigCommunicationWidget, QString(tr("Communication")) );
   mpTabs->addTab( mpProxyWidget,               QString(tr("Proxy")) );
   mpTabs->addTab( welldone,                    QString(tr("Done")) );
   handleTabChange( 0 );
   for( i = 1; i < mpTabs->count(); i++ )
   {
      mpTabs->setTabEnabled( i, false );
   }
   
   mainLayout->addWidget( about );
   mainLayout->addWidget( mpTabs );
   mainLayout->addStretch();
   mainLayout->addWidget( mpHint );
   mainLayout->addWidget( mpNext );
   
   connect( mpTabs, SIGNAL(currentChanged(int)),
            this, SLOT(handleTabChange(int)) );
   connect( mpNext, SIGNAL(pressed()),
            this, SLOT(handleNextButton()) );
   connect( mpConfigCommunicationWidget, SIGNAL(fullNoCommunnicationClicked()),
            this, SLOT(unlockCommunication()) );
   connect( mpDatabaseWidget, SIGNAL(databaseOk()),
            this, SLOT(unlockDatabase()) );
   
   setLayout( mainLayout );
}


void MainWidget::handleTabChange( int newTab )
{
   switch( mLastTab )
   {
      case 2:
         mpConfigCommunicationWidget->writeSettings();
         break;
      case 3:
         mpProxyWidget->writeSettings();
         mProxyOk = true;
         break;
      default:
         /* nothing */
         break;
   }
   
   switch( newTab )
   {
      case 0:
         mpHint->setText( tr("\n\nA recommendation for each pannel will be displayed here.") );
         mpNext->setDisabled( false );
         break;
      case 1:
         mpHint->setText( tr("\nA database is needed for operation.\nPlease create one.") );
         mpNext->setDisabled( !mDatabaseOk );
         break;
      case 2:
         mpConfigCommunicationWidget->readSettings();
         mpHint->setText( tr("\n\"Full Communication\" is strongly recommended\nto take advantage of all SLART features.") );
         mpNext->setDisabled( !mCommunicationOk );
         break;
      case 3:
         mpProxyWidget->readSettings();
         mpHint->setText( tr("\nUsusally a proxy is not necessary for web access."
                             "\nIf unsure look at the proxy settings of your web browser.") );
         mpNext->setDisabled( false );
         break;
      case 4:
         mpHint->setText( tr("\n\nA recommendation for each pannel will be displayed here.") );
         mpNext->setDisabled( !mDatabaseOk || !mCommunicationOk || !mProxyOk );
         break;
   }
   
   if( newTab < (mpTabs->count() - 1) )
   {
      mpNext->setText( tr("Next") );
   }
   else
   {
      mpNext->setText( tr("Done") );
   }
   
   mLastTab = newTab;
}


void MainWidget::handleNextButton()
{
   if( mLastTab < (mpTabs->count() - 1) )
   {
      mpTabs->setTabEnabled( mLastTab + 1, true );
      mpTabs->setCurrentIndex( mLastTab + 1 );
   }
   else
   {
      QApplication::exit((mDatabaseOk ? 0x2 : 0) | (mCommunicationOk ? 0x4 : 0) | (mProxyOk ? 0x8 : 0));
   }
}


void MainWidget::unlockDatabase()
{
   mDatabaseOk = true;
   mpNext->setDisabled( false );
}


void MainWidget::unlockCommunication()
{
   mCommunicationOk = true;
   mpNext->setDisabled( false );
}
