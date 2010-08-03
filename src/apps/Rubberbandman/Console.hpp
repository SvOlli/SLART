/**
 * src/app/Rubberbandman/Console.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
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

public slots:
   void handleProgress( int checked, int processed );
   
private:
   Console( const Console &other );
   Console &operator=( const Console &other );

   QTextStream    mStdOut;
   QString        mCheckedText;
   QString        mProcessedText;
};

#endif