/**
 * src/libs/Common/PasswordChecker.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef PASSWORDCHECKER_HPP
#define PASSWORDCHECKER_HPP

/* base class */
#include <QObject>

/* system headers */

/* Qt headers */
#include <QString>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


class PasswordChecker : public QObject
{
Q_OBJECT

public:
   /*  */
   static PasswordChecker *get( QWidget *parent = 0 );
   /*  */
   void setLockMessage( const QString &header, const QString &message );
   /*  */
   void setUnlockMessage( const QString &header, const QString &message );
   /*  */
   bool lock();
   /*  */
   bool unlock();

signals:
   void enabled( bool enabled );

private:
   /*  */
   PasswordChecker( QWidget *parent = 0 );
   virtual ~PasswordChecker();
   PasswordChecker( const PasswordChecker &other );
   PasswordChecker &operator=( const PasswordChecker &other );

   /*  */
   static PasswordChecker  *gpPasswordChecker;

   QWidget                 *mpParent;
   QString                 mPassword;
   QString                 mLockHeader;
   QString                 mLockMessage;
   QString                 mUnlockHeader;
   QString                 mUnlockMessage;
};

#endif /* PASSWORDCHECKER_HPP */
