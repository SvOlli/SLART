/**
 * SLARTComWidget.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "SLARTComWidget.hpp"

#include <QtGui>
#include <QString>

#include "FileSysBrowser.hpp"
#include "InfoEdit.hpp"
#include "GlobalConfigWidget.hpp"
#include "MySettings.hpp"

#include "Trace.hpp"


SLARTComWidget::SLARTComWidget( QWidget *parent, Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpInfoEdit( new InfoEdit() )
, mpNowPlaying( new QPushButton( tr("NP: To Clipboard"), this ) )
, mpShowInFilesystem( new QPushButton( tr("Show In Filesystem"), this ) )
, mSLARTCom()
{
   QVBoxLayout *mainLayout = new QVBoxLayout( this );
   QHBoxLayout *buttonLayout = new QHBoxLayout;
   
   buttonLayout->addWidget( mpNowPlaying );
   buttonLayout->addWidget( mpShowInFilesystem );
   mainLayout->addStretch();
   mainLayout->addLayout( buttonLayout );
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
   
   readConfig();
}


void SLARTComWidget::handleSLART( const QStringList &message )
{
   if( (message.at(0) == "p0p") && (message.size() > 1) )
   {
      mpInfoEdit->load( message.at(1) );
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


void SLARTComWidget::readConfig()
{
   mSLARTCom.resetReceiver();
}
