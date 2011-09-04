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


class Console : public QObject
{
   Q_OBJECT

public:
   Console( const QString &checkedText,
            const QString &processedText,
            QObject *parent = 0 );
   virtual ~Console();

   /*  */
   void message( const QString &msg );

public slots:
   /*  */
   void handleProgress( int checked, int processed );

private:
   Console( const Console &that );
   Console &operator=( const Console &that );

   QTextStream    mStdOut;
   QString        mCheckedText;
   QString        mProcessedText;
};

#endif
