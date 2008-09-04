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
   MySettings settings;
   
   QStringList applications( settings.value("Startup", QStringList()).toStringList() );
   if( isChecked() )
   {
      /* start */
      mTerminating = false;
      mProcess.start( mName );
      if( !applications.contains( text() ) )
      {
         applications.append( text() );
         settings.setValue("Startup", applications);
      }
   }
   else
   {
      /* stop */
      mTerminating = true;
      mProcess.terminate();
      if( applications.contains( text() ) )
      {
         applications.removeAt( applications.indexOf( text() ) );
         if( applications.count() > 0 )
         {
            settings.setValue( "Startup", applications );
         }
         else
         {
            settings.remove( "Startup" );
         }
      }
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
