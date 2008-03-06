/**
 * ExecButton.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "ExecButton.hpp"

#include <QtGui>
#include "MySettings.hpp"
#include "ConfigDialog.hpp"
#include "MyClipboard.hpp"

#include "Trace.hpp"


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
         text.append( tr("Could not start ") );
         break;
      case QProcess::Crashed:
         if( mTerminating ) return;
         text.append( tr("Crashed application ") );
         break;
      default:
         text.append( tr("Error running ") );
         break;
   }
   text.append( mName );
   
   QMessageBox::warning( this, QCoreApplication::applicationName(), text );
   setChecked( false );
}


void ExecButton::handleFinished( int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/ )
{
   setChecked( false );
}
