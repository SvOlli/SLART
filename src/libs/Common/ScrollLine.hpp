/*
 * src/libs/Common/ScrollLine.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef SCROLLLINE_HPP
#define SCROLLLINE_HPP SCROLLLINE_HPP

/* base class */
#include <QLineEdit>

/* system headers */

/* Qt headers */
#include <QString>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QMouseEvent;
class QTimer;

/* forward declaration of local classes */


/*!
  \addtogroup Common

  @{
*/

/*!
 \brief \todo complete documentation

 \dotfile "graphs/libs/Common/ScrollLine_connect.dot" "Connect Graph"
*/
class ScrollLine : public QLineEdit
{
   Q_OBJECT

public:
   ScrollLine( QWidget *parent, bool autoScroll = true );
   virtual ~ScrollLine();

   /*!
    \brief set the filename to use when dragged

   */
   void setDragFileName( const QString &fileName = QString() );

public slots:
   /*!
    \brief callback for timer to scroll

   */
   void scrolling();
   /*!
    \brief intercept setText for resetting scroll position

   */
   void setText( const QString &text );

protected:
   /*!
    \brief handle mouse event for correct copy'n'paste

   */
   void mouseDoubleClickEvent( QMouseEvent *event );
   /*!
    \brief handle mouse event for correct copy'n'paste

   */
   void mousePressEvent( QMouseEvent *event );
   /*!
    \brief handle mouse event for correct copy'n'paste

   */
   void mouseReleaseEvent( QMouseEvent *event );
   /*!
    \brief intecepting paint event to change coloring role

   */
   void paintEvent( QPaintEvent *event );

private:
   Q_DISABLE_COPY( ScrollLine )

   static QTimer        *cpTimer;
   bool                 mClicked;
   int                  mDirection;
   int                  mPosition;
   QString              mDragFileName;
};

/*! @} */

#endif
