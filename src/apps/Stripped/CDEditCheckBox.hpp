/**
 * src/apps/Stripped/CDEditCheckBox.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef CDEDITCHECKBOX_HPP
#define CDEDITCHECKBOX_HPP CDEDITCHECKBOX_HPP

/* base class */
#include <QCheckBox>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */
class CDEdit;


class CDEditCheckBox : public QCheckBox
{
Q_OBJECT
public:
   CDEditCheckBox( CDEdit *parent );
   CDEditCheckBox( const QString &text, CDEdit *parent );
protected:
   void keyPressEvent( QKeyEvent *event );
private:
   CDEdit   *mpParent;
};

#endif
