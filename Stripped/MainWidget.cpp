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
, mpEjectButton( new QPushButton( tr("Eject"), this ) )
, mEncoders()
{
   int i;
   char cwd[PATH_MAX];
   MySettings settings;

   mEncoders.append( new OggEncoder( this ) );
   mEncoders.append( new RawEncoder( this ) );

   QVBoxLayout *mainLayout   = new QVBoxLayout( this );
   QHBoxLayout *pathLayout   = new QHBoxLayout();
   QHBoxLayout *buttonLayout = new QHBoxLayout();
   
   QLabel *mpLogo = new QLabel( this );
   mpLogo->setText( tr("Stripped") );
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
   
   QComboBox   *devicesBox  = new QComboBox( this );
   QPushButton *tocButton   = new QPushButton( tr("Read Toc"), this );
   QPushButton *ripButton   = new QPushButton( tr("Rip Tracks"), this );
   QComboBox   *encodersBox = new QComboBox( this );
   
   connect( devicesBox, SIGNAL(currentIndexChanged(QString)),
            mpCDReader, SLOT(setDevice(QString)) );
   connect( encodersBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(changeEncoder(int)) );
   buttonLayout->addWidget( devicesBox );
   buttonLayout->addWidget( tocButton );
   buttonLayout->addWidget( ripButton );
   buttonLayout->addWidget( mpEjectButton );
   buttonLayout->addWidget( encodersBox );
   for( i = 0; i < mEncoders.size(); i++ )
   {
      buttonLayout->addWidget( mEncoders.at(i) );
      encodersBox->addItem( mEncoders.at(i)->name );
   }
   mpCDReader->getDevices( devicesBox );
   
   mainLayout->addWidget( mpLogo );
   mainLayout->addLayout( pathLayout );
   mainLayout->addWidget( mpCDDB );
   mainLayout->addWidget( mpCDEdit );
   mainLayout->addWidget( mpCDReader );
   mainLayout->addLayout( buttonLayout );
   
   setLayout( mainLayout );

   connect( tocButton, SIGNAL(pressed()), mpCDReader, SLOT(readToc()) );
   connect( ripButton, SIGNAL(pressed()), mpCDReader, SLOT(readTracks()) );
   connect( mpEjectButton, SIGNAL(pressed()), this, SLOT(eject()) );
   connect( mpCDDB, SIGNAL(tocUpdated()), mpCDEdit,   SLOT(updateCDDB()) );
   connect( mpDirButton, SIGNAL(clicked()), this,     SLOT(setDownloadDir()) );
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
TRACESTART(MainWidget::eject)
   mpEjectButton->setCheckable( true );
   mpEjectButton->setChecked( true );
   QCoreApplication::processEvents();
   mpCDReader->eject();
   mpEjectButton->setChecked( false );
   mpEjectButton->setCheckable( false );
}
