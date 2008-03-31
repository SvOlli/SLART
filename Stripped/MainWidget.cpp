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
#include "CDDB.hpp"
#include "ConfigDialog.hpp"
#include "MySettings.hpp"

#include "Trace.hpp"


MainWidget::MainWidget( QWidget *parent , Qt::WindowFlags flags )
: QWidget( parent, flags )
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
   char cwd[PATH_MAX];
   MySettings settings;

   QVBoxLayout *mainLayout   = new QVBoxLayout( this );
#if QT_VERSION < 0x040300
   mainLayout->setMargin( 3 );
#else
   mainLayout->setContentsMargins( 3, 3, 3, 3 );
#endif
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

   mpCancelButton->setDisabled( true );
   mpButtonLayout->addWidget( mpSettingsButton );
   mpButtonLayout->addWidget( mpCancelButton );
   mpButtonLayout->addWidget( mpTocButton );
   mpButtonLayout->addWidget( mpCDTextButton );
   mpButtonLayout->addWidget( mpRipButton );
   mpButtonLayout->addWidget( mpEjectButton );

   mainLayout->addWidget( mpLogo );
   mainLayout->addLayout( pathLayout );
   mainLayout->addWidget( mpCDDB );
   mainLayout->addWidget( mpCDEdit );
   mainLayout->addWidget( mpCDReader );
   mainLayout->addLayout( mpButtonLayout );
   
   setLayout( mainLayout );

   connect( mpSettingsButton, SIGNAL(pressed()),
            mpConfigDialog, SLOT(exec()) );
   connect( mpTocButton, SIGNAL(pressed()),
            mpCDReader, SLOT(readToc()) );
   connect( mpCDTextButton, SIGNAL(pressed()),
            mpCDReader, SLOT(readCDText()) );
   connect( mpRipButton, SIGNAL(pressed()),
            mpCDReader, SLOT(readTracks()) );
   connect( mpEjectButton, SIGNAL(pressed()),
            this, SLOT(eject()) );
   connect( mpCDDB, SIGNAL(tocUpdated()),
            mpCDEdit, SLOT(updateCDDB()) );
   connect( mpDirButton, SIGNAL(clicked()),
            this, SLOT(setRippingDir()) );
   
   connect( mpCancelButton, SIGNAL(clicked()),
            mpCDReader, SLOT(cancel()) );
   connect( mpCancelButton, SIGNAL(clicked()),
            mpCDDB, SLOT(cancel()) );
   
   connect( mpCDReader, SIGNAL(starting()),
            this, SLOT(working()) );
   connect( mpCDReader, SIGNAL(stopping()),
            this, SLOT(finished()) );
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
   mpRipButton->setDisabled( false );
   mpEjectButton->setDisabled( false );
}
