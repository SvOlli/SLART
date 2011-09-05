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


/*!
  \addtogroup Common

  @{
*/

/*!
 \brief TODO

*/
class PasswordChecker : public QObject
{
   Q_OBJECT

public:
   /*!
    \brief get the singleton of the PasswordChecker

   */
   static PasswordChecker *get( QWidget *parent = 0 );
   /*!
    \brief set the message that will be displayed during locking

   */
   void setLockMessage( const QString &header, const QString &message );
   /*!
    \brief set the message that will be displayed during unlocking

   */
   void setUnlockMessage( const QString &header, const QString &message );
   /*!
    \brief display dialog for locking

   */
   bool lock();
   /*!
    \brief display dialog for unlocking

   */
   bool unlock();

signals:
   void enabled( bool enabled );

private:
   /*!
    \brief singleton: constructor is private

   */
   PasswordChecker( QWidget *parent = 0 );
   virtual ~PasswordChecker();
   PasswordChecker( const PasswordChecker &that );
   PasswordChecker &operator=( const PasswordChecker &that );

   /*!
    \brief singleton: only instance of PasswordChecker

   */
   static PasswordChecker  *cpPasswordChecker;

   QWidget                 *mpParent;
   QString                 mPassword;
   QString                 mLockHeader;
   QString                 mLockMessage;
   QString                 mUnlockHeader;
   QString                 mUnlockMessage;
};

/*! @} */

#endif
