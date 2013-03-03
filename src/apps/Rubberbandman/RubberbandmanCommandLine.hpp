/*
 * src/app/Rubberbandman/RubberbandmanCommandLine.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef RUBBERBANDMANCOMMANDLINE_HPP
#define RUBBERBANDMANCOMMANDLINE_HPP RUBBERBANDMANCOMMANDLINE_HPP

/* base class */
#include <QObject>

/* system headers */

/* Qt headers */
#include <QStringList>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */
class Console;
class DatabaseWorker;


/*!
  \addtogroup Rubberbandman

  @{
*/

/*!
 \brief generating text console output for non gui operation

*/
class RubberbandmanCommandLine : public QObject
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param parent
   */
   RubberbandmanCommandLine( const QStringList &args, QObject *parent = 0 );

   /*!
    \brief destructor

   */
   virtual ~RubberbandmanCommandLine();

public slots:
   /*!
    \brief run next job

   */
   void nextJob();

   /*!
    \brief print out a message that the job was completed

   */
   void jobDone();

signals:
   /*!
    \brief done with all jobs

   */
   void done();

private:
   Q_DISABLE_COPY( RubberbandmanCommandLine )

   DatabaseWorker    *mpDatabaseWorker;
   Console           *mpConsole;
   QString           mBaseDir;
   QStringList       mArgs;
};

/*! @} */

#endif
