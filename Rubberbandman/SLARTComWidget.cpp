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

#include "Trace.hpp"


SLARTComWidget::SLARTComWidget( QWidget *parent, Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpInfoEdit( new InfoEdit() )
, mpNowPlaying( new QPushButton( tr("NP: to clipboard"), this ) )
, mSLARTCom()
{
   QVBoxLayout *mainLayout = new QVBoxLayout( this );
   QHBoxLayout *buttonLayout = new QHBoxLayout;
   
   buttonLayout->addWidget( mpNowPlaying );
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
   QApplication::clipboard()->setText( mpInfoEdit->tagsFileName( "NP: |$ARTIST| - |$TITLE|", false ),
                                       QClipboard::Selection );
}


void SLARTComWidget::readConfig()
{
   mSLARTCom.resetReceiver();
}
