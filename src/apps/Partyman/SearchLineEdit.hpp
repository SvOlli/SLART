/*
 * src/apps/Partyman/SearchTrackWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef SEARCHLINEEDIT_HPP
#define SEARCHLINEEDIT_HPP SEARCHLINEEDIT_HPP

/* base class */
#include <QLineEdit>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */
#include "SearchTrackWidget.hpp"


/*!
  \addtogroup Partyman

  @{
*/

/*!
 \brief \todo complete documentation

*/
class SearchLineEdit : public QLineEdit
{
public:
   SearchLineEdit( SearchTrackWidget *parent );
protected:
   virtual void keyPressEvent( QKeyEvent *event );
private:
   SearchTrackWidget *mpSearchTrackWidget;
};

/*! @} */

#endif
