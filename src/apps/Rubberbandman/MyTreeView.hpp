/**
 * src/apps/Rubberbandman/MyTreeView.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/*!
    \brief class declaration

   */
#include <QTreeView>

/* system headers */

/* Qt headers */
#include <QtGui>
#include <QString>

/* local library headers */
#include <Database.hpp>
#include <DirWalker.hpp>
#include <MySettings.hpp>
#include <Satellite.hpp>

/* local headers */
#include "ConfigDialog.hpp"


/*!
  \addtogroup Rubberbandman

  @{
*/

/*!
 \brief TODO

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

protected:
   /*!
    \brief handle return/enter key

   */
   virtual void keyPressEvent( QKeyEvent *event );
};

/*! @} */
