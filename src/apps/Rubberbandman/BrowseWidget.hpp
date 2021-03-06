/*
 * src/apps/Rubberbandman/BrowseWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
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


/*!
  \addtogroup Rubberbandman

  @{
*/

/*!
 \brief widget for file system browser and item editor

 \dotfile "graphs/apps/Rubberbandman/BrowseWidget_connect.dot" "Connect Graph"
*/
class BrowseWidget : public QWidget
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param parent
   */
   BrowseWidget( QWidget *parent = 0 );

   /*!
    \brief destructor

   */
   virtual ~BrowseWidget();

public slots:
   /*!
    \brief show a specific file in filesystem tree

    \param fileName
   */
   void scrollTo( const QString &fileName );

private:
   Q_DISABLE_COPY( BrowseWidget )

   FileSysBrowser *mpFileSysBrowser; /*!< \brief file system browser */
   InfoEdit       *mpInfoEdit; /*!< \brief item editor */
};

/*! @} */

#endif
