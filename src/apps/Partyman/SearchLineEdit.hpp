/*
 * src/apps/Partyman/SearchWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
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
#include "SearchWidget.hpp"


/*!
  \addtogroup Partyman

  @{
*/

/*!
 \brief TODO

*/
class SearchLineEdit : public QLineEdit
{
public:
   SearchLineEdit( SearchWidget *parent );
protected:
   virtual void keyPressEvent( QKeyEvent *event );
private:
   SearchWidget *mpSearchWidget;
};

/*! @} */

#endif
