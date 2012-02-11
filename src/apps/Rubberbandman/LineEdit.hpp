/*
 * src/libs/Common/LineEdit.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef LINEEDIT_HPP
#define LINEEDIT_HPP LINEEDIT_HPP

/* base class */
#include <QLineEdit>

/* system headers */

/* Qt headers */
#include <QString>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QMouseEvent;
class QTimer;

/* forward declaration of local classes */


/*!
  \addtogroup Rubberbandman

  @{
*/

/*!
 \brief TODO

*/
class LineEdit : public QLineEdit
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param parent
   */
   LineEdit( QWidget *parent );

   /*!
    \brief destructor

   */
   virtual ~LineEdit();

public slots:
   /*!
    \brief set texts for auto complete

   */
   void setCompleterTexts( const QStringList &list );
   /*!
    \brief add one text for auto complete

   */
   void addCompleterText( const QString &text = QString() );

protected:

private:
   Q_DISABLE_COPY( LineEdit )
};

/*! @} */

#endif
