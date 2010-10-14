/**
 * src/libs/Sorcerer/SorcererWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "SorcererWidget.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <AboutWidget.hpp>
#include <Database.hpp>
#include <ProxyWidget.hpp>
#include <MySettings.hpp>
#include "../../apps/Innuendo/SatelliteConfigWidget.hpp"
#include "../../apps/Rubberbandman/DatabaseWidget.hpp"

/* local headers */


SorcererWidget::SorcererWidget( QWidget *parent , Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpDatabase( new Database() )
, mpTabs( new QTabWidget( this ) )
, mpHint( new QLabel( this ) )
, mpNext( new QPushButton( tr("Next"), this ) )
, mpDatabaseWidget( new DatabaseWidget( mpDatabase, this ) )
, mpSatelliteConfigWidget( new SatelliteConfigWidget( this ) )
, mpProxyWidget( new ProxyWidget( this ) )
, mLastTab( 0 )
, mDatabaseOk( false )
, mCommunicationOk( MySettings( "Global" ).value( "SatellitePort" ).isValid() )
, mProxyOk( MySettings( "Global/HTTPProxy" ).value( "Enable" ).isValid() )
, mQuit( tr("quit") )
, mStart( tr("start") )
{
   int i;
   unlockDatabase();
   
   QVBoxLayout *mainLayout = new QVBoxLayout( this );
   mainLayout->setContentsMargins( 3, 3, 3, 3 );
   parent->setWindowIcon( QIcon( ":/SLART.png" ) );
   
   AboutWidget *about = new AboutWidget( this );
   QLabel *welcome    = new QLabel( tr("Hello and welcome to SLART<hr>"
                                       "Before %1 can be started a little bit of setup needs to be done.<br><br>"
                                       "This little wizard will help you on this task. There's no need to worry,<br>"
                                       "anything you configure here can be configured in the appropriate<br>"
                                       "SLART application as well.").arg( QApplication::applicationName() ), this );
   QLabel *welldone   = new QLabel( tr("Well, that's all. Wasn't so hard, was it?<br><br>"
                                       "Now, press 'Done' to %1 %2.").arg(
                                             QApplication::applicationName() == "Sorcerer" ? mQuit : mStart,
                                             QApplication::applicationName() ), this );
   welcome->setAlignment( Qt::AlignCenter );
   welldone->setAlignment( Qt::AlignCenter );
   mpHint->setAlignment( Qt::AlignCenter );
   mpHint->setFrameShadow( QFrame::Raised );
   mpHint->setFrameShape( QFrame::Box );
   
   mpTabs->addTab( welcome,                 QString(tr("Welcome")) );
   mpTabs->addTab( mpDatabaseWidget,        QString(tr("Database")) );
   mpTabs->addTab( mpSatelliteConfigWidget, QString(tr("Communication")) );
   mpTabs->addTab( mpProxyWidget,           QString(tr("Proxy")) );
   mpTabs->addTab( welldone,                QString(tr("Done")) );
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
   
   setLayout( mainLayout );
}


SorcererWidget::~SorcererWidget()
{
   delete mpDatabase;
}


int SorcererWidget::errors()
{
   return (mDatabaseOk ? 0 : 0x2) | (mCommunicationOk ? 0 : 0x4) | (mProxyOk ? 0 : 0x8);
}


void SorcererWidget::handleTabChange( int newTab )
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
         mpHint->setText( tr("\n\nPress 'Done' now to %1 %2.")
                          .arg( QApplication::applicationName() == "Sorcerer" ? mQuit : mStart,
                                QApplication::applicationName() ) );
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


void SorcererWidget::handleNextButton()
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


void SorcererWidget::unlockDatabase()
{
   if( Database::exists() )
   {
      mDatabaseOk = mpDatabase->getTrackInfoList( 0 ) > 2;
   }
   if( mDatabaseOk )
   {
      mpNext->setDisabled( false );
   }
}


void SorcererWidget::unlockCommunication()
{
   mCommunicationOk = true;
   mpNext->setDisabled( false );
}
