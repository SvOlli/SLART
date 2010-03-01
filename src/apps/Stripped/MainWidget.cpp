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
#include "CDDB.hpp"
#include "CDEdit.hpp"
#include "CDReader.hpp"
#include "CDToc.hpp"
#include "ConfigDialog.hpp"


MainWidget::MainWidget( QWidget *parent , Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpSatellite( Satellite::get( this ) )
, mpDirButton( new QPushButton( this ) )
, mpToc( new CDToc() )
, mpCDDB( new CDDB( mpToc, this ) )
, mpCDEdit( new CDEdit( mpToc, mpCDDB, this ) )
, mpCDReader( new CDReader( mpToc, mpCDEdit, this ) )
, mpConfigDialog( new ConfigDialog( mpCDReader, this ) )
, mpSettingsButton( new QPushButton( tr("Settings"), this ) )
, mpCancelButton( new QPushButton( tr("Cancel"), this ) )
, mpButtonLayout( new QHBoxLayout() )
, mpTocButton( new QPushButton( tr("Read Toc"), this ) )
, mpCDTextButton( new QPushButton( tr("Read CDText"), this ) )
, mpRipButton( new QPushButton( tr("Rip Tracks"), this ) )
, mpEjectButton( new QPushButton( tr("Eject"), this ) )
{
   MySettings settings;
   mpRipButton->setEnabled( false );
   
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
   mpDirButton->setText( settings.VALUE_DIRECTORY );
   QDir::setCurrent( mpDirButton->text() );

   mpCancelButton->setDisabled( true );
   mpButtonLayout->addWidget( mpSettingsButton );
   mpButtonLayout->addWidget( mpCancelButton );
   mpButtonLayout->addWidget( mpTocButton );
   mpButtonLayout->addWidget( mpCDTextButton );
   mpButtonLayout->addWidget( mpRipButton );
   mpButtonLayout->addWidget( mpEjectButton );

   mainLayout->addLayout( pathLayout );
   mainLayout->addWidget( mpCDDB );
   mainLayout->addWidget( mpCDEdit );
   mainLayout->addWidget( mpCDReader );
   mainLayout->addLayout( mpButtonLayout );
   
   setLayout( mainLayout );

   connect( mpSettingsButton, SIGNAL(pressed()),
            mpConfigDialog, SLOT(exec()) );
   connect( mpTocButton, SIGNAL(pressed()),
            mpCDReader, SLOT(readTocCDDB()) );
   connect( mpCDTextButton, SIGNAL(pressed()),
            mpCDReader, SLOT(readTocCDText()) );
   connect( mpRipButton, SIGNAL(pressed()),
            mpCDReader, SLOT(readTracks()) );
   connect( mpEjectButton, SIGNAL(pressed()),
            this, SLOT(eject()) );
   connect( mpCDDB, SIGNAL(tocUpdated()),
            mpCDEdit, SLOT(updateCDDB()) );
   connect( mpDirButton, SIGNAL(clicked()),
            this, SLOT(setRippingDir()) );
   connect( mpCDEdit, SIGNAL(containsData(bool)),
            mpRipButton, SLOT(setEnabled(bool)) );
   
   connect( mpCancelButton, SIGNAL(clicked()),
            mpCDReader, SLOT(cancel()) );
   connect( mpCancelButton, SIGNAL(clicked()),
            mpCDDB, SLOT(cancel()) );
   
   connect( mpCDReader, SIGNAL(starting()),
            this, SLOT(working()) );
   connect( mpCDReader, SIGNAL(stopping()),
            this, SLOT(finished()) );
   
   mpSettingsButton->setObjectName( QString("SettingsButton") );
   mpSatellite->restart();
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

   QDir::setCurrent( mpDirButton->text() );
}


void MainWidget::eject()
{
   working( false );
   mpEjectButton->setCheckable( true );
   mpEjectButton->setChecked( true );
   QCoreApplication::processEvents();
   mpCDReader->eject();
   mpEjectButton->setChecked( false );
   mpEjectButton->setCheckable( false );
   finished();
   mpCDEdit->clear();
}


void MainWidget::working( bool allowCancel )
{
   mpCancelButton->setDisabled( !allowCancel );
   mpSettingsButton->setDisabled( true );
   mpTocButton->setDisabled( true );
   mpCDTextButton->setDisabled( true );
   mpRipButton->setDisabled( true );
   mpEjectButton->setDisabled( allowCancel );
}


void MainWidget::finished()
{
   mpCancelButton->setDisabled( true );
   mpSettingsButton->setDisabled( false );
   mpTocButton->setDisabled( false );
   mpCDTextButton->setDisabled( false );
   mpRipButton->setDisabled( mpCDEdit->isEmpty() );
   mpEjectButton->setDisabled( false );
}
