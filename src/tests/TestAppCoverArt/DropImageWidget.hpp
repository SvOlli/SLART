/*
 * src/libs/Common/MainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef DROPIMAGEWIDGET_HPP
#define DROPIMAGEWIDGET_HPP DROPIMAGEWIDGET_HPP

/* base class */
#include <ImageWidget.hpp>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QDragEnterEvent;
class QDropEvent;
class QUrl;

/* forward declaration of local classes */


/*!
  \addtogroup Common

  @{
*/

/*!
 \brief \todo complete documentation

*/
class DropImageWidget : public ImageWidget
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param parent
   */
   DropImageWidget( QWidget *parent );

   /*!
    \brief destructor

   */
   virtual ~DropImageWidget();

protected:
   /*!
    \brief for implementing dropping

    \param event
   */
   void dragEnterEvent( QDragEnterEvent *event );

   /*!
    \brief for implementing dropping

    \param event
   */
   void dropEvent( QDropEvent *event );

public slots:

signals:
   /*!
    \brief notify of an url that has been dropped

    If a list of urls has been dropped only notify using the first.

    \param url
   */
   void droppedUrl( const QUrl &url );

private:
   Q_DISABLE_COPY( DropImageWidget )
};

/*! @} */

#endif
