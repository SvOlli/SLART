/**
 * MainWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "MainWidget.hpp"

#include <QtGui>

#include "CDReader.hpp"
#include "CDToc.hpp"
#include "CDEdit.hpp"
#include "OggEncoder.hpp"
#include "RawEncoder.hpp"
#include "CDDB.hpp"
#include "MySettings.hpp"

#include "Trace.hpp"


MainWidget::MainWidget( QWidget *parent , Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpDirButton( new QPushButton( this ) )
, mpToc( new CDToc() )
, mpCDDB( new CDDB( mpToc, this ) )
, mpCDEdit( new CDEdit( mpToc, mpCDDB, this ) )
, mpCDReader( new CDReader( mpToc, mpCDEdit, this ) )
, mpCancelButton( new QPushButton( tr("Cancel"), this ) )
, mpButtonLayout( new QHBoxLayout() )
, mpDevicesBox( new QComboBox( this ) )
, mpTocButton( new QPushButton( tr("Read Toc"), this ) )
, mpRipButton( new QPushButton( tr("Rip Tracks"), this ) )
, mpEjectButton( new QPushButton( tr("Eject"), this ) )
, mpEncodersBox( new QComboBox( this ) )
, mEncoders()
{
   int i;
   char cwd[PATH_MAX];
   MySettings settings;

   mEncoders.append( new OggEncoder( this ) );
   mEncoders.append( new RawEncoder( this ) );

   QVBoxLayout *mainLayout   = new QVBoxLayout( this );
   QHBoxLayout *pathLayout   = new QHBoxLayout();
   
   QLabel *mpLogo = new QLabel( this );
   mpLogo->setText( QApplication::applicationName() );
   mpLogo->setAlignment( Qt::AlignCenter );
   mpLogo->setFrameShadow( QFrame::Raised );
   mpLogo->setFrameShape( QFrame::Box );

   QLabel *targetDirLabel   = new QLabel( tr("Base Directory"), this );
   pathLayout->addWidget( targetDirLabel );
   pathLayout->addWidget( mpDirButton );
   pathLayout->setStretchFactor( targetDirLabel,  0 );
   pathLayout->setStretchFactor( mpDirButton, 1 );
   QVariant targetDir( getcwd(&cwd[0], PATH_MAX) );
   mpDirButton->setText( settings.value("Directory", targetDir).toString() );
   chdir( mpDirButton->text().toLocal8Bit().constData() );

   connect( mpDevicesBox, SIGNAL(currentIndexChanged(QString)),
            mpCDReader, SLOT(setDevice(QString)) );
   connect( mpEncodersBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(changeEncoder(int)) );
   mpCancelButton->setDisabled( true );
   mpButtonLayout->addWidget( mpCancelButton );
   mpButtonLayout->addWidget( mpDevicesBox );
   mpButtonLayout->addWidget( mpTocButton );
   mpButtonLayout->addWidget( mpRipButton );
   mpButtonLayout->addWidget( mpEjectButton );
   mpButtonLayout->addWidget( mpEncodersBox );
   for( i = 0; i < mEncoders.size(); i++ )
   {
      mpButtonLayout->addWidget( mEncoders.at(i) );
      mpEncodersBox->addItem( mEncoders.at(i)->name );
   }
   mpCDReader->getDevices( mpDevicesBox );
   
   mainLayout->addWidget( mpLogo );
   mainLayout->addLayout( pathLayout );
   mainLayout->addWidget( mpCDDB );
   mainLayout->addWidget( mpCDEdit );
   mainLayout->addWidget( mpCDReader );
   mainLayout->addLayout( mpButtonLayout );
   
   setLayout( mainLayout );

   connect( mpTocButton, SIGNAL(pressed()),
            mpCDReader, SLOT(readToc()) );
   connect( mpRipButton, SIGNAL(pressed()),
            mpCDReader, SLOT(readTracks()) );
   connect( mpEjectButton, SIGNAL(pressed()),
            this, SLOT(eject()) );
   connect( mpCDDB, SIGNAL(tocUpdated()),
            mpCDEdit, SLOT(updateCDDB()) );
   connect( mpDirButton, SIGNAL(clicked()),
            this, SLOT(setDownloadDir()) );
   
   connect( mpCancelButton, SIGNAL(clicked()),
            mpCDReader, SLOT(cancel()) );
   connect( mpCancelButton, SIGNAL(clicked()),
            mpCDDB, SLOT(cancel()) );
   
   connect( mpCDReader, SIGNAL(starting()),
            this, SLOT(working()) );
   connect( mpCDReader, SIGNAL(stopping()),
            this, SLOT(finished()) );
}


void MainWidget::changeEncoder( int id )
{
   mpCDReader->setEncoder( mEncoders.at(id) );
   for( int i = 0; i < mEncoders.size(); i++ )
   {
      mEncoders.at(i)->setHidden( i != id );
   }
}


void MainWidget::setDownloadDir()
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

   chdir( mpDirButton->text().toLocal8Bit().constData() );
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
}


void MainWidget::working( bool allowCancel )
{
   int i;
   mpCancelButton->setDisabled( !allowCancel );
   mpDevicesBox->setDisabled( true );
   mpTocButton->setDisabled( true );
   mpRipButton->setDisabled( true );
   mpEjectButton->setDisabled( allowCancel );
   mpEncodersBox->setDisabled( true );
   for( i = 0; i < mEncoders.size(); i++ )
   {
      mEncoders.at(i)->setDisabled( true );
   }
}


void MainWidget::finished()
{
   int i;
   mpCancelButton->setDisabled( true );
   mpDevicesBox->setDisabled( false );
   mpTocButton->setDisabled( false );
   mpRipButton->setDisabled( false );
   mpEjectButton->setDisabled( false );
   mpEncodersBox->setDisabled( false );
   for( i = 0; i < mEncoders.size(); i++ )
   {
      mEncoders.at(i)->setDisabled( false );
   }
}
