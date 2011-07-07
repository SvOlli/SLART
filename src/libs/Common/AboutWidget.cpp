/**
 * src/libs/Common/AboutWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#include "AboutWidget.hpp"
#include "Trace.hpp"
#include "Version.hpp"

#include <QtGui>


AboutWidget::AboutWidget( QWidget *parent, const QString &aboutText )
: QWidget( parent )
{
   QVBoxLayout *mainLayout = new QVBoxLayout( this );
   mainLayout->setContentsMargins( 1, 1, 1, 1 );

   QLabel *aboutLabel = new QLabel( this );
   if( aboutText.isEmpty() )
   {
      QString message( "<table><tr><td><img src=':/SLART.png'></td><td>%1 Version %2"
         " written by Sven Oliver Moll as a part of <a href='http://svolli.org/software/slart/'>"
         "%3</a>.<br>Distributed unter the terms of the "
         "<a href='http://www.gnu.org/licenses/gpl.html'>GPL</a>. Based upon the "
         "<a href='about:qt'>Qt</a> framework.</td></td></table>" );
      aboutLabel->setText( message.arg( QApplication::applicationName(),
                           SLART_VERSION, QApplication::organizationName() ) );
   }
   else
   {
      aboutLabel->setText( aboutText );
   }
   connect( aboutLabel, SIGNAL(linkActivated(const QString&)),
            this, SLOT(handleLink(const QString&)) );

   mainLayout->addWidget( aboutLabel );
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
   }
   else
   {
      QDesktopServices::openUrl( QUrl(link) );
   }
}


