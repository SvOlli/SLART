/*
 * src/apps/Stripped/CDEditCheckBox.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
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


/*!
  \addtogroup Stripped
  @{
  */

/*!
 \brief helper for CDEdit providing a checkbox that can be left with cursor up / down

*/
class CDEditCheckBox : public QCheckBox
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param parent
   */
   CDEditCheckBox( CDEdit *parent );

   /*!
    \brief constructor that allow setting a text

    \param text
    \param parent
   */
   CDEditCheckBox( const QString &text, CDEdit *parent );

protected:
   /*!
    \brief Reimplemented from QCheckBox::keyPressEvent()

    Handling of cursor up / down keys

    \param event
   */
   void keyPressEvent( QKeyEvent *event );

private:
   CDEdit   *mpParent; /*!< \brief \todo complete documentation */
};

/*! @} */

#endif
