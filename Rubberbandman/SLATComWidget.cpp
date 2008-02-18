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
, mSLATCom()
{
TRACESTART(SLATComWidget::SLATComWidget)
   QVBoxLayout *layout = new QVBoxLayout;
   layout->addWidget( mpInfoEdit );
   setLayout(layout);
   
   connect( &mSLATCom, SIGNAL(packageRead(QStringList)),
            this, SLOT(handleSLAT(QStringList)) );

   connect( &mSLATCom, SIGNAL(updateConfig()),
            this, SLOT(readConfig()) );

   readConfig();
}


void SLATComWidget::handleSLAT( const QStringList &message )
{
TRACESTART(SLATComWidget::handleSLAT)
TRACEMSG << message;
   if( (message.at(0) == "p0p") && (message.size() > 1) )
   {
      mpInfoEdit->load( message.at(1) );
   }
}


void SLATComWidget::readConfig()
{
   mSLATCom.resetReceiver();
}
