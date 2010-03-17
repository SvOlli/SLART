/**
 * src/apps/Stripped/MainWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "MainWidget.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <MySettings.hpp>
#include <Satellite.hpp>

/* local headers */
#include "CDDBClient.hpp"
#include "CDEdit.hpp"
#include "CDInfo.hpp"
#include "CDReader.hpp"
#include "ConfigDialog.hpp"

#include <QTimer>

MainWidget::MainWidget( QWidget *parent , Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpSatellite( Satellite::get( this ) )
, mpDirButton( new QPushButton( this ) )
, mpCDInfo( new CDInfo() )
, mpCDDBClient( new CDDBClient( mpCDInfo, this ) )
, mpCDEdit( new CDEdit( mpCDInfo, mpCDDBClient, this ) )
, mpCDReader( new CDReader( mpCDInfo, mpCDEdit, this ) )
, mpMessage( new QLabel( this ) )
, mpConfigDialog( new ConfigDialog( mpCDReader, this ) )
, mpButtonLayout( new QHBoxLayout() )
, mpSettingsButton( new QPushButton( tr("Settings"), this ) )
, mpCancelButton( new QPushButton( tr("Cancel"), this ) )
, mpScanButton( new QPushButton( tr("Scan CD"), this ) )
, mpRipButton( new QPushButton( tr("Rip Tracks"), this ) )
, mpEjectButton( new QPushButton( tr("Eject"), this ) )
{
   MySettings settings;

   mpRipButton->setEnabled( false );
   mpDirButton->setText( settings.VALUE_DIRECTORY );
   
   /* for style sheets */
   mpSettingsButton->setObjectName( QString("SettingsButton") );

   QVBoxLayout *mainLayout   = new QVBoxLayout( this );
#if QT_VERSION < 0x040300
   mainLayout->setMargin( 3 );
#else
   mainLayout->setContentsMargins( 3, 3, 3, 3 );
#endif
   parent->setWindowIcon( QIcon( ":/SLART.png" ) );
   QHBoxLayout *pathLayout   = new QHBoxLayout();
   
   QLabel *targetDirLabel   = new QLabel( tr("Base Directory"), this );
   pathLayout->addWidget( targetDirLabel );
   pathLayout->addWidget( mpDirButton );
   pathLayout->setStretchFactor( targetDirLabel,  0 );
   pathLayout->setStretchFactor( mpDirButton, 1 );

   mpCancelButton->setDisabled( true );
   mpButtonLayout->addWidget( mpSettingsButton );
   mpButtonLayout->addWidget( mpCancelButton );
   mpButtonLayout->addWidget( mpScanButton );
   mpButtonLayout->addWidget( mpRipButton );
   mpButtonLayout->addWidget( mpEjectButton );

   mainLayout->addLayout( pathLayout );
   mainLayout->addWidget( mpCDDBClient );
   mainLayout->addWidget( mpCDEdit );
   mainLayout->addWidget( mpCDReader );
   mainLayout->addWidget( mpMessage );
   mainLayout->addLayout( mpButtonLayout );
   
   setLayout( mainLayout );

   /* buttons */
   connect( mpDirButton, SIGNAL(clicked()),
            this, SLOT(setRippingDir()) );

   connect( mpSettingsButton, SIGNAL(pressed()),
            mpConfigDialog, SLOT(exec()) );
   connect( mpConfigDialog, SIGNAL(configChanged()),
            this, SLOT(handleConfigUpdate()) );

   connect( mpCancelButton, SIGNAL(clicked()),
            mpCDReader, SLOT(cancel()) );
   connect( mpCancelButton, SIGNAL(clicked()),
            mpCDDBClient, SLOT(cancel()) );

   connect( mpScanButton, SIGNAL(pressed()),
            mpCDReader, SLOT(readToc()) );

   connect( mpRipButton, SIGNAL(pressed()),
            mpCDReader, SLOT(readTracks()) );
   connect( mpCDEdit, SIGNAL(containsData(bool)),
            mpRipButton, SLOT(setEnabled(bool)) );

   connect( mpEjectButton, SIGNAL(pressed()),
            this, SLOT(eject()) );

   /* edit sheet interaction */
   connect( mpCDReader, SIGNAL(gotToc()),
            mpCDEdit, SLOT(update()) );
   connect( mpCDDBClient, SIGNAL(infoUpdated()),
            mpCDEdit, SLOT(update()) );
   connect( mpCDReader, SIGNAL(setTrackDisabled(int,bool)),
            mpCDEdit, SLOT(setTrackDisabled(int,bool)) );
   connect( mpCDReader, SIGNAL(ensureVisible(int)),
            mpCDEdit, SLOT(ensureVisible(int)) );

   /* other interaction */
   connect( mpConfigDialog, SIGNAL(stateNoDrive()),
            this, SLOT(stateNoDrive()) );

   connect( mpCDReader, SIGNAL(stateNoDisc()),
            this, SLOT(stateNoDisc()) );
   connect( mpCDReader, SIGNAL(stateDisc()),
            this, SLOT(stateDisc()) );
   connect( mpCDReader, SIGNAL(stateScan()),
            this, SLOT(stateScan()) );
   connect( mpCDReader, SIGNAL(stateRip()),
            this, SLOT(stateRip()) );

   connect( mpCDDBClient, SIGNAL(stateNet()),
            this, SLOT(stateNet()) );
   connect( mpCDDBClient, SIGNAL(stateDisc()),
            this, SLOT(stateDisc()) );

   connect( mpCDDBClient, SIGNAL(requestCDText()),
            mpCDReader, SLOT(readCDText()) );
   connect( mpCDDBClient, SIGNAL(message(const QString &)),
            this, SLOT(showMessage(const QString &)) );
   connect( mpCDReader, SIGNAL(message(const QString &)),
            this, SLOT(showMessage(const QString &)) );

   mpSatellite->restart();
   handleConfigUpdate();
   stateNoDisc();
   QTimer::singleShot( 2000, this, SLOT(stateNoDisc()) );
}


void MainWidget::setRippingDir()
{
   QFileDialog fileDialog( this );

   fileDialog.setFileMode( QFileDialog::DirectoryOnly );
   fileDialog.setDirectory( mpDirButton->text() );
   fileDialog.setReadOnly( false );

   if( fileDialog.exec() )
   {
      MySettings settings;
      QString result( fileDialog.selectedFiles().at(0) );
      mpDirButton->setText( result );
      settings.setValue( "Directory", result.replace('\\','/') );
   }
}


void MainWidget::eject()
{
   mpCDDBClient->clear();
   mpCDEdit->clear();
   mpCDReader->eject();
}


void MainWidget::showMessage( const QString &message )
{
   mpMessage->setText( message );
}


void MainWidget::handleConfigUpdate()
{
   MySettings settings;
   if( settings.VALUE_AUTOFREEDB )
   {
      connect( mpCDReader, SIGNAL(gotToc()),
               mpCDDBClient, SLOT(handleComboBox()) );
   }
   else
   {
      disconnect( mpCDReader, SIGNAL(gotToc()),
                  mpCDDBClient, SLOT(handleComboBox()) );
   }
}


void MainWidget::stateNoDrive()
{
   mpCDEdit->clear();
   mpCDEdit->setDisabled( true );
   mpCDDBClient->setDisabled( true );
   mpSettingsButton->setDisabled( false );
   mpCancelButton->setDisabled( true );
   mpScanButton->setDisabled( true );
   mpRipButton->setDisabled( true );
   mpEjectButton->setDisabled( true );
   QMessageBox::critical(this, QApplication::applicationName(),
         tr("No drive found capable of reading audio CDs.") );
}


void MainWidget::stateNoDisc()
{
   mpCDEdit->clear();
   mpCDEdit->setDisabled( true );
   mpCDDBClient->setDisabled( true );
   mpSettingsButton->setDisabled( false );
   mpCancelButton->setDisabled( true );
   mpScanButton->setDisabled( false );
   mpRipButton->setDisabled( true );
   mpEjectButton->setDisabled( false );
}


void MainWidget::stateDisc()
{
   mpCDEdit->setDisabled( false );
   mpCDDBClient->setDisabled( false );
   mpSettingsButton->setDisabled( false );
   mpCancelButton->setDisabled( true );
   mpScanButton->setDisabled( false );
   mpRipButton->setDisabled( false );
   mpEjectButton->setDisabled( false );
}


void MainWidget::stateScan()
{
   mpCDEdit->setDisabled( false );
   mpCDDBClient->setDisabled( false );
   mpSettingsButton->setDisabled( true );
   mpCancelButton->setDisabled( false );
   mpScanButton->setDisabled( true );
   mpRipButton->setDisabled( true );
   mpEjectButton->setDisabled( true );
}


void MainWidget::stateNet()
{
   mpCDEdit->setDisabled( false );
   mpCDDBClient->setDisabled( false );
   mpSettingsButton->setDisabled( true );
   mpCancelButton->setDisabled( false );
   mpScanButton->setDisabled( true );
   mpRipButton->setDisabled( true );
   mpEjectButton->setDisabled( true );
}


void MainWidget::stateRip()
{
   mpCDEdit->setDisabled( false );
   mpCDDBClient->setDisabled( false );
   mpSettingsButton->setDisabled( true );
   mpCancelButton->setDisabled( false );
   mpScanButton->setDisabled( true );
   mpRipButton->setDisabled( true );
   mpEjectButton->setDisabled( true );
}
