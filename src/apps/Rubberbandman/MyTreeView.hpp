/*
 * src/apps/Rubberbandman/MyTreeView.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef MYTREEVIEW_HPP
#define MYTREEVIEW_HPP MYTREEVIEW_HPP

/*!
    \brief class declaration

   */
#include <QTreeView>

/* system headers */

/* Qt headers */
#include <QModelIndex>

/* local library headers */
#include <Database.hpp>
#include <DirWalker.hpp>
#include <Satellite.hpp>

/* local headers */

/* forward declaration of Qt classes */
class QTimer;

/* forward declaration of local classes */


/*!
  \addtogroup Rubberbandman

  @{
*/

/*!
 \brief \todo complete documentation

*/
class MyTreeView : public QTreeView
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param parent
   */
   MyTreeView( QWidget *parent );

   /*!
    \brief destructor

   */
   virtual ~MyTreeView();

public slots:
   /*!
    \brief \todo

   */
   void scrollToActive();

protected:
   /*!
    \brief handle return/enter key

   */
   virtual void keyPressEvent( QKeyEvent *event );

private:
   Q_DISABLE_COPY( MyTreeView )
};

/*! @} */

#endif
