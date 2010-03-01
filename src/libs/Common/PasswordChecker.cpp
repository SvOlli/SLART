/**
 * src/libs/Common/PasswordChecker.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* base class */
#include "PasswordChecker.hpp"

/* class declaration */

/* system headers */

/* Qt headers */
#include <QInputDialog>

/* local library headers */

/* local headers */

/* class variables */
PasswordChecker *PasswordChecker::gpPasswordChecker = 0;


PasswordChecker *PasswordChecker::get( QWidget *parent )
{
   if( !gpPasswordChecker )
   {
      gpPasswordChecker = new PasswordChecker( parent );
   }
   return gpPasswordChecker;
}


PasswordChecker::PasswordChecker( QWidget *parent )
: QObject( parent )
, mpParent( parent )
, mPassword()
, mLockHeader()
, mLockMessage()
, mUnlockHeader()
, mUnlockMessage()
{
}


PasswordChecker::~PasswordChecker()
{
   gpPasswordChecker = 0;
}


void PasswordChecker::setLockMessage( const QString &header,
                                      const QString &message )
{
   mLockHeader  = header;
   mLockMessage = message;
}


void PasswordChecker::setUnlockMessage( const QString &header,
                                        const QString &message )
{
   mUnlockHeader  = header;
   mUnlockMessage = message;
}


bool PasswordChecker::lock()
{
   /* check if we want to lock instead of verify */
   mPassword =
      QInputDialog::getText( mpParent, mLockHeader, mLockMessage,
                             QLineEdit::Password );
   bool enable = !mPassword.isEmpty();
   emit enabled( enable );
   return enable;
}


bool PasswordChecker::unlock()
{
   /* no password set -> no kiosk mode */
   if( mPassword.isEmpty() )
   {
      return true;
   }

   QString password( QInputDialog::getText( mpParent, mUnlockHeader, mUnlockMessage,
                                            QLineEdit::Password ) );

   if( mPassword == password )
   {
      mPassword.clear();
      return true;
   }
   else
   {
      return false;
   }
}
