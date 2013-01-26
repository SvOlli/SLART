/*
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

   QString iconFileName( QString(":/%1/Icon.png").arg( QCoreApplication::applicationName() ) );
   QLabel *aboutLabel = new QLabel( this );
   if( aboutText.isEmpty() )
   {
      QString message( "<table><tr valign='middle'><td><img src='%1'></td><td>%2 Version %3"
         " written by Sven Oliver Moll as a part of <a href='http://svolli.org/software/slart/'>"
         "%3</a>.<br>Distributed unter the terms of the "
         "<a href='http://www.gnu.org/licenses/gpl.html'>GPL</a>. Based upon the "
         "<a href='about:qt'>Qt</a> framework.</td></td></table>" );
      aboutLabel->setText( message.arg( iconFileName, QApplication::applicationName(),
                           SLART_VERSION, QApplication::organizationName() ) );
   }
   else
   {
      aboutLabel->setText( aboutText );
   }
   connect( aboutLabel, SIGNAL(linkActivated(QString)),
            this, SLOT(handleLink(QString)) );

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


