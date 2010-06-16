/**
 * src/apps/Rubberbandman/BrowseWidget.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef BROWSEWIDGET_HPP
#define BROWSEWIDGET_HPP BROWSEWIDGET_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */
#include <QDir>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QPushButton;
class QString;

/* forward declaration of local classes */
class Database;
class FileSysBrowser;
class InfoEdit;


class BrowseWidget : public QWidget
{
Q_OBJECT
   
public:
   BrowseWidget( Database *database, QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   
public slots:
   /* show a specific file in filesystem tree */
   void scrollTo( const QString &fileName );
   
private:
   BrowseWidget( const BrowseWidget &other );
   BrowseWidget &operator=( const BrowseWidget &other );
   
   FileSysBrowser *mpFileSysBrowser;
   InfoEdit       *mpInfoEdit;
};

#endif
