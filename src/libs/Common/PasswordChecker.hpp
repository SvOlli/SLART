/**
 * src/libs/Common/PasswordChecker.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef PASSWORDCHECKER_HPP
#define PASSWORDCHECKER_HPP

#include <QObject>

#include <QString>


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
   /* bad workaround: will not be linked in when declared private */
   static PasswordChecker  *gpPasswordChecker;

signals:
   void enabled( bool enabled );

private:
   /*  */
   PasswordChecker();
   virtual ~PasswordChecker();
   PasswordChecker( const PasswordChecker &other );
   PasswordChecker &operator=( const PasswordChecker &other );

   QWidget                 *mpParent;
   QString                 mPassword;
   QString                 mLockHeader;
   QString                 mLockMessage;
   QString                 mUnlockHeader;
   QString                 mUnlockMessage;
};

#endif /* PASSWORDCHECKER_HPP */
