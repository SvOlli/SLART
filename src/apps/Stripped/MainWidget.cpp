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
   connect( mpCDReader, SIGNAL(starting()),
            this, SLOT(working()) );
   connect( mpCDReader, SIGNAL(stopping()),
            this, SLOT(finished()) );
   connect( mpCDDBClient, SIGNAL(message(const QString &)),
            this, SLOT(showMessage(const QString &)) );
   connect( mpCDReader, SIGNAL(message(const QString &)),
            this, SLOT(showMessage(const QString &)) );

   mpSatellite->restart();
   handleConfigUpdate();
}


void MainWidget::setState( enum operationState state )
{
   switch( state )
   {
      case stateInit:
      case stateNoDrive:
      case stateIdle:
      case stateScan:
      case stateNet:
      case stateRip:
      default:
         break;
   }
   mState = state;
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


void MainWidget::working( bool allowCancel )
{
   mpCancelButton->setDisabled( !allowCancel );
   mpSettingsButton->setDisabled( true );
   mpScanButton->setDisabled( true );
   mpRipButton->setDisabled( true );
   mpEjectButton->setDisabled( allowCancel );
}


void MainWidget::finished()
{
   mpCancelButton->setDisabled( true );
   mpSettingsButton->setDisabled( false );
   mpScanButton->setDisabled( false );
   mpRipButton->setDisabled( mpCDEdit->isEmpty() );
   mpEjectButton->setDisabled( false );
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
