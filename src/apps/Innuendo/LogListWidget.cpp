/*
 * src/Partyman/LogListModel.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "LogListWidget.hpp"

/* system headers */

/* Qt headers */
#include <QFileInfo>
#include <QMimeData>
#include <QUrl>

/* local library headers */

/* local headers */
#include "ConfigDialog.hpp"


LogListWidget::LogListWidget( QWidget *parent )
: QListWidget( parent )
{
}


QMimeData *LogListWidget::mimeData( const QList<QListWidgetItem *> items ) const
{
   QMimeData *mimeData = new QMimeData();

   QList<QUrl> urls;
   QStringList mimeText;
   QString itemText;
   foreach( const QListWidgetItem *item, items )
   {
      itemText = item->text();
      if( !itemText.isEmpty() )
      {
         mimeText << itemText;
         if( itemText.startsWith("/") )
         {
            urls << QUrl::fromLocalFile( itemText );
         }
      }
   }
   mimeData->setText( mimeText.join("\n") );
   mimeData->setUrls( urls );

   return mimeData;
}


QStringList LogListWidget::mimeTypes() const
{
   QStringList types;
   types << "text/plain" << "text/uri-list";
   return types;
}
