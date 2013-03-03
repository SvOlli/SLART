/*
 * src/apps/Partyman/PartymanConfigDialog.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "PartymanConfigDialog.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <AboutWidget.hpp>
#include <Database.hpp>
#include <GlobalConfigWidget.hpp>
#include <PasswordChecker.hpp>
#include <Satellite.hpp>
#include <Settings.hpp>
#include <Version.hpp>
#include <WidgetShot.hpp>

/* local headers */


PartymanConfigDialog::PartymanConfigDialog( Database *database, QWidget *parent, Qt::WindowFlags flags )
: QDialog( parent, flags )
, mpDatabase( database )
, mpDerMixDhostLabel( new QLabel( tr("Hostname:") ) )
, mpDerMixDhost( new QLineEdit( this ) )
, mpDerMixDportLabel( new QLabel( tr("Port:") ) )
, mpDerMixDport( new QSpinBox( this ) )
, mpDerMixDlog( new QCheckBox( tr("Log communication to stdout"), this ) )
, mpDerMixDrun( new QCheckBox( tr("Start/stop via Partyman"), this ) )
, mpDerMixDcmdLabel( new QLabel( tr("Command:"), this ) )
, mpDerMixDcmd( new QLineEdit( this ) )
, mpDerMixDparamsLabel( new QLabel( tr("Parameters:"), this ) )
, mpDerMixDparams( new QLineEdit( this ) )
, mpAutoConnect( new QCheckBox( tr("Connect on startup"), this ) )
, mpCrossfadeTime( new QSpinBox( this ) )
, mpNormalizeMode( new QComboBox( this ) )
, mpNormalizeValue( new QDoubleSpinBox( this ) )
, mpLogCmd( new QLineEdit( this ) )
, mpCountSkip( new QCheckBox( tr("Increase track played counter on skip"), this ) )
, mpStopOnUnderpass( new QCheckBox( tr("Stop playing if Underpass is started"), this ) )
, mpTrayIcon( new QCheckBox( tr("Show tray icon"), this ) )
, mpTrayIconBubble( new QCheckBox( tr("With bubble"), this ) )
, mpTrayIconBubbleTime( new QDoubleSpinBox( this ) )
, mpPlayOnlyFavorite( new QCheckBox( tr("Play favorite tracks only"), this ) )
, mpPlayOnlyLeastPlayed( new QCheckBox( tr("Play least played tracks"), this ) )
, mpPlayGroup( new QComboBox( this ) )
, mpPlayNotAgainCount( new QSpinBox( this ) )
, mpNamePattern( new QLineEdit( this ) )
, mpPlayerPattern( new QLineEdit( this ) )
, mpListPattern( new QLineEdit( this ) )
, mpTrayIconPattern( new QLineEdit( this ) )
, mpUpdateBrowserButton( new QPushButton( tr("Update browse tab"), this ) )
, mpStartKioskButton( new QPushButton( tr("Start kiosk mode"), this ) )
, mpGlobalSettings( new GlobalConfigWidget( this ) )
{
   setWindowTitle( QApplication::applicationName() + ": " + tr("Settings") );
   setWindowIcon( QIcon(":/Partyman/Icon.png") );

   mpGlobalSettings->showClipboard();
   mpGlobalSettings->showAnimate();
   mpGlobalSettings->showDoubleClickInterval();

   QStringList modes;
   modes << tr("No Normalization")
         << tr("Normalize By Peak")
         << tr("Normalize By Power:");
   mpNormalizeMode->addItems( modes );

   mpNormalizeValue->setSingleStep( 0.01 );
   mpNormalizeValue->setRange( 0.0, 1.0 );
   mpNormalizeValue->setAlignment( Qt::AlignRight );
   mpNormalizeValue->setDisabled( true );
   mpTrayIconBubbleTime->setDecimals( 1 );
   mpTrayIconBubbleTime->setSuffix( "s" );
   mpTrayIconBubbleTime->setSingleStep( 1.0 );
   mpTrayIconBubbleTime->setRange( 1.0, 15.0 );
   mpTrayIconBubbleTime->setAlignment( Qt::AlignRight );
   mpDerMixDport->setRange( 1, 65535 );
   mpDerMixDport->setAlignment( Qt::AlignRight );
   mpCrossfadeTime->setRange( 1, 30 );
   mpCrossfadeTime->setSuffix( "s" );
   mpCrossfadeTime->setAlignment( Qt::AlignRight );
   mpPlayNotAgainCount->setRange( 0, 999 );
   mpPlayNotAgainCount->setAlignment( Qt::AlignRight );

   QWidget     *dermixdTab    = new QWidget( this );
   QGridLayout *dermixdLayout = new QGridLayout( dermixdTab );
   dermixdLayout->addWidget( mpDerMixDrun, 0, 0, 1, 4 );
   dermixdLayout->addWidget( mpDerMixDcmdLabel, 1, 0 );
   dermixdLayout->addWidget( mpDerMixDcmd, 1, 1, 1, 3 );
   dermixdLayout->addWidget( mpDerMixDparamsLabel, 2, 0 );
   dermixdLayout->addWidget( mpDerMixDparams, 2, 1, 1, 3 );
   dermixdLayout->addWidget( mpDerMixDhostLabel, 3, 0 );
   dermixdLayout->addWidget( mpDerMixDhost, 3, 1, 1, 3 );
   dermixdLayout->addWidget( mpDerMixDportLabel, 4, 0 );
   dermixdLayout->addWidget( mpDerMixDport, 4, 1 );
   dermixdLayout->addWidget( mpDerMixDlog, 4, 2, 1, 2 );
   dermixdLayout->addWidget( mpNormalizeMode, 5, 0, 1, 3 );
   dermixdLayout->addWidget( mpNormalizeValue, 5, 3 );
   dermixdLayout->setColumnStretch( 2, 2 );
   dermixdLayout->setColumnStretch( 3, 1 );
   dermixdLayout->setRowStretch( 6, 1 );
   dermixdTab->setLayout( dermixdLayout );
   connect( mpNormalizeMode, SIGNAL(currentIndexChanged(int)),
            this, SLOT(handleNormalizeMode(int)) );
   connect( mpDerMixDrun, SIGNAL(clicked(bool)),
            this, SLOT(handleDerMixDrun(bool)) );

   QWidget     *partymanTab    = new QWidget( this );
   QGridLayout *partymanLayout = new QGridLayout( partymanTab );
   partymanLayout->addWidget( mpAutoConnect,                        0, 0, 1, 5 );
   partymanLayout->addWidget( new QLabel( tr("Crossfade time:") ),  1, 0, 1, 2 );
   partymanLayout->addWidget( mpCrossfadeTime,                      1, 4 );
   partymanLayout->addWidget( mpTrayIcon,                           2, 0, 1, 2 );
   partymanLayout->addWidget( mpTrayIconBubble,                     2, 2, 1, 2 );
   partymanLayout->addWidget( mpTrayIconBubbleTime,                 2, 4 );
   partymanLayout->addWidget( new QLabel( tr("External logger:") ), 3, 0 );
   partymanLayout->addWidget( mpLogCmd,                             3, 1, 1, 4 );
   partymanLayout->addWidget( mpCountSkip,                          4, 0, 1, 5 );
   partymanLayout->addWidget( mpStopOnUnderpass,                    5, 0, 1, 5 );
   partymanLayout->setColumnStretch( 1, 1 );
   partymanLayout->setColumnStretch( 2, 1 );
   partymanLayout->setRowStretch( 7, 1 );
   partymanTab->setLayout( partymanLayout );
   connect( mpTrayIcon, SIGNAL(clicked(bool)),
            this, SLOT(handleShowTrayIcon(bool)) );

   QWidget     *randomTab    = new QWidget( this );
   QGridLayout *randomLayout = new QGridLayout( randomTab );
   randomLayout->addWidget( mpPlayOnlyFavorite, 0, 0, 1, 3 );
   randomLayout->addWidget( mpPlayOnlyLeastPlayed, 1, 0, 1, 3 );
   randomLayout->addWidget( new QLabel( tr("Play group:") ), 2, 0 );
   randomLayout->addWidget( mpPlayGroup, 2, 1, 1, 2 );
   randomLayout->addWidget( new QLabel( tr("Number of tracks before an artist is played again:") ), 3, 0, 1, 2 );
   randomLayout->addWidget( mpPlayNotAgainCount, 3, 2 );
   randomLayout->setRowStretch( 6, 1 );
   randomTab->setLayout( randomLayout );

   QWidget     *displayTab          = new QWidget( this );
   QGridLayout *displayLayout       = new QGridLayout( displayTab );
   QBoxLayout  *displayButtonLayout = new QHBoxLayout;
   displayLayout->addWidget( new QLabel( tr("Display patterns:") ), 0, 0, 1, 2 );
   displayLayout->addWidget( new QLabel( tr("Title:") ), 1, 0 );
   displayLayout->addWidget( mpNamePattern, 1, 1 );
   displayLayout->addWidget( new QLabel( tr("Tray icon:") ), 2, 0 );
   displayLayout->addWidget( mpTrayIconPattern, 2, 1 );
   displayLayout->addWidget( new QLabel( tr("Player:") ), 3, 0 );
   displayLayout->addWidget( mpPlayerPattern, 3, 1 );
   displayLayout->addWidget( new QLabel( tr("List:") ), 4, 0 );
   displayLayout->addWidget( mpListPattern, 4, 1 );
   displayLayout->setRowStretch( 6, 1 );
   displayButtonLayout->addWidget( mpUpdateBrowserButton );
   displayButtonLayout->addWidget( mpStartKioskButton );
   displayLayout->addLayout( displayButtonLayout, 7, 0, 1, 2 );
   displayTab->setLayout( displayLayout );

   QPushButton *okButton     = new QPushButton( tr("OK"), this );
   QPushButton *cancelButton = new QPushButton( tr("Cancel"), this );

   QHBoxLayout *buttonLayout = new QHBoxLayout;
   buttonLayout->addWidget( okButton );
   buttonLayout->addWidget( cancelButton );

   QBoxLayout *mainLayout;
   AboutWidget *about  = new AboutWidget( this,
   tr("<table align='center'><tr><td align='center'>Our hail to the new king:</td></tr>"
   "<tr><td align='center'><a href='http://svolli.org/software/partyman/'>"
   "<img src=':/Partyman/Icon.png'></a>&nbsp;&nbsp;<a href='http://svolli.org/software/partyman/'>"
   "<img src=':/Partyman/Writing.png'></a></td></tr><tr><td align='center'>Version " SLART_VERSION
   " written by Sven Oliver Moll as a part of <a href='http://svolli.org/software/slart/'>SLART</a>."
   "</td></tr><tr><td align='left'>Distributed unter the terms of the "
   "<a href='http://www.gnu.org/licenses/gpl.html'>GPL</a>.</td></tr><tr><td align='center'>"
   "This is a frontend for <a href='http://dermixd.de/'>DerMixD</a>.</td></tr>"
   "<tr><td align='right'>Based upon the <a href='about:qt'>Qt</a> framework.</td></tr></table>") );

   mainLayout = new QVBoxLayout( this );
   QTabWidget *tabs = new QTabWidget( this );
   tabs->addTab( dermixdTab,       tr("DerMixD") );
   tabs->addTab( partymanTab,      tr("Partyman") );
   tabs->addTab( randomTab,        tr("Random") );
   tabs->addTab( displayTab,       tr("Display") );
   tabs->addTab( mpGlobalSettings, tr("Global") );

   mainLayout->addWidget( about );
   mainLayout->addWidget( tabs );
   mainLayout->addLayout( buttonLayout );

   setLayout( mainLayout );

   connect( okButton, SIGNAL(clicked()),
            this, SLOT(accept()) );
   connect( cancelButton, SIGNAL(clicked()),
            this, SLOT(reject()) );
   connect( this, SIGNAL(accepted()),
            this, SLOT(writeSettings()) );
   connect( this, SIGNAL(rejected()),
            this, SLOT(readSettings()) );
   connect( mpUpdateBrowserButton, SIGNAL(clicked()),
            this, SIGNAL(updateBrowser()) );
   connect( mpStartKioskButton, SIGNAL(clicked()),
            this, SLOT(handleStartKiosk()) );

   PasswordChecker::get()->setLockMessage(
         tr("Enter Password For Enabling Kiosk Mode"),
         tr("To enable Kiosk Mode enter a password that will be needed for unlocking.\n"
               "(Empty password does not activate Kiosk Mode)")
   );
   PasswordChecker::get()->setUnlockMessage(
         tr("Enter Password For Unlocking Kiosk Mode"),
         tr("The feature you've requested is not available in Kiosk Mode, "
               "enter password to disable.")
   );

   readSettings();

   WidgetShot::addWidget( "PartymanConfigDialog", this );
}


