/*
 * src/libs/Common/MyLineEdit.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef MYLINEEDIT_HPP
#define MYLINEEDIT_HPP MYLINEEDIT_HPP

/* base class */
#include <QLineEdit>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QCompleter;
class QTimer;

/* forward declaration of local classes */


/*!
  \addtogroup Common
 @{
 */

/*!
 \brief variant of QLineEdit, that delays the pop-up of the auto completer

 \dotfile "graphs/libs/Common/LineEditDAC_connect.dot" "Connect Graph"
*/
class LineEditDAC : public QLineEdit
{
   Q_OBJECT

   /*!
    \brief delay for showing the completer as property
    */
   Q_PROPERTY( int delay
               READ delay
               WRITE setDelay
               )

public:
   /*!
    \brief constructor

    \param parent
   */
   explicit LineEditDAC( QWidget *parent = 0 );

   /*!
    \brief destructor

   */
   virtual ~LineEditDAC();

   /*!
    \brief intercept for delayed setting of completer

    \param completer
   */
   virtual void setCompleter( QCompleter *completer );

   /*!
    \brief intercept for starting timeout

    \param text
   */
   virtual void setText( const QString &text );

   /*!
    \brief return the delay of the completer

    \return int
   */
   int delay() const;

public slots:
   /*!
    \brief set the delay of the completer

    \param msec
   */
   void setDelay( int msec );

   /*!
    \brief set the intercepted completer after timeout

   */
   void setCompleter();

private:
   Q_DISABLE_COPY( LineEditDAC )

   QTimer      *mpTimer; /*!< \brief timer for delayed showing of completer */
   QCompleter  *mpCompleter; /*!< \brief the stored completer */
   int         mDelay; /*!< \brief delay for showing completer */
};
/*! @} */

#endif
