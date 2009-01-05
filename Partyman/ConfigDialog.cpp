/**
 * ConfigDialog.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "ConfigDialog.hpp"
#include "Database.hpp"
#include "GlobalConfigWidget.hpp"
#include "MySettings.hpp"
#include "AboutWidget.hpp"
#include "Version.hpp"

#include <QtGui>


ConfigDialog::ConfigDialog( Database *database, QWidget *parent, Qt::WindowFlags flags )
: QDialog( parent, flags )
, mpDatabase( database )
, mpDerMixDhostLabel( new QLabel( tr("Hostname:") ) )
, mpDerMixDhost( new QLineEdit( this ) )
, mpDerMixDportLabel( new QLabel( tr("Port:") ) )
, mpDerMixDport( new QSpinBox( this ) )
, mpDerMixDlog( new QCheckBox( tr("Log Communication To stdout"), this ) )
, mpDerMixDrun( new QCheckBox( tr("Start/Stop via Partyman"), this ) )
, mpDerMixDcmdLabel( new QLabel( tr("Command:"), this ) )
, mpDerMixDcmd( new QLineEdit( this ) )
, mpDerMixDparamsLabel( new QLabel( tr("Parameters:"), this ) )
, mpDerMixDparams( new QLineEdit( this ) )
, mpAutoConnect( new QCheckBox( tr("Connect On Startup"), this ) )
, mpCrossfadeTime( new QSpinBox( this ) )
, mpSLARTCommunication( new QCheckBox( tr("Use SLART UDP Communication"), this ) )
, mpUDPListenerPort( new QSpinBox( this ) )
, mpNormalizeMode( new QComboBox( this ) )
, mpNormalizeValue( new QDoubleSpinBox( this ) )
, mpLogCmd( new QLineEdit( this ) )
, mpCountSkip( new QCheckBox( tr("Increase Track Played Counter On Skip"), this ) )
, mpTrayIcon( new QCheckBox( tr("Show Tray Icon"), this ) )
, mpTrayIconBubble( new QCheckBox( tr("With Bubble"), this ) )
, mpTrayIconBubbleTime( new QDoubleSpinBox( this ) )
, mpPlayOnlyFavorite( new QCheckBox( tr("Play Favorite Tracks Only"), this ) )
, mpPlayOnlyLeastPlayed( new QCheckBox( tr("Play Least Played Tracks Only"), this ) )
, mpPlayFolder( new QComboBox( this ) )
, mpPlayNotAgainCount( new QSpinBox( this ) )
, mpNamePattern( new QLineEdit( this ) )
, mpPlayerPattern( new QLineEdit( this ) )
, mpListPattern( new QLineEdit( this ) )
, mpTrayIconPattern( new QLineEdit( this ) )
, mpUpdateBrowserButton( new QPushButton( tr("Update Browser Tab"), this ) )
, mpGlobalSettings( new GlobalConfigWidget( this ) )
{
   setWindowTitle( QApplication::applicationName()+tr(" Settings") );
   setWindowIcon( QIcon(":/PartymanSmile.gif") );
   
   mpGlobalSettings->showClipboard();
   mpGlobalSettings->showAnimate();
   
   QStringList modes;
   modes << tr("No Normalization") 
         << tr("Normalize By Peak") 
         << tr("Normalize By Power:");
   mpNormalizeMode->addItems( modes );
   
   mpNormalizeValue->setSingleStep( 0.01 );
   mpNormalizeValue->setRange( 0.0, 1.0 );
   mpTrayIconBubbleTime->setDecimals( 1 );
   mpTrayIconBubbleTime->setSuffix( "s" );
   mpTrayIconBubbleTime->setSingleStep( 0.1 );
   mpTrayIconBubbleTime->setRange( 1.0, 15.0 );
   mpUDPListenerPort->setRange( 1, 65535 );
   mpDerMixDport->setRange( 1, 65535 );
   mpCrossfadeTime->setRange( 1, 30 );
   mpCrossfadeTime->setSuffix( "s" );
   mpPlayNotAgainCount->setRange( 0, 999 );
   
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
   connect( mpSLARTCommunication, SIGNAL(clicked(bool)),
            this, SLOT(handleUDPListen(bool)) );
   
   QWidget     *partymanTab    = new QWidget( this );
   QGridLayout *partymanLayout = new QGridLayout( partymanTab );
   partymanLayout->addWidget( mpAutoConnect,                        0, 0, 1, 5 );
   partymanLayout->addWidget( new QLabel( tr("Crossfade Time:") ),  1, 0, 1, 2 );
   partymanLayout->addWidget( mpCrossfadeTime,                      1, 4 );
   partymanLayout->addWidget( mpSLARTCommunication,                 2, 0, 1, 3 );
   partymanLayout->addWidget( new QLabel( tr("Port") ),             2, 3 );
   partymanLayout->addWidget( mpUDPListenerPort,                    2, 4 );
   partymanLayout->addWidget( mpTrayIcon,                           3, 0, 1, 2 );
   partymanLayout->addWidget( mpTrayIconBubble,                     3, 2, 1, 2 );
   partymanLayout->addWidget( mpTrayIconBubbleTime,                 3, 4 );
   partymanLayout->addWidget( new QLabel( tr("External Logger:") ), 4, 0 );
   partymanLayout->addWidget( mpLogCmd,                             4, 1, 1, 4 );
   partymanLayout->addWidget( mpCountSkip,                          5, 0, 1, 5 );
   partymanLayout->setColumnStretch( 1, 1 );
   partymanLayout->setColumnStretch( 2, 1 );
   partymanLayout->setRowStretch( 5, 1 );
   partymanTab->setLayout( partymanLayout );
   connect( mpTrayIcon, SIGNAL(clicked(bool)),
            this, SLOT(handleShowTrayIcon(bool)) );
   
   QWidget     *randomTab    = new QWidget( this );
   QGridLayout *randomLayout = new QGridLayout( randomTab );
   randomLayout->addWidget( mpPlayOnlyFavorite, 0, 0, 1, 3 );
   randomLayout->addWidget( mpPlayOnlyLeastPlayed, 1, 0, 1, 3 );
   randomLayout->addWidget( new QLabel( tr("Play Folder:") ), 2, 0 );
   randomLayout->addWidget( mpPlayFolder, 2, 1, 1, 2 );
   randomLayout->addWidget( new QLabel( tr("Number Of Tracks An Artist Is Not Played Again:") ), 3, 0, 1, 2 );
   randomLayout->addWidget( mpPlayNotAgainCount, 3, 2 );
   randomLayout->setRowStretch( 6, 1 );
   randomTab->setLayout( randomLayout );
   
   QWidget     *displayTab    = new QWidget( this );
   QGridLayout *displayLayout = new QGridLayout( displayTab );
   displayLayout->addWidget( new QLabel( tr("Display Patterns:") ), 0, 0, 1, 2 );
   displayLayout->addWidget( new QLabel( tr("Title:") ), 1, 0 );
   displayLayout->addWidget( mpNamePattern, 1, 1 );
   displayLayout->addWidget( new QLabel( tr("Tray Icon:") ), 2, 0 );
   displayLayout->addWidget( mpTrayIconPattern, 2, 1 );
   displayLayout->addWidget( new QLabel( tr("Player:") ), 3, 0 );
   displayLayout->addWidget( mpPlayerPattern, 3, 1 );
   displayLayout->addWidget( new QLabel( tr("List:") ), 4, 0 );
   displayLayout->addWidget( mpListPattern, 4, 1 );
   displayLayout->setRowStretch( 5, 1 );
   displayLayout->addWidget( mpUpdateBrowserButton, 6, 0, 1, 2 );
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
   "<img src=':/PartymanSmile.gif'></a>&nbsp;&nbsp;<a href='http://svolli.org/software/partyman/'>"
   "<img src=':/PartymanWriting.gif'></a></td></tr><tr><td align='center'>Version " SLART_VERSION
   " written by Sven Oliver Moll as a part of <a href='http://svolli.org/software/slart/'>SLART</a>."
   "</td></tr><tr><td align='left'>Distributed unter the terms of the "
   "<a href='http://www.gnu.org/licenses/gpl.html'>GPL</a>.</td></tr><tr><td align='center'>"
   "This is a frontend for <a href='http://dermixd.de/'>DerMixD</a>.</td></tr>"
   "<tr><td align='right'>Based upon the <a href='about:qt'>Qt</a> framework.</td></tr></table>") );
   
   mainLayout = new QVBoxLayout( this );
   QTabWidget *tabs = new QTabWidget( this );
   tabs->addTab( dermixdTab,       QString(tr("DerMixD")) );
   tabs->addTab( partymanTab,      QString(tr("Partyman")) );
   tabs->addTab( randomTab,        QString(tr("Random")) );
   tabs->addTab( displayTab,       QString(tr("Display")) );
   tabs->addTab( mpGlobalSettings, QString(tr("Global")) );
   
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
   
   readSettings();
}


void ConfigDialog::exec()
{
   readSettings();
   QDialog::exec();
}


void ConfigDialog::readSettings()
{
   MySettings settings;
   
   mpPlayFolder->clear();
   mpPlayFolder->addItem( tr("| All |") );
   mpPlayFolder->addItems( mpDatabase->getFolders() );
   
   mpDerMixDhost->setText( settings.VALUE_DERMIXDHOST );
   mpDerMixDport->setValue( settings.VALUE_DERMIXDPORT );
   mpDerMixDlog->setChecked( settings.VALUE_DERMIXDLOG );
   mpDerMixDrun->setChecked( settings.VALUE_DERMIXDRUN );
   mpDerMixDcmd->setText( settings.VALUE_DERMIXDCMD );
   mpDerMixDparams->setText( settings.VALUE_DERMIXDPARAMS );
   mpAutoConnect->setChecked( settings.VALUE_AUTOCONNECT );
   mpCrossfadeTime->setValue( settings.VALUE_CROSSFADETIME );
   mpSLARTCommunication->setChecked( settings.VALUE_SLARTCOMMUNICATION );
   mpUDPListenerPort->setValue(settings.VALUE_UDPLISTENERPORT );
   mpNormalizeMode->setCurrentIndex( settings.VALUE_NORMALIZEMODE );
   mpNormalizeValue->setValue( settings.VALUE_NORMALIZEVALUE );
   mpLogCmd->setText( settings.VALUE_LOGCMD );
   mpCountSkip->setChecked( settings.VALUE_COUNTSKIP );
   mpTrayIcon->setChecked( settings.VALUE_TRAYICON );
   mpTrayIconBubble->setChecked( settings.VALUE_TRAYICONBUBBLE );
   mpTrayIconBubbleTime->setValue( settings.VALUE_TRAYICONBUBBLETIME );
   mpPlayOnlyFavorite->setChecked( settings.VALUE_PLAYONLYFAVORITE );
   mpPlayOnlyLeastPlayed->setChecked( settings.VALUE_PLAYONLYLEASTPLAYED );
   mpPlayNotAgainCount->setValue(settings.VALUE_PLAYNOTAGAINCOUNT );
   mpPlayerPattern->setText( settings.VALUE_PLAYERPATTERN );
   mpListPattern->setText( settings.VALUE_LISTPATTERN );
   mpNamePattern->setText( settings.VALUE_NAMEPATTERN );
   mpTrayIconPattern->setText( settings.VALUE_TRAYICONPATTERN );
   handleUDPListen( mpSLARTCommunication->isChecked() );
   handleDerMixDrun( mpDerMixDrun->isChecked() );
   mpGlobalSettings->readSettings();
   
   QString playFolder( settings.VALUE_PLAYFOLDER );
   if( !playFolder.isEmpty() )
   {
      int i = mpPlayFolder->findText( playFolder );
      if( i > 0 )
      {
         mpPlayFolder->setCurrentIndex( i );
      }
   }
   
   emit configChanged();
}


void ConfigDialog::writeSettings()
{
   MySettings settings;

   settings.setValue( "DerMixDhost", mpDerMixDhost->text() );
   settings.setValue( "DerMixDport", mpDerMixDport->value() );
   settings.setValue( "DerMixDlog", mpDerMixDlog->isChecked() );
   settings.setValue( "DerMixDrun", mpDerMixDrun->isChecked() );
   settings.setValue( "DerMixDcmd", mpDerMixDcmd->text() );
   settings.setValue( "DerMixDparams", mpDerMixDparams->text() );
   settings.setValue( "AutoConnect", mpAutoConnect->isChecked() );
   settings.setValue( "CrossfadeTime", mpCrossfadeTime->value() );
   settings.setValue( "SLARTCommunication", mpSLARTCommunication->isChecked() );
   settings.setValue( "UDPListenerPort", mpUDPListenerPort->value() );
   settings.setValue( "NormalizeMode", mpNormalizeMode->currentIndex() );
   settings.setValue( "NormalizeValue", mpNormalizeValue->value() );
   settings.setValue( "LogCmd", mpLogCmd->text() );
   settings.setValue( "CountSkip", mpCountSkip->isChecked() );
   settings.setValue( "TrayIcon", mpTrayIcon->isChecked() );
   settings.setValue( "TrayIconBubble", mpTrayIconBubble->isChecked() );
   settings.setValue( "TrayIconBubbleTime", mpTrayIconBubbleTime->value() );
   settings.setValue( "PlayOnlyFavorite", mpPlayOnlyFavorite->isChecked() );
   settings.setValue( "PlayOnlyLeastPlayed", mpPlayOnlyLeastPlayed->isChecked() );
   settings.setValue( "PlayNotAgainCount", mpPlayNotAgainCount->value() );
   settings.setValue( "PlayerPattern", mpPlayerPattern->text() );
   settings.setValue( "ListPattern", mpListPattern->text() );
   settings.setValue( "NamePattern", mpNamePattern->text() );
   settings.setValue( "TrayIconPattern", mpTrayIconPattern->text() );
   if( mpPlayFolder->currentIndex() )
   {
      settings.setValue( "PlayFolder", mpPlayFolder->currentText() );
   }
   else
   {
      settings.remove( "PlayFolder" );
   }
   mpGlobalSettings->writeSettings();
   settings.sync();
   settings.sendNotification( "p0c" );

   emit configChanged();
}


void ConfigDialog::handleDerMixDrun( bool checked )
{
   mpDerMixDcmdLabel->setDisabled( !checked );
   mpDerMixDcmd->setDisabled( !checked );
   mpDerMixDparamsLabel->setDisabled( !checked );
   mpDerMixDparams->setDisabled( !checked );
   mpDerMixDhostLabel->setDisabled( checked );
   mpDerMixDhost->setDisabled( checked );
}


void ConfigDialog::handleUDPListen( bool checked )
{
   mpUDPListenerPort->setDisabled( !checked );
}


void ConfigDialog::handleNormalizeMode( int mode )
{
   mpNormalizeValue->setDisabled( mode != 2 );
}


void ConfigDialog::handleShowTrayIcon( bool checked )
{
   mpTrayIconBubble->setDisabled( !checked );
   mpTrayIconBubbleTime->setDisabled( !checked );
}
