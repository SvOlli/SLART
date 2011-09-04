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


/*!
  \addtogroup Stripped
  @{
  */

/*!
 \brief helper for CDEdit providing a line edit that can be left with cursor up / down

*/
class CDEditLineEdit : public QLineEdit
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param parent
   */
   CDEditLineEdit( CDEdit *parent );

   /*!
    \brief constructor that allow setting a text

    \param text
    \param parent
   */
   CDEditLineEdit( const QString &text, CDEdit *parent );

protected:
   /*!
    \brief Reimplemented from QLineEdit::keyPressEvent()

    Handling of cursor up / down keys

    \param event
   */
   void keyPressEvent( QKeyEvent *event );

private:
   CDEdit   *mpParent; /*!< TODO */
};

/*! @} */

#endif
