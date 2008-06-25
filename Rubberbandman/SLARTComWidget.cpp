/**
 * SLARTComWidget.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "SLARTComWidget.hpp"

#include <QtGui>
#include <QString>

#include "Database.hpp"
#include "FileSysBrowser.hpp"
#include "InfoEdit.hpp"
#include "GlobalConfigWidget.hpp"
#include "MySettings.hpp"

#include "Trace.hpp"


SLARTComWidget::SLARTComWidget( Database *database, QWidget *parent, Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpDatabase( database )
, mpInfoEdit( new InfoEdit( database ) )
, mpNowPlaying( new QPushButton( tr("NP: To Clipboard"), this ) )
, mpShowInFilesystem( new QPushButton( tr("Show In Filesystem"), this ) )
, mpFavorite( new QCheckBox( tr("Favorite Track"), this ) )
, mpUnwanted( new QCheckBox( tr("Unwanted Track"), this ) )
, mSLARTCom()
, mTrackInfo()
{
   QVBoxLayout *mainLayout = new QVBoxLayout( this );
   QHBoxLayout *buttonLayout = new QHBoxLayout;
   QHBoxLayout *checkboxLayout = new QHBoxLayout;
   
   buttonLayout->addWidget( mpNowPlaying );
   buttonLayout->addWidget( mpShowInFilesystem );
   checkboxLayout->addWidget( mpFavorite );
   checkboxLayout->addWidget( mpUnwanted );
   mainLayout->addStretch();
   mainLayout->addLayout( buttonLayout );
   mainLayout->addLayout( checkboxLayout );
   mainLayout->addStretch();
   mainLayout->addWidget( mpInfoEdit );
   setLayout(mainLayout);
   
   connect( &mSLARTCom, SIGNAL(packageRead(QStringList)),
            this, SLOT(handleSLART(QStringList)) );
   connect( &mSLARTCom, SIGNAL(updateConfig()),
            this, SLOT(readConfig()) );
   connect( mpNowPlaying, SIGNAL(clicked()),
            this, SLOT(handleNowPlaying()) );
   connect( mpShowInFilesystem, SIGNAL(clicked()),
            this, SLOT(handleShowInFilesystem()) );
   connect( mpFavorite, SIGNAL(clicked()),
            this, SLOT(handleFavorite()) );
   connect( mpUnwanted, SIGNAL(clicked()),
            this, SLOT(handleUnwanted()) );
   
   readConfig();
}


void SLARTComWidget::handleSLART( const QStringList &message )
{
   if( (message.at(0) == "p0p") && (message.size() > 1) )
   {
      mpInfoEdit->load( message.at(1) );
      mpDatabase->getTrackInfoByFileName( &mTrackInfo, message.at(1) );
      mpFavorite->setChecked( mTrackInfo.isFlagged( TrackInfo::Favorite ) );
      mpUnwanted->setChecked( mTrackInfo.isFlagged( TrackInfo::Unwanted ) );
   }
}


void SLARTComWidget::handleNowPlaying()
{
   GlobalConfigWidget::setClipboard( mpInfoEdit->tagsFileName( 
      MySettings().value("PlayingPattern", "NP: |$ARTIST| - |$TITLE|").toString(), false ) );
}


void SLARTComWidget::handleShowInFilesystem()
{
   if( !(mpInfoEdit->fileName().isEmpty()) )
   {
      emit showInFilesystem( mpInfoEdit->fileName() );
   }
}


void SLARTComWidget::handleFavorite()
{
   if( mpFavorite->isChecked() )
   {
      mpUnwanted->setChecked( false );
   }
   mTrackInfo.setFlag( TrackInfo::Favorite, mpFavorite->isChecked() );
   mpDatabase->beginTransaction();
   mpDatabase->updateTrackInfo( &mTrackInfo );
   mpDatabase->endTransaction( true );
}


void SLARTComWidget::handleUnwanted()
{
   if( mpUnwanted->isChecked() )
   {
      mpFavorite->setChecked( false );
   }
   mTrackInfo.setFlag( TrackInfo::Unwanted, mpUnwanted->isChecked() );
   mpDatabase->beginTransaction();
   mpDatabase->updateTrackInfo( &mTrackInfo );
   mpDatabase->endTransaction( true );
}


void SLARTComWidget::readConfig()
{
   mSLARTCom.resetReceiver();
}
