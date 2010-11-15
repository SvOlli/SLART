/**
 * src/apps/Karmadrome/ButtonsWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef BUTTONSWIDGET_HPP
#define BUTTONSWIDGET_HPP BUTTONSWIDGET_HPP

/* base class */
#include <QGroupBox>

/* system headers */

/* Qt headers */
#include <QList>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QAbstractButton;
class QGridLayout;
class QSignalMapper;

/* forward declaration of local classes */


class ButtonsWidget : public QGroupBox
{
Q_OBJECT
   
public:
   ButtonsWidget( QWidget *parent = 0 );
   ButtonsWidget( const QString &title, QWidget *parent = 0 );
   
   /* update the button list: replace all existing buttons */
   void updateButtons( const QStringList &list );
   /* set buttons checked state */
   void lockButtons( const QStringList &list );
   
signals:
   /* signalize the widget clicked on */
   void clicked( QWidget *widget );
   
private:
   ButtonsWidget( const ButtonsWidget &that );
   ButtonsWidget &operator=( const ButtonsWidget &that );
   
   QGridLayout             *mpMainLayout;
   QSignalMapper           *mpSignalMapper;
   QList<QAbstractButton*> mButtonList;
};

#endif
