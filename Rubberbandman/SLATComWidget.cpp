/**
 * SLATComWidget.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "SLATComWidget.hpp"

#include <QtGui>
#include <QString>

#include "FileSysBrowser.hpp"
#include "InfoEdit.hpp"

#include "Trace.hpp"


SLATComWidget::SLATComWidget( QWidget *parent, Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpInfoEdit( new InfoEdit() )
, mpNowPlaying( new QPushButton( tr("NP: to clipboard"), this ) )
, mSLATCom()
{
   QVBoxLayout *mainLayout = new QVBoxLayout( this );
   QHBoxLayout *buttonLayout = new QHBoxLayout;
   
   buttonLayout->addWidget( mpNowPlaying );
   mainLayout->addWidget( mpInfoEdit );
   mainLayout->addStretch();
   mainLayout->addLayout( buttonLayout );
   setLayout(mainLayout);
   
   connect( &mSLATCom, SIGNAL(packageRead(QStringList)),
            this, SLOT(handleSLAT(QStringList)) );

   connect( &mSLATCom, SIGNAL(updateConfig()),
            this, SLOT(readConfig()) );

   connect( mpNowPlaying, SIGNAL(clicked()),
            this, SLOT(handleNowPlaying()) );
   
   readConfig();
}


void SLATComWidget::handleSLAT( const QStringList &message )
{
   if( (message.at(0) == "p0p") && (message.size() > 1) )
   {
      mpInfoEdit->load( message.at(1) );
   }
}


void SLATComWidget::handleNowPlaying()
{
   QApplication::clipboard()->setText( mpInfoEdit->tagsFileName( "NP: |$ARTIST| - |$TITLE|", false ),
                                       QClipboard::Selection );
}


void SLATComWidget::readConfig()
{
   mSLATCom.resetReceiver();
}
