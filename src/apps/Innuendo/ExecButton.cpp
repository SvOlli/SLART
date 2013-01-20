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


ExecButton::ExecButton( const QString &name, QWidget *parent )
: QPushButton( name, parent )
, mName( QCoreApplication::applicationDirPath() )
, mpProcess( new QProcess( this ) )
, mTerminating( false )
{
   mName.append( '/' );
   mName.append( name );

   setCheckable( true );

   connect( this, SIGNAL(clicked()),
            this, SLOT(handleClick()) );
   connect( mpProcess, SIGNAL(error(QProcess::ProcessError)),
            this, SLOT(handleError(QProcess::ProcessError)) );
   connect( mpProcess, SIGNAL(finished(int,QProcess::ExitStatus)),
            this, SLOT(handleFinished(int,QProcess::ExitStatus)) );
}


ExecButton::~ExecButton()
{
   mpProcess->terminate();
   mpProcess->waitForFinished();
}


void ExecButton::handleClick()
{
   if( isChecked() )
   {
      /* start */
      mTerminating = false;
      mpProcess->start( mName );
   }
   else
   {
      /* stop */
      mTerminating = true;
      mpProcess->terminate();
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
