/*
 * src/apps/Innuendo/ExecButton.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "ExecButton.hpp"

/* system headers */

/* Qt headers */
#include <QCoreApplication>
#include <QMessageBox>

/* local library headers */

/* local headers */
#include "ConfigDialog.hpp"


ExecButton::ExecButton( const QString &name, QWidget *parent )
: QPushButton( name, parent )
, mName( QCoreApplication::applicationDirPath() )
, mProcess()
, mTerminating( false )
{
   mName.append( '/' );
   mName.append( name );

   setCheckable( true );

   connect( this, SIGNAL(clicked()),
            this, SLOT(handleClick()) );
   connect( &mProcess, SIGNAL(error(QProcess::ProcessError)),
            this, SLOT(handleError(QProcess::ProcessError)) );
   connect( &mProcess, SIGNAL(finished(int,QProcess::ExitStatus)),
            this, SLOT(handleFinished(int,QProcess::ExitStatus)) );
}


ExecButton::~ExecButton()
{
   disconnect( this, SIGNAL(clicked()),
               this, SLOT(handleClick()) );
   disconnect( &mProcess, SIGNAL(error(QProcess::ProcessError)),
               this, SLOT(handleError(QProcess::ProcessError)) );
   disconnect( &mProcess, SIGNAL(finished(int,QProcess::ExitStatus)),
               this, SLOT(handleFinished(int,QProcess::ExitStatus)) );
   mProcess.terminate();
   mProcess.waitForFinished();
}


void ExecButton::handleClick()
{
   if( isChecked() )
   {
      /* start */
      mTerminating = false;
      mProcess.start( mName );
   }
   else
   {
      /* stop */
      mTerminating = true;
      mProcess.terminate();
   }
}


void ExecButton::handleError( QProcess::ProcessError error )
{
   QString text;

   switch( error )
   {
      case QProcess::FailedToStart:
         text.append( tr("Could not start %1").arg( mName ) );
         break;
      case QProcess::Crashed:
         if( mTerminating ) return;
         text.append( tr("Crashed application %1").arg( mName ) );
         break;
      default:
         text.append( tr("Error running %1").arg( mName ) );
         break;
   }

   QMessageBox::warning( this, QCoreApplication::applicationName(), text );
   setChecked( false );
}


void ExecButton::handleFinished( int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/ )
{
   setChecked( false );
}
