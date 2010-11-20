/**
 * src/libs/Sorcerer/SorcererPlugin.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "SorcererPlugin.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <MainWindow.hpp>
#include <Version.hpp>

/* local headers */
#include "SorcererWidget.hpp"


SorcererPlugin::SorcererPlugin()
: mGlobal("Global")
, mFunkytown("Funkytown")
, mInnuendo("Innuendo")
, mKarmadrome("Karmadrome")
, mNotorious("Notorious")
, mPartyman("Partyman")
, mRubberbandman("Rubberbandman")
, mStripped("Stripped")
{
}


SorcererPlugin::~SorcererPlugin()
{
}


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
   if( mGlobal.value( "ShowCleanupDialog", false ).toBool() )
   {
      QMessageBox::information( 0, app->applicationName(),
                                QObject::tr("New version (%1 -> %2).\n"
                                            "Cleaning up registry.")
                                .arg(mGlobal.value("Version","none").toString(), SLART_VERSION) );
   }
   mGlobal.remove("UseGlobalStyleSheetFile");

   cleanupSettings( &mFunkytown );
   mFunkytown.remove( "flv2mpeg4" );

   cleanupSettings( &mInnuendo );
   mInnuendo.remove( "Next" );
   mInnuendo.remove( "Play" );
   mInnuendo.remove( "Stop" );

   cleanupSettings( &mKarmadrome );
   mKarmadrome.remove( "ButtonRows" );

   cleanupSettings( &mPartyman );
   mPartyman.remove( "DatabaseFilename" );

   cleanupSettings( &mRubberbandman );

   cleanupSettings( &mNotorious );

   cleanupSettings( &mStripped );
   QString  strippedEncoder     = mStripped.value( "Encoder", QString() ).toString();
   QVariant strippedOggQuality  = mStripped.value( "OggQuality" );
   QVariant strippedFlacQuality = mStripped.value( "FlacQuality" );
   QVariant strippedFlacUseOga  = mStripped.value( "FlacUseOga" );
   if( !strippedEncoder.isEmpty() )
   {
      mStripped.remove( "Encoder" );
      mStripped.beginGroup( strippedEncoder );
      mStripped.setValue( "UseEncoder", true );
      mStripped.endGroup();
   }
   mStripped.beginGroup( "ogg" );
   if( !strippedOggQuality.isNull() )
   {
      mStripped.setValue( "OggQuality", strippedOggQuality );
   }
   mStripped.endGroup();
   mStripped.beginGroup( "FLAC" );
   if( !strippedFlacQuality.isNull() )
   {
      mStripped.setValue( "FlacQuality", strippedFlacQuality );
   }
   if( !strippedFlacUseOga.isNull() )
   {
      mStripped.setValue( "FlacUseOga", strippedFlacUseOga );
   }
   mStripped.endGroup();
   mGlobal.setValue( "Version", SLART_VERSION );
}


void SorcererPlugin::hidden()
{
   /* show a message box when a cleanup has triggered a registry cleanup */
   setDefault( &mGlobal, "ShowCleanupDialog", false );

   /* Create "hidden" entries, that can not be set via dialogs */
   setDefault( &mFunkytown, "UserAgent", "Funkytown" );

   QStringList list;
   list << "*.mp3" << "*.ogg" << "*.flac" << "*.oga";
   setDefault( &mRubberbandman, "FileExtensions", list );
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


Q_EXPORT_PLUGIN2(SorcererInterface, SorcererPlugin)
