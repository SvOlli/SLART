/**
 * src/apps/Stripped/CDEditLineEdit.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef CDEDITLINEEDIT_HPP
#define CDEDITLINEEDIT_HPP CDEDITLINEEDIT_HPP

/* base class */
#include <QLineEdit>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */
class CDEdit;


class CDEditLineEdit : public QLineEdit
{
Q_OBJECT
public:
   CDEditLineEdit( CDEdit *parent );
   CDEditLineEdit( const QString &text, CDEdit *parent );
protected:
   void keyPressEvent( QKeyEvent *event );
private:
   CDEdit   *mpParent;
};

#endif