PartymanConfigDialog::~PartymanConfigDialog()
{
}


void PartymanConfigDialog::exec()
{
   if( !(PasswordChecker::get()->unlock()) )
   {
      return;
   }
   readSettings();
   QDialog::exec();
}


void PartymanConfigDialog::readSettings()
{
   mpPlayGroup->clear();
   mpPlayGroup->addItem( tr("| All |") );
   mpPlayGroup->addItems( mpDatabase->getGroups() );

   mpDerMixDhost->setText( Settings::value( Settings::PartymanDerMixDhost ) );
   mpDerMixDport->setValue( Settings::value( Settings::PartymanDerMixDport ) );
   mpDerMixDlog->setChecked( Settings::value( Settings::PartymanDerMixDlog ) );
   mpDerMixDrun->setChecked( Settings::value( Settings::PartymanDerMixDrun ) );
   mpDerMixDcmd->setText( Settings::value( Settings::PartymanDerMixDcmd ) );
   mpDerMixDparams->setText( Settings::value( Settings::PartymanDerMixDparams ) );
   mpAutoConnect->setChecked( Settings::value( Settings::PartymanAutoConnect ) );
   mpCrossfadeTime->setValue( Settings::value( Settings::PartymanCrossfadeTime ) );
   mpNormalizeMode->setCurrentIndex( Settings::value( Settings::PartymanNormalizeMode ) );
   mpNormalizeValue->setValue( Settings::value( Settings::PartymanNormalizeValue ) );
   mpLogCmd->setText( Settings::value( Settings::PartymanLogCmd ) );
   mpCountSkip->setChecked( Settings::value( Settings::PartymanCountSkip ) );
   mpStopOnUnderpass->setChecked( Settings::value( Settings::PartymanStopOnUnderpassStart ));
   mpTrayIcon->setChecked( Settings::value( Settings::PartymanTrayIcon ) );
   mpTrayIconBubble->setChecked( Settings::value( Settings::PartymanTrayIconBubble ) );
   mpTrayIconBubbleTime->setValue( Settings::value( Settings::PartymanTrayIconBubbleTime ) );
   mpPlayOnlyFavorite->setChecked( Settings::value( Settings::PartymanPlayOnlyFavorite ) );
   mpPlayOnlyLeastPlayed->setChecked( Settings::value( Settings::PartymanPlayOnlyLeastPlayed ) );
   mpPlayNotAgainCount->setValue(Settings::value( Settings::PartymanPlayNotAgainCount ) );
   mpPlayerPattern->setText( Settings::value( Settings::PartymanPlayerPattern ) );
   mpListPattern->setText( Settings::value( Settings::PartymanListPattern ) );
   mpNamePattern->setText( Settings::value( Settings::PartymanNamePattern ) );
   mpTrayIconPattern->setText( Settings::value( Settings::PartymanTrayIconPattern ) );
   handleDerMixDrun( mpDerMixDrun->isChecked() );
   mpGlobalSettings->readSettings();

   QString playGroup( Settings::value( Settings::PartymanPlayGroup ) );
   if( !playGroup.isEmpty() )
   {
      int i = mpPlayGroup->findText( playGroup );
      if( i > 0 )
      {
         mpPlayGroup->setCurrentIndex( i );
      }
   }
   handleShowTrayIcon( mpTrayIcon->isChecked() );

   emit configChanged();
}


