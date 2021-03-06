/*
 * src/apps/Sorcerer/SorcererWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "SorcererMainWidget.hpp"

/* system headers */

/* Qt headers */
#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

/* local library headers */
#include <AboutWidget.hpp>
#include <DatabaseInterface.hpp>
#include <ProxyWidget.hpp>
#include <Settings.hpp>
#include <WindowIconChanger.hpp>
#include "../../apps/Innuendo/SatelliteConfigWidget.hpp"
#include "../../apps/Rubberbandman/DatabaseWidget.hpp"

/* local headers */


SorcererMainWidget::SorcererMainWidget( QWidget *parent, Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpDatabase( DatabaseInterface::get() )
, mpTabs( new QTabWidget( this ) )
, mpHint( new QLabel( this ) )
, mpNext( new QPushButton( tr("Next"), this ) )
, mpDatabaseWidget( new DatabaseWidget( this ) )
, mpSatelliteConfigWidget( new SatelliteConfigWidget( this ) )
, mpProxyWidget( new ProxyWidget( this ) )
, mLastTab( 0 )
, mDatabaseOk( false )
, mCommunicationOk( Settings::get( "Global" )->value( "SatellitePort" ).isValid() )
, mProxyOk( Settings::get( "Global" )->value( "HTTPProxy/Enable" ).isValid() )
, mQuit( tr("quit") )
, mStart( tr("start") )
{
   int i;
   unlockDatabase();

   QVBoxLayout *mainLayout = new QVBoxLayout( this );
   mainLayout->setContentsMargins( 3, 3, 3, 3 );
   parent->setWindowIcon( QIcon( ":/Sorcerer/Icon.png" ) );

   AboutWidget *about = new AboutWidget( this );
   QLabel *welcome    = new QLabel( tr("Hello and welcome to SLART<br>"
                                       "<img src=':/SLART/Icon.png'><hr>"
                                       "Before %1 can be started a little bit of setup needs to be done.<br><br>"
                                       "This little wizard will help you on this task. There's no need to worry,<br>"
                                       "anything you configure here can be configured in the appropriate<br>"
                                       "SLART application as well.").arg( QApplication::applicationName() ), this );
   QLabel *welldone = 0;
   if( QApplication::arguments().size() > 1 )
   {
      parent->setWindowTitle( tr("%1 (running for %2)").arg(
                                 QApplication::applicationName(),
                                 QApplication::arguments().at(1) ) );
      welldone = new QLabel( tr("Well, that's all. Wasn't so hard, was it?<br><br>"
                                "Now, press 'Done' to start %1.").arg(
                                QApplication::arguments().at(1) ) );
   }
   else
   {
      welldone = new QLabel( tr("Now, press 'Done' to quit Sorcerer.") );
   }
   welcome->setAlignment( Qt::AlignCenter );
   welldone->setAlignment( Qt::AlignCenter );
   mpHint->setAlignment( Qt::AlignCenter );
   mpHint->setFrameShadow( QFrame::Raised );
   mpHint->setFrameShape( QFrame::Box );

   mpTabs->addTab( welcome,                 QIcon( ":/Sorcerer/Icon.png" ),      tr("Welcome") );
   mpTabs->addTab( mpDatabaseWidget,        QIcon( ":/Rubberbandman/Icon.png" ), tr("Database") );
   mpTabs->addTab( mpSatelliteConfigWidget, QIcon( ":/Innuendo/Icon.png" ),      tr("Communication") );
   mpTabs->addTab( mpProxyWidget,           QIcon( ":/SLART/Icon.png" ),         tr("Proxy") );
   mpTabs->addTab( welldone,                QIcon( ":/Sorcerer/Icon.png" ),      tr("Done") );
   for( i = 0; i < mpTabs->count(); i++ )
   {
      mpTabs->setTabEnabled( i, (i==0) );
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
   connect( mpSatelliteConfigWidget, SIGNAL(fullOrNoCommunication()),
            this, SLOT(unlockCommunication()) );
   connect( mpDatabaseWidget, SIGNAL(databaseUpdated()),
            this, SLOT(unlockDatabase()) );
   handleTabChange(0);
   WindowIconChanger *wic = new WindowIconChanger( parent, QIcon(":/Common/DatabaseUp.png"), this );
   mpDatabase->connectActivityIndicator( wic, SLOT(changed(bool)) );

   setLayout( mainLayout );
}


SorcererMainWidget::~SorcererMainWidget()
{
}


int SorcererMainWidget::errors()
{
   return (mDatabaseOk ? 0 : 0x2) | (mCommunicationOk ? 0 : 0x4) | (mProxyOk ? 0 : 0x8);
}


void SorcererMainWidget::handleTabChange( int newTab )
{
   switch( mLastTab )
   {
      case 2:
         mpSatelliteConfigWidget->writeSettings();
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
         mpHint->setText( tr("A recommendation for each pannel will be displayed here.\n\n"
                             "Right now, just press the 'Next'-Button below.") );
         mpNext->setDisabled( false );
         break;
      case 1:
         mpHint->setText( tr("A database is needed for operation. Please create one by selecting the\n"
                             "directory containing the music by pressing '...' and then press 'Update' to\n"
                             "create the database. This might take some time. Press 'Next' once it's done.") );
         mpNext->setDisabled( !mDatabaseOk );
         break;
      case 2:
         mpSatelliteConfigWidget->readSettings();
         mpHint->setText( tr("'Full Communication' is strongly recommended\nto take advantage of all SLART features.\n"
                             "So go ahead and press 'Full Communication' and 'Next'.") );
         mpNext->setDisabled( !mCommunicationOk );
         break;
      case 3:
         mpProxyWidget->readSettings();
         mpHint->setText( tr("Ususally a proxy is not necessary for web access.\n"
                             "If unsure look at the proxy settings of your web browser.\n"
                             "Once you've set the proxy (or decided not to) press 'Next'.") );
         mpNext->setDisabled( false );
         break;
      case 4:
         if( QApplication::arguments().size() > 1 )
         {
            mpHint->setText( tr("\n\nNow, press 'Done' to start %1.").arg( QApplication::arguments().at(1) ) );
         }
         else
         {
            mpHint->setText( tr("\n\nNow, press 'Done' to quit.") );
         }
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


void SorcererMainWidget::handleNextButton()
{
   if( mLastTab < (mpTabs->count() - 1) )
   {
      mpTabs->setTabEnabled( mLastTab + 1, true );
      mpTabs->setCurrentIndex( mLastTab + 1 );
   }
   else
   {
      QApplication::quit();
   }
}


void SorcererMainWidget::unlockDatabase()
{
   if( DatabaseInterface::exists() )
   {
      mpDatabase->getTrackInfoList( this, SLOT(countTracks(TrackInfoList)) );
   }
}

void SorcererMainWidget::countTracks( const TrackInfoList &list )
{
   mDatabaseOk = list.count() > 2;

   if( mDatabaseOk )
   {
      mpNext->setDisabled( false );
   }
}


void SorcererMainWidget::unlockCommunication()
{
   mCommunicationOk = true;
   mpNext->setDisabled( false );
}
