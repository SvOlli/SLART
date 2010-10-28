/**
 * src/apps/Rubberbandman/MyTreeView.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
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


class MyTreeView : public QTreeView
{
public:
   MyTreeView( QWidget *parent );
   virtual ~MyTreeView();

protected:
   /* handle return/enter key */
   virtual void keyPressEvent( QKeyEvent *event );
};