void PartymanConfigDialog::writeSettings()
{
   Settings::setValue( Settings::PartymanDerMixDhost, mpDerMixDhost->text() );
   Settings::setValue( Settings::PartymanDerMixDport, mpDerMixDport->value() );
   Settings::setValue( Settings::PartymanDerMixDlog, mpDerMixDlog->isChecked() );
   Settings::setValue( Settings::PartymanDerMixDrun, mpDerMixDrun->isChecked() );
   Settings::setValue( Settings::PartymanDerMixDcmd, mpDerMixDcmd->text() );
   Settings::setValue( Settings::PartymanDerMixDparams, mpDerMixDparams->text() );
   Settings::setValue( Settings::PartymanAutoConnect, mpAutoConnect->isChecked() );
   Settings::setValue( Settings::PartymanCrossfadeTime, mpCrossfadeTime->value() );
   Settings::setValue( Settings::PartymanNormalizeMode, mpNormalizeMode->currentIndex() );
   Settings::setValue( Settings::PartymanNormalizeValue, mpNormalizeValue->value() );
   Settings::setValue( Settings::PartymanLogCmd, mpLogCmd->text() );
   Settings::setValue( Settings::PartymanCountSkip, mpCountSkip->isChecked() );
   Settings::setValue( Settings::PartymanStopOnUnderpassStart, mpStopOnUnderpass->isChecked() );
   Settings::setValue( Settings::PartymanTrayIcon, mpTrayIcon->isChecked() );
   Settings::setValue( Settings::PartymanTrayIconBubble, mpTrayIconBubble->isChecked() );
   Settings::setValue( Settings::PartymanTrayIconBubbleTime, mpTrayIconBubbleTime->value() );
   Settings::setValue( Settings::PartymanPlayOnlyFavorite, mpPlayOnlyFavorite->isChecked() );
   Settings::setValue( Settings::PartymanPlayOnlyLeastPlayed, mpPlayOnlyLeastPlayed->isChecked() );
   Settings::setValue( Settings::PartymanPlayNotAgainCount, mpPlayNotAgainCount->value() );
   Settings::setValue( Settings::PartymanPlayerPattern, mpPlayerPattern->text() );
   Settings::setValue( Settings::PartymanListPattern, mpListPattern->text() );
   Settings::setValue( Settings::PartymanNamePattern, mpNamePattern->text() );
   Settings::setValue( Settings::PartymanTrayIconPattern, mpTrayIconPattern->text() );
   Settings::setValue( Settings::PartymanPlayGroup, mpPlayGroup->currentIndex() ?
                                                     mpPlayGroup->currentText() :
                                                     QString() );
   mpGlobalSettings->writeSettings();
   if( Satellite::get() )
   {
      //! \todo check if useful
      Satellite::get()->send( "p0c" );
   }

   emit configChanged();
}


void PartymanConfigDialog::handleDerMixDrun( bool checked )
{
   mpDerMixDcmdLabel->setDisabled( !checked );
   mpDerMixDcmd->setDisabled( !checked );
   mpDerMixDparamsLabel->setDisabled( !checked );
   mpDerMixDparams->setDisabled( !checked );
   mpDerMixDhostLabel->setDisabled( checked );
   mpDerMixDhost->setDisabled( checked );
}


void PartymanConfigDialog::handleNormalizeMode( int mode )
{
   mpNormalizeValue->setDisabled( mode != 2 );
}


void PartymanConfigDialog::handleShowTrayIcon( bool checked )
{
   if ( !QSystemTrayIcon::isSystemTrayAvailable() )
   {
      checked = false;
      mpTrayIcon->setDisabled( !checked );
   }

   mpTrayIconBubble->setDisabled( !checked );
   mpTrayIconBubbleTime->setDisabled( !checked );
}


void PartymanConfigDialog::handleStartKiosk()
{
   if( PasswordChecker::get( this )->lock() )
   {
      accept();
   }
}
