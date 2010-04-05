/**
 * src/libs/Sorcerer/SorcererPlugin.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "SorcererPlugin.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <MainWindow.hpp>
#include <MySettings.hpp>
#include <Version.hpp>

/* local headers */
#include "SorcererWidget.hpp"


int SorcererPlugin::setup( QApplication *app )
{
   MainWindow window( false );
   SorcererWidget *sorcererWidget = new SorcererWidget( &window );
   window.setMainWidget( sorcererWidget );
   if( app->applicationName() == "Sorcerer" )
   {
      window.changeTitle( QIcon(), app->applicationName() );
   }
   else
   {
      window.changeTitle( QIcon(),
                          QString("Sorcerer (running for %1)").arg(app->applicationName()) );
   }
   window.show();
   app->exec();

   int errors = sorcererWidget->errors();
   delete sorcererWidget;
   sorcererWidget = 0;
   return errors;
}


void SorcererPlugin::cleanup( QApplication *app )
{
   MySettings Global("Global");
   MySettings Funkytown("Funkytown");
   MySettings Innuendo("Innuendo");
   MySettings Karmadrome("Karmadrome");
   MySettings Partyman("Partyman");
   MySettings Rubberbandman("Rubberbandman");
   MySettings Stripped("Stripped");

   if( Global.value( "ShowCleanupDialog", false ).toBool() )
   {
      QMessageBox::information( 0, app->applicationName(),
                                QObject::tr("New version (%1 -> %2).\n"
                                            "Cleaning up registry.")
                                .arg(Global.value("Version","none").toString()), SLART_VERSION );
      Global.remove("UseGlobalStyleSheetFile");

      cleanupSettings( &Funkytown );
      Funkytown.remove( "flv2mpeg4" );

      cleanupSettings( &Innuendo );
      Innuendo.remove( "Next" );
      Innuendo.remove( "Play" );
      Innuendo.remove( "Stop" );

      cleanupSettings( &Karmadrome );
      Karmadrome.remove( "ButtonRows" );

      cleanupSettings( &Partyman );
      Partyman.remove( "DatabaseFilename" );

      cleanupSettings( &Rubberbandman );

      cleanupSettings( &Stripped );
      QString  Encoder     = Stripped.value( "Encoder", QString() ).toString();
      QVariant OggQuality  = Stripped.value( "OggQuality" );
      QVariant FlacQuality = Stripped.value( "FlacQuality" );
      QVariant FlacUseOga  = Stripped.value( "FlacUseOga" );
      if( !Encoder.isEmpty() )
      {
         Stripped.remove( "Encoder" );
         Stripped.beginGroup( Encoder );
         Stripped.setValue( "UseEncoder", true );
         Stripped.endGroup();
      }
      Stripped.beginGroup( "ogg" );
      if( !OggQuality.isNull() )
      {
         Stripped.setValue( "OggQuality", OggQuality );
      }
      Stripped.endGroup();
      Stripped.beginGroup( "FLAC" );
      if( !FlacQuality.isNull() )
      {
         Stripped.setValue( "FlacQuality", FlacQuality );
      }
      if( !FlacQuality.isNull() )
      {
         Stripped.setValue( "FlacQuality", FlacUseOga );
      }
      Stripped.endGroup();
   }
}


void SorcererPlugin::hidden()
{
   MySettings Global("Global");
   MySettings Funkytown("Funkytown");
   MySettings Innuendo("Innuendo");
   MySettings Karmadrome("Karmadrome");
   MySettings Partyman("Partyman");
   MySettings Rubberbandman("Rubberbandman");
   MySettings Stripped("Stripped");

   /* show a message box when a cleanup has triggered a registry cleanup */
   setDefault( &Global, "ShowCleanupDialog", false );

   /* Create "hidden" entries, that can not be set via dialogs */
   setDefault( &Funkytown, "UserAgent", "Funkytown" );

   QStringList list;
   list << "*.mp3" << "*.ogg" << "*.flac" << "*.oga";
   setDefault( &Rubberbandman, "FileExtensions", list );
}


void SorcererPlugin::cleanupSettings( MySettings *settings )
{
   if( settings )
   {
      settings->remove( "Listener" );
      settings->remove( "SLARTCommunication" );
      settings->remove( "StyleSheet" );
      settings->remove( "UDPListenerPort" );
   }
}


void SorcererPlugin::setDefault( MySettings *settings, const QString &name,
                                 const QVariant &value )
{
   if( !settings->contains( name ) )
   {
      settings->setValue( name, value );
   }
}


Q_EXPORT_PLUGIN2(Sorcerer, SorcererPlugin)
