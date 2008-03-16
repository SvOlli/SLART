/**
 * AboutWidget.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "AboutWidget.hpp"
#include "ConfigDialog.hpp"

#include <QtGui>

#include "Trace.hpp"

#include "Version.hpp"


AboutWidget::AboutWidget( ConfigDialog *config, QWidget *parent )
: QWidget( parent )
, mpConfig( config )
{
   QVBoxLayout *mainLayout = new QVBoxLayout( this );
#if QT_VERSION < 0x040300
   mainLayout->setMargin( 1 );
#else
   mainLayout->setContentsMargins( 1, 1, 1, 1 );
#endif
   
   QLabel *aboutText = new QLabel( tr("<table align='center'><tr><td align='center'>Our hail to the new king:</td></tr>"
   "<tr><td align='center'><a href='http://svolli.org/software/partyman/'><img src=':/PartymanSmile.gif'></a>&nbsp;"
   "&nbsp;<a href='http://svolli.org/software/partyman/'><img src=':/PartymanWriting.gif'></a></td></tr>"
   "<tr><td align='center'>Version " SLART_VERSION " written by Sven Oliver Moll.</td></tr>"
   "<tr><td align='center'>Distributed unter the terms of the <a href='http://www.gnu.org/licenses/gpl.html'>GPL</a>.<br>"
   "This is a frontend for <a href='http://dermixd.de/'>DerMixD</a> using <a href='about:qt'>Qt</a>.<br>"
   "<b>[<a href='about:settings'><font color='#ff0000'>Settings</font></a>]</b></td></tr></table>" ), this );
   connect( aboutText, SIGNAL(linkActivated(QString)), this, SLOT(handleLink(QString)) );

   mainLayout->addWidget( aboutText );
   setLayout( mainLayout );
}


void AboutWidget::handleLink( const QUrl &link )
{
   handleLink( link.toString() );
}


void AboutWidget::handleLink( const QString &link )
{
   if( link.startsWith("about:") )
   {
      if( link.endsWith( ":qt" ) )
      {
         QMessageBox::aboutQt( this, QApplication::applicationName() );
      }
      if( link.endsWith( ":settings" ) )
      {
         mpConfig->exec();
      }
   }
   else
   {
      QDesktopServices::openUrl( link );
   }
}


