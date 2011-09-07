/**
 * src/app/Rubberbandman/Console.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef CONSOLE_HPP
#define CONSOLE_HPP CONSOLE_HPP

/* base class */
#include <QObject>

/* system headers */

/* Qt headers */
#include <QString>
#include <QTextStream>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


/*!
  \addtogroup Rubberbandman

  @{
*/

/*!
 \brief generating text console output for non gui operation

*/
class Console : public QObject
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param checkedText
    \param processedText
    \param parent
   */
   Console( const QString &checkedText,
            const QString &processedText,
            QObject *parent = 0 );

   /*!
    \brief destructor

   */
   virtual ~Console();

   /*!
    \brief 

   */
   void message( const QString &msg );

public slots:
   /*!
    \brief 

   */
   void handleProgress( int checked, int processed );

private:
   Q_DISABLE_COPY( Console )

   QTextStream    mStdOut;
   QString        mCheckedText;
   QString        mProcessedText;
};

/*! @} */

#endif
