/*
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


/*!
  \addtogroup Karmadrome

  @{
*/

/*!
 \brief a group box holding a dynamic amount of buttons

 \dotfile "graphs/apps/Karmadrome/ButtonsWidget_connect.dot" "Connect Graph"
*/
class ButtonsWidget : public QGroupBox
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param parent
   */
   ButtonsWidget( QWidget *parent = 0 );
   /*!
    \brief construtor

    \param title title of the group box
    \param parent
   */
   ButtonsWidget( const QString &title, QWidget *parent = 0 );

public slots:
   /*!
    \brief update the button list: replace all existing buttons

    \param list
   */
   void updateButtons( const QStringList &list );
   /*!
    \brief set buttons checked state

    \param list
   */
   void lockButtons( const QStringList &list );

signals:
   /*!
    \brief signalize the widget clicked on

    \param widget
   */
   void clicked( QWidget *widget );

private:
   Q_DISABLE_COPY( ButtonsWidget )

   QGridLayout             *mpMainLayout; /*!< \brief layout of buttons */
   QSignalMapper           *mpSignalMapper; /*!< \brief used to match buttons */
   QList<QAbstractButton*> mButtonList; /*!< \brief list of all buttons */
};

/*! @} */

#endif
