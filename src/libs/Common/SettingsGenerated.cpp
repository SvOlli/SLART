#include "Settings.hpp"
#include <QApplication>
#include <QDir>
#include <QPoint>
#include <QSize>


void Settings::setValue( CommonPoint id, const QPoint &value )
{
   MySettings *settings = cpSettings->get();
   switch( id )
   {
   case CommonMainWindowPosition:
      settings->setValue( "MainWindowPosition", value );
      break;
   default:
      qFatal( "illegal CommonPoint value" );
   }
   settings->endGroup();
}


QPoint Settings::value( CommonPoint id )
{
   QPoint retval;
   MySettings *settings = cpSettings->get();
   switch( id )
   {
   case CommonMainWindowPosition:
      retval = settings->value( "MainWindowPosition" ).toPoint();
      break;
   default:
      qFatal( "illegal CommonPoint value" );
   }
   return retval;
}


void Settings::setValue( CommonSize id, const QSize &value )
{
   MySettings *settings = cpSettings->get();
   switch( id )
   {
   case CommonMainWindowSize:
      settings->setValue( "MainWindowSize", value );
      break;
   default:
      qFatal( "illegal CommonSize value" );
   }
   settings->endGroup();
}


QSize Settings::value( CommonSize id )
{
   QSize retval;
   MySettings *settings = cpSettings->get();
   switch( id )
   {
   case CommonMainWindowSize:
      retval = settings->value( "MainWindowSize" ).toSize();
      break;
   default:
      qFatal( "illegal CommonSize value" );
   }
   return retval;
}


void Settings::setValue( CommonString id, const QString &value )
{
   MySettings *settings = cpSettings->get();
   switch( id )
   {
   case CommonStyleSheetFile:
      settings->setValue( "StyleSheetFile", value );
      break;
   default:
      qFatal( "illegal CommonString value" );
   }
   settings->endGroup();
}


QString Settings::value( CommonString id )
{
   QString retval;
   MySettings *settings = cpSettings->get();
   switch( id )
   {
   case CommonStyleSheetFile:
      retval = settings->value( "StyleSheetFile" ).toString();
      break;
   default:
      qFatal( "illegal CommonString value" );
   }
   return retval;
}


void Settings::setValue( CommonBool id, bool value )
{
   MySettings *settings = cpSettings->get();
   switch( id )
   {
   case CommonUseGlobalStyleSheetFile:
      settings->setValue( "UseGlobalStyleSheetFile", value );
      break;
   case CommonUseSatellite:
      settings->setValue( "UseSatellite", value );
      break;
   default:
      qFatal( "illegal CommonBool value" );
   }
   settings->endGroup();
}


bool Settings::value( CommonBool id )
{
   bool retval;
   MySettings *settings = cpSettings->get();
   switch( id )
   {
   case CommonUseGlobalStyleSheetFile:
      retval = settings->value( "UseGlobalStyleSheetFile" ).toBool();
      break;
   case CommonUseSatellite:
      retval = settings->value( "UseSatellite", false ).toBool();
      break;
   default:
      qFatal( "illegal CommonBool value" );
   }
   return retval;
}


void Settings::setValue( MagicFLACString id, const QString &value )
{
   MySettings *settings = cpSettings->get();
   settings->beginGroup( "FLAC" );
   switch( id )
   {
   case MagicFLACDirectory:
      settings->setValue( "Directory", value );
      break;
   default:
      qFatal( "illegal MagicFLACString value" );
   }
   settings->endGroup();
}


QString Settings::value( MagicFLACString id )
{
   QString retval;
   MySettings *settings = cpSettings->get();
   switch( id )
   {
   case MagicFLACDirectory:
      retval = settings->value( "Directory", QDir::current().absolutePath() ).toString();
      break;
   default:
      qFatal( "illegal MagicFLACString value" );
   }
   settings->endGroup();
   return retval;
}


void Settings::setValue( MagicFLACBool id, bool value )
{
   MySettings *settings = cpSettings->get();
   settings->beginGroup( "FLAC" );
   switch( id )
   {
   case MagicFLACDirectoryOverride:
      settings->setValue( "DirectoryOverride", value );
      break;
   case MagicFLACFlacUseOga:
      settings->setValue( "FlacUseOga", value );
      break;
   case MagicFLACUseEncoder:
      settings->setValue( "UseEncoder", value );
      break;
   default:
      qFatal( "illegal MagicFLACBool value" );
   }
   settings->endGroup();
}


bool Settings::value( MagicFLACBool id )
{
   bool retval;
   MySettings *settings = cpSettings->get();
   switch( id )
   {
   case MagicFLACDirectoryOverride:
      retval = settings->value( "DirectoryOverride", false ).toBool();
      break;
   case MagicFLACFlacUseOga:
      retval = settings->value( "FlacUseOga", false ).toBool();
      break;
   case MagicFLACUseEncoder:
      retval = settings->value( "UseEncoder", false ).toBool();
      break;
   default:
      qFatal( "illegal MagicFLACBool value" );
   }
   settings->endGroup();
   return retval;
}


void Settings::setValue( MagicFLACInt id, int value )
{
   MySettings *settings = cpSettings->get();
   settings->beginGroup( "FLAC" );
   switch( id )
   {
   case MagicFLACFlacQuality:
      settings->setValue( "FlacQuality", value );
      break;
   default:
      qFatal( "illegal MagicFLACInt value" );
   }
   settings->endGroup();
}


int Settings::value( MagicFLACInt id )
{
   int retval;
   MySettings *settings = cpSettings->get();
   switch( id )
   {
   case MagicFLACFlacQuality:
      retval = settings->value( "FlacQuality", 5 ).toInt();
      break;
   default:
      qFatal( "illegal MagicFLACInt value" );
   }
   settings->endGroup();
   return retval;
}


void Settings::setValue( Magicmp3String id, const QString &value )
{
   MySettings *settings = cpSettings->get();
   settings->beginGroup( "mp3" );
   switch( id )
   {
   case Magicmp3Directory:
      settings->setValue( "Directory", value );
      break;
   default:
      qFatal( "illegal Magicmp3String value" );
   }
   settings->endGroup();
}


QString Settings::value( Magicmp3String id )
{
   QString retval;
   MySettings *settings = cpSettings->get();
   switch( id )
   {
   case Magicmp3Directory:
      retval = settings->value( "Directory", QDir::current().absolutePath() ).toString();
      break;
   default:
      qFatal( "illegal Magicmp3String value" );
   }
   settings->endGroup();
   return retval;
}


void Settings::setValue( Magicmp3Bool id, bool value )
{
   MySettings *settings = cpSettings->get();
   settings->beginGroup( "mp3" );
   switch( id )
   {
   case Magicmp3DirectoryOverride:
      settings->setValue( "DirectoryOverride", value );
      break;
   case Magicmp3UseEncoder:
      settings->setValue( "UseEncoder", value );
      break;
   case Magicmp3UseLatin1:
      settings->setValue( "UseLatin1", value );
      break;
   case Magicmp3VBRQuality:
      settings->setValue( "VBRQuality", value );
      break;
   default:
      qFatal( "illegal Magicmp3Bool value" );
   }
   settings->endGroup();
}


bool Settings::value( Magicmp3Bool id )
{
   bool retval;
   MySettings *settings = cpSettings->get();
   switch( id )
   {
   case Magicmp3DirectoryOverride:
      retval = settings->value( "DirectoryOverride", false ).toBool();
      break;
   case Magicmp3UseEncoder:
      retval = settings->value( "UseEncoder", false ).toBool();
      break;
   case Magicmp3UseLatin1:
      retval = settings->value( "UseLatin1", false ).toBool();
      break;
   case Magicmp3VBRQuality:
      retval = settings->value( "VBRQuality", 4.0 ).toBool();
      break;
   default:
      qFatal( "illegal Magicmp3Bool value" );
   }
   settings->endGroup();
   return retval;
}


void Settings::setValue( MagicoggString id, const QString &value )
{
   MySettings *settings = cpSettings->get();
   settings->beginGroup( "ogg" );
   switch( id )
   {
   case MagicoggDirectory:
      settings->setValue( "Directory", value );
      break;
   default:
      qFatal( "illegal MagicoggString value" );
   }
   settings->endGroup();
}


QString Settings::value( MagicoggString id )
{
   QString retval;
   MySettings *settings = cpSettings->get();
   switch( id )
   {
   case MagicoggDirectory:
      retval = settings->value( "Directory", QDir::current().absolutePath() ).toString();
      break;
   default:
      qFatal( "illegal MagicoggString value" );
   }
   settings->endGroup();
   return retval;
}


void Settings::setValue( MagicoggBool id, bool value )
{
   MySettings *settings = cpSettings->get();
   settings->beginGroup( "ogg" );
   switch( id )
   {
   case MagicoggDirectoryOverride:
      settings->setValue( "DirectoryOverride", value );
      break;
   case MagicoggUseEncoder:
      settings->setValue( "UseEncoder", value );
      break;
   default:
      qFatal( "illegal MagicoggBool value" );
   }
   settings->endGroup();
}


bool Settings::value( MagicoggBool id )
{
   bool retval;
   MySettings *settings = cpSettings->get();
   switch( id )
   {
   case MagicoggDirectoryOverride:
      retval = settings->value( "DirectoryOverride", false ).toBool();
      break;
   case MagicoggUseEncoder:
      retval = settings->value( "UseEncoder", false ).toBool();
      break;
   default:
      qFatal( "illegal MagicoggBool value" );
   }
   settings->endGroup();
   return retval;
}


void Settings::setValue( MagicoggDouble id, double value )
{
   MySettings *settings = cpSettings->get();
   settings->beginGroup( "ogg" );
   switch( id )
   {
   case MagicoggOggQuality:
      settings->setValue( "OggQuality", value );
      break;
   default:
      qFatal( "illegal MagicoggDouble value" );
   }
   settings->endGroup();
}


double Settings::value( MagicoggDouble id )
{
   double retval;
   MySettings *settings = cpSettings->get();
   switch( id )
   {
   case MagicoggOggQuality:
      retval = settings->value( "OggQuality", 0.4 ).toDouble();
      break;
   default:
      qFatal( "illegal MagicoggDouble value" );
   }
   settings->endGroup();
   return retval;
}


void Settings::setValue( MagicwavString id, const QString &value )
{
   MySettings *settings = cpSettings->get();
   settings->beginGroup( "wav" );
   switch( id )
   {
   case MagicwavDirectory:
      settings->setValue( "Directory", value );
      break;
   default:
      qFatal( "illegal MagicwavString value" );
   }
   settings->endGroup();
}


QString Settings::value( MagicwavString id )
{
   QString retval;
   MySettings *settings = cpSettings->get();
   switch( id )
   {
   case MagicwavDirectory:
      retval = settings->value( "Directory", QDir::current().absolutePath() ).toString();
      break;
   default:
      qFatal( "illegal MagicwavString value" );
   }
   settings->endGroup();
   return retval;
}


void Settings::setValue( MagicwavBool id, bool value )
{
   MySettings *settings = cpSettings->get();
   settings->beginGroup( "wav" );
   switch( id )
   {
   case MagicwavDirectoryOverride:
      settings->setValue( "DirectoryOverride", value );
      break;
   case MagicwavUseEncoder:
      settings->setValue( "UseEncoder", value );
      break;
   default:
      qFatal( "illegal MagicwavBool value" );
   }
   settings->endGroup();
}


bool Settings::value( MagicwavBool id )
{
   bool retval;
   MySettings *settings = cpSettings->get();
   switch( id )
   {
   case MagicwavDirectoryOverride:
      retval = settings->value( "DirectoryOverride", false ).toBool();
      break;
   case MagicwavUseEncoder:
      retval = settings->value( "UseEncoder", false ).toBool();
      break;
   default:
      qFatal( "illegal MagicwavBool value" );
   }
   settings->endGroup();
   return retval;
}


void Settings::setValue( FunkytownString id, const QString &value )
{
   MySettings *settings = cpSettings->get( "Funkytown" );
   switch( id )
   {
   case FunkytownUserAgent:
      settings->setValue( "UserAgent", value );
      break;
   default:
      qFatal( "illegal FunkytownString value" );
   }
   settings->endGroup();
}


QString Settings::value( FunkytownString id )
{
   QString retval;
   MySettings *settings = cpSettings->get( "Funkytown" );
   switch( id )
   {
   case FunkytownUserAgent:
      retval = settings->value( "UserAgent", "Shockwave Flash" ).toString();
      break;
   default:
      qFatal( "illegal FunkytownString value" );
   }
   return retval;
}


void Settings::setValue( FunkytownBool id, bool value )
{
   MySettings *settings = cpSettings->get( "Funkytown" );
   switch( id )
   {
   case FunkytownCoverArt:
      settings->setValue( "CoverArt", value );
      break;
   case FunkytownOverwrite:
      settings->setValue( "Overwrite", value );
      break;
   case FunkytownTollKeep:
      settings->setValue( "TollKeep", value );
      break;
   default:
      qFatal( "illegal FunkytownBool value" );
   }
   settings->endGroup();
}


bool Settings::value( FunkytownBool id )
{
   bool retval;
   MySettings *settings = cpSettings->get( "Funkytown" );
   switch( id )
   {
   case FunkytownCoverArt:
      retval = settings->value( "CoverArt", false ).toBool();
      break;
   case FunkytownOverwrite:
      retval = settings->value( "Overwrite", false ).toBool();
      break;
   case FunkytownTollKeep:
      retval = settings->value( "TollKeep", true ).toBool();
      break;
   default:
      qFatal( "illegal FunkytownBool value" );
   }
   return retval;
}


void Settings::setValue( FunkytownUlonglong id, qulonglong value )
{
   MySettings *settings = cpSettings->get( "Funkytown" );
   switch( id )
   {
   case FunkytownBytes:
      settings->setValue( "Bytes", value );
      break;
   default:
      qFatal( "illegal FunkytownUlonglong value" );
   }
   settings->endGroup();
}


qulonglong Settings::value( FunkytownUlonglong id )
{
   qulonglong retval;
   MySettings *settings = cpSettings->get( "Funkytown" );
   switch( id )
   {
   case FunkytownBytes:
      retval = settings->value( "Bytes", 0 ).toULongLong();
      break;
   default:
      qFatal( "illegal FunkytownUlonglong value" );
   }
   return retval;
}


void Settings::setValue( FunkytownUint id, uint value )
{
   MySettings *settings = cpSettings->get( "Funkytown" );
   switch( id )
   {
   case FunkytownFiles:
      settings->setValue( "Files", value );
      break;
   default:
      qFatal( "illegal FunkytownUint value" );
   }
   settings->endGroup();
}


uint Settings::value( FunkytownUint id )
{
   uint retval;
   MySettings *settings = cpSettings->get( "Funkytown" );
   switch( id )
   {
   case FunkytownFiles:
      retval = settings->value( "Files", 0 ).toUInt();
      break;
   default:
      qFatal( "illegal FunkytownUint value" );
   }
   return retval;
}


void Settings::setValue( GlobalString id, const QString &value )
{
   MySettings *settings = cpSettings->get( "Global" );
   switch( id )
   {
   case GlobalMusicBase:
      settings->setValue( "MusicBase", value );
      break;
   case GlobalSatelliteHost:
      settings->setValue( "SatelliteHost", value );
      break;
   case GlobalStyleSheetFile:
      settings->setValue( "StyleSheetFile", value );
      break;
   case GlobalVersion:
      settings->setValue( "Version", value );
      break;
   default:
      qFatal( "illegal GlobalString value" );
   }
   settings->endGroup();
}


QString Settings::value( GlobalString id )
{
   QString retval;
   MySettings *settings = cpSettings->get( "Global" );
   switch( id )
   {
   case GlobalMusicBase:
      retval = settings->value( "MusicBase", "/" ).toString();
      break;
   case GlobalSatelliteHost:
      retval = settings->value( "SatelliteHost", "127.0.0.1" ).toString();
      break;
   case GlobalStyleSheetFile:
      retval = settings->value( "StyleSheetFile" ).toString();
      break;
   case GlobalVersion:
      retval = settings->value( "Version" ).toString();
      break;
   default:
      qFatal( "illegal GlobalString value" );
   }
   return retval;
}


void Settings::setValue( GlobalBool id, bool value )
{
   MySettings *settings = cpSettings->get( "Global" );
   switch( id )
   {
   case GlobalAnimateViews:
      settings->setValue( "AnimateViews", value );
      break;
   case GlobalNormalizeCase:
      settings->setValue( "NormalizeCase", value );
      break;
   case GlobalNormalizeSpaces:
      settings->setValue( "NormalizeSpaces", value );
      break;
   case GlobalShowCleanupDialog:
      settings->setValue( "ShowCleanupDialog", value );
      break;
   case GlobalUseSatellite:
      settings->setValue( "UseSatellite", value );
      break;
   default:
      qFatal( "illegal GlobalBool value" );
   }
   settings->endGroup();
}


bool Settings::value( GlobalBool id )
{
   bool retval;
   MySettings *settings = cpSettings->get( "Global" );
   switch( id )
   {
   case GlobalAnimateViews:
      retval = settings->value( "AnimateViews", false ).toBool();
      break;
   case GlobalNormalizeCase:
      retval = settings->value( "NormalizeCase", false ).toBool();
      break;
   case GlobalNormalizeSpaces:
      retval = settings->value( "NormalizeSpaces", false ).toBool();
      break;
   case GlobalShowCleanupDialog:
      retval = settings->value( "ShowCleanupDialog" ).toBool();
      break;
   case GlobalUseSatellite:
      retval = settings->value( "UseSatellite", false ).toBool();
      break;
   default:
      qFatal( "illegal GlobalBool value" );
   }
   return retval;
}


void Settings::setValue( GlobalInt id, int value )
{
   MySettings *settings = cpSettings->get( "Global" );
   switch( id )
   {
   case GlobalClipboardMode:
      settings->setValue( "ClipboardMode", value );
      break;
   case GlobalDoubleClickInterval:
      settings->setValue( "DoubleClickInterval", value );
      break;
   case GlobalSatellitePort:
      settings->setValue( "SatellitePort", value );
      break;
   default:
      qFatal( "illegal GlobalInt value" );
   }
   settings->endGroup();
}


int Settings::value( GlobalInt id )
{
   int retval;
   MySettings *settings = cpSettings->get( "Global" );
   switch( id )
   {
   case GlobalClipboardMode:
      retval = settings->value( "ClipboardMode", 0 ).toInt();
      break;
   case GlobalDoubleClickInterval:
      retval = settings->value( "DoubleClickInterval", QApplication::doubleClickInterval() ).toInt();
      break;
   case GlobalSatellitePort:
      retval = settings->value( "SatellitePort", 24222 ).toInt();
      break;
   default:
      qFatal( "illegal GlobalInt value" );
   }
   return retval;
}


void Settings::setValue( GlobalHTTPProxyString id, const QString &value )
{
   MySettings *settings = cpSettings->get( "Global" );
   settings->beginGroup( "HTTPProxy" );
   switch( id )
   {
   case GlobalHTTPProxyHost:
      settings->setValue( "Host", value );
      break;
   case GlobalHTTPProxyLogin:
      settings->setValue( "Login", value );
      break;
   case GlobalHTTPProxyPassword:
      settings->setValue( "Password", value );
      break;
   case GlobalHTTPProxyPort:
      settings->setValue( "Port", value );
      break;
   default:
      qFatal( "illegal GlobalHTTPProxyString value" );
   }
   settings->endGroup();
}


QString Settings::value( GlobalHTTPProxyString id )
{
   QString retval;
   MySettings *settings = cpSettings->get( "Global" );
   switch( id )
   {
   case GlobalHTTPProxyHost:
      retval = settings->value( "Host" ).toString();
      break;
   case GlobalHTTPProxyLogin:
      retval = settings->value( "Login" ).toString();
      break;
   case GlobalHTTPProxyPassword:
      retval = settings->value( "Password" ).toString();
      break;
   case GlobalHTTPProxyPort:
      retval = settings->value( "Port", "8080" ).toString();
      break;
   default:
      qFatal( "illegal GlobalHTTPProxyString value" );
   }
   settings->endGroup();
   return retval;
}


void Settings::setValue( GlobalHTTPProxyBool id, bool value )
{
   MySettings *settings = cpSettings->get( "Global" );
   settings->beginGroup( "HTTPProxy" );
   switch( id )
   {
   case GlobalHTTPProxyAuth:
      settings->setValue( "Auth", value );
      break;
   case GlobalHTTPProxyEnable:
      settings->setValue( "Enable", value );
      break;
   default:
      qFatal( "illegal GlobalHTTPProxyBool value" );
   }
   settings->endGroup();
}


bool Settings::value( GlobalHTTPProxyBool id )
{
   bool retval;
   MySettings *settings = cpSettings->get( "Global" );
   switch( id )
   {
   case GlobalHTTPProxyAuth:
      retval = settings->value( "Auth", false ).toBool();
      break;
   case GlobalHTTPProxyEnable:
      retval = settings->value( "Enable", false ).toBool();
      break;
   default:
      qFatal( "illegal GlobalHTTPProxyBool value" );
   }
   settings->endGroup();
   return retval;
}


void Settings::setValue( InnuendoStringList id, const QStringList &value )
{
   MySettings *settings = cpSettings->get( "Innuendo" );
   switch( id )
   {
   case InnuendoStartup:
      settings->setValue( "Startup", value );
      break;
   default:
      qFatal( "illegal InnuendoStringList value" );
   }
   settings->endGroup();
}


QStringList Settings::value( InnuendoStringList id )
{
   QStringList retval;
   MySettings *settings = cpSettings->get( "Innuendo" );
   switch( id )
   {
   case InnuendoStartup:
      retval = settings->value( "Startup" ).toStringList();
      break;
   default:
      qFatal( "illegal InnuendoStringList value" );
   }
   return retval;
}


void Settings::setValue( InnuendoInt id, int value )
{
   MySettings *settings = cpSettings->get( "Innuendo" );
   switch( id )
   {
   case InnuendoBufferSize:
      settings->setValue( "BufferSize", value );
      break;
   default:
      qFatal( "illegal InnuendoInt value" );
   }
   settings->endGroup();
}


int Settings::value( InnuendoInt id )
{
   int retval;
   MySettings *settings = cpSettings->get( "Innuendo" );
   switch( id )
   {
   case InnuendoBufferSize:
      retval = settings->value( "BufferSize", 500 ).toInt();
      break;
   default:
      qFatal( "illegal InnuendoInt value" );
   }
   return retval;
}


void Settings::setValue( KarmadromeString id, const QString &value )
{
   MySettings *settings = cpSettings->get( "Karmadrome" );
   switch( id )
   {
   case KarmadromeExportDirectory:
      settings->setValue( "ExportDirectory", value );
      break;
   case KarmadromeImportDirectory:
      settings->setValue( "ImportDirectory", value );
      break;
   default:
      qFatal( "illegal KarmadromeString value" );
   }
   settings->endGroup();
}


QString Settings::value( KarmadromeString id )
{
   QString retval;
   MySettings *settings = cpSettings->get( "Karmadrome" );
   switch( id )
   {
   case KarmadromeExportDirectory:
      retval = settings->value( "ExportDirectory" ).toString();
      break;
   case KarmadromeImportDirectory:
      retval = settings->value( "ImportDirectory" ).toString();
      break;
   default:
      qFatal( "illegal KarmadromeString value" );
   }
   return retval;
}


void Settings::setValue( KarmadromeBool id, bool value )
{
   MySettings *settings = cpSettings->get( "Karmadrome" );
   switch( id )
   {
   case KarmadromeClearBeforeImport:
      settings->setValue( "ClearBeforeImport", value );
      break;
   case KarmadromeExportAsRelative:
      settings->setValue( "ExportAsRelative", value );
      break;
   case KarmadromeRandomizeExport:
      settings->setValue( "RandomizeExport", value );
      break;
   case KarmadromeUseCheckBoxes:
      settings->setValue( "UseCheckBoxes", value );
      break;
   default:
      qFatal( "illegal KarmadromeBool value" );
   }
   settings->endGroup();
}


bool Settings::value( KarmadromeBool id )
{
   bool retval;
   MySettings *settings = cpSettings->get( "Karmadrome" );
   switch( id )
   {
   case KarmadromeClearBeforeImport:
      retval = settings->value( "ClearBeforeImport", false ).toBool();
      break;
   case KarmadromeExportAsRelative:
      retval = settings->value( "ExportAsRelative", false ).toBool();
      break;
   case KarmadromeRandomizeExport:
      retval = settings->value( "RandomizeExport" ).toBool();
      break;
   case KarmadromeUseCheckBoxes:
      retval = settings->value( "UseCheckBoxes", false ).toBool();
      break;
   default:
      qFatal( "illegal KarmadromeBool value" );
   }
   return retval;
}


void Settings::setValue( KarmadromeInt id, int value )
{
   MySettings *settings = cpSettings->get( "Karmadrome" );
   switch( id )
   {
   case KarmadromeNumberOfColumns:
      settings->setValue( "NumberOfColumns", value );
      break;
   default:
      qFatal( "illegal KarmadromeInt value" );
   }
   settings->endGroup();
}


int Settings::value( KarmadromeInt id )
{
   int retval;
   MySettings *settings = cpSettings->get( "Karmadrome" );
   switch( id )
   {
   case KarmadromeNumberOfColumns:
      retval = settings->value( "NumberOfColumns", 3 ).toInt();
      break;
   default:
      qFatal( "illegal KarmadromeInt value" );
   }
   return retval;
}


void Settings::setValue( NotoriousString id, const QString &value )
{
   MySettings *settings = cpSettings->get( "Notorious" );
   switch( id )
   {
   case NotoriousDatabaseFile:
      settings->setValue( "DatabaseFile", value );
      break;
   default:
      qFatal( "illegal NotoriousString value" );
   }
   settings->endGroup();
}


QString Settings::value( NotoriousString id )
{
   QString retval;
   MySettings *settings = cpSettings->get( "Notorious" );
   switch( id )
   {
   case NotoriousDatabaseFile:
      retval = settings->value( "DatabaseFile" ).toString();
      break;
   default:
      qFatal( "illegal NotoriousString value" );
   }
   return retval;
}


void Settings::setValue( PartymanPoint id, const QPoint &value )
{
   MySettings *settings = cpSettings->get( "Partyman" );
   switch( id )
   {
   case PartymanSplitterSizes:
      settings->setValue( "SplitterSizes", value );
      break;
   default:
      qFatal( "illegal PartymanPoint value" );
   }
   settings->endGroup();
}


QPoint Settings::value( PartymanPoint id )
{
   QPoint retval;
   MySettings *settings = cpSettings->get( "Partyman" );
   switch( id )
   {
   case PartymanSplitterSizes:
      retval = settings->value( "SplitterSizes" ).toPoint();
      break;
   default:
      qFatal( "illegal PartymanPoint value" );
   }
   return retval;
}


void Settings::setValue( PartymanString id, const QString &value )
{
   MySettings *settings = cpSettings->get( "Partyman" );
   switch( id )
   {
   case PartymanDerMixDcmd:
      settings->setValue( "DerMixDcmd", value );
      break;
   case PartymanDerMixDhost:
      settings->setValue( "DerMixDhost", value );
      break;
   case PartymanListPattern:
      settings->setValue( "ListPattern", value );
      break;
   case PartymanLogCmd:
      settings->setValue( "LogCmd", value );
      break;
   case PartymanNamePattern:
      settings->setValue( "NamePattern", value );
      break;
   case PartymanPlayFolder:
      settings->setValue( "PlayFolder", value );
      break;
   case PartymanPlayerPattern:
      settings->setValue( "PlayerPattern", value );
      break;
   case PartymanSearch:
      settings->setValue( "Search", value );
      break;
   case PartymanTrayIconPattern:
      settings->setValue( "TrayIconPattern", value );
      break;
   default:
      qFatal( "illegal PartymanString value" );
   }
   settings->endGroup();
}


QString Settings::value( PartymanString id )
{
   QString retval;
   MySettings *settings = cpSettings->get( "Partyman" );
   switch( id )
   {
   case PartymanDerMixDcmd:
      retval = settings->value( "DerMixDcmd", "dermixd" ).toString();
      break;
   case PartymanDerMixDhost:
      retval = settings->value( "DerMixDhost", "localhost" ).toString();
      break;
   case PartymanListPattern:
      retval = settings->value( "ListPattern", "(|$PLAYTIME|)|$ARTIST| - |$TITLE|" ).toString();
      break;
   case PartymanLogCmd:
      retval = settings->value( "LogCmd" ).toString();
      break;
   case PartymanNamePattern:
      retval = settings->value( "NamePattern", QApplication::applicationName() + ": " + "|$TITLE|" ).toString();
      break;
   case PartymanPlayFolder:
      retval = settings->value( "PlayFolder" ).toString();
      break;
   case PartymanPlayerPattern:
      retval = settings->value( "PlayerPattern", "|$ARTIST| - |$TITLE|" ).toString();
      break;
   case PartymanSearch:
      retval = settings->value( "Search" ).toString();
      break;
   case PartymanTrayIconPattern:
      retval = settings->value( "TrayIconPattern", "|$ARTIST|#n|$TITLE|#n|$ALBUM|" ).toString();
      break;
   default:
      qFatal( "illegal PartymanString value" );
   }
   return retval;
}


void Settings::setValue( PartymanStringList id, const QStringList &value )
{
   MySettings *settings = cpSettings->get( "Partyman" );
   switch( id )
   {
   case PartymanPlayedArtists:
      settings->setValue( "PlayedArtists", value );
      break;
   case PartymanPlaylist:
      settings->setValue( "Playlist", value );
      break;
   default:
      qFatal( "illegal PartymanStringList value" );
   }
   settings->endGroup();
}


QStringList Settings::value( PartymanStringList id )
{
   QStringList retval;
   MySettings *settings = cpSettings->get( "Partyman" );
   switch( id )
   {
   case PartymanPlayedArtists:
      retval = settings->value( "PlayedArtists" ).toStringList();
      break;
   case PartymanPlaylist:
      retval = settings->value( "Playlist" ).toStringList();
      break;
   default:
      qFatal( "illegal PartymanStringList value" );
   }
   return retval;
}


void Settings::setValue( PartymanBool id, bool value )
{
   MySettings *settings = cpSettings->get( "Partyman" );
   switch( id )
   {
   case PartymanAutoConnect:
      settings->setValue( "AutoConnect", value );
      break;
   case PartymanCountSkip:
      settings->setValue( "CountSkip", value );
      break;
   case PartymanDerMixDlog:
      settings->setValue( "DerMixDlog", value );
      break;
   case PartymanDerMixDrun:
      settings->setValue( "DerMixDrun", value );
      break;
   case PartymanPlayOnlyFavorite:
      settings->setValue( "PlayOnlyFavorite", value );
      break;
   case PartymanPlayOnlyLeastPlayed:
      settings->setValue( "PlayOnlyLeastPlayed", value );
      break;
   case PartymanSplitterVertical:
      settings->setValue( "SplitterVertical", value );
      break;
   case PartymanTrayIcon:
      settings->setValue( "TrayIcon", value );
      break;
   case PartymanTrayIconBubble:
      settings->setValue( "TrayIconBubble", value );
      break;
   default:
      qFatal( "illegal PartymanBool value" );
   }
   settings->endGroup();
}


bool Settings::value( PartymanBool id )
{
   bool retval;
   MySettings *settings = cpSettings->get( "Partyman" );
   switch( id )
   {
   case PartymanAutoConnect:
      retval = settings->value( "AutoConnect", false ).toBool();
      break;
   case PartymanCountSkip:
      retval = settings->value( "CountSkip", false ).toBool();
      break;
   case PartymanDerMixDlog:
      retval = settings->value( "DerMixDlog", false ).toBool();
      break;
   case PartymanDerMixDrun:
      retval = settings->value( "DerMixDrun", true ).toBool();
      break;
   case PartymanPlayOnlyFavorite:
      retval = settings->value( "PlayOnlyFavorite", false ).toBool();
      break;
   case PartymanPlayOnlyLeastPlayed:
      retval = settings->value( "PlayOnlyLeastPlayed", false ).toBool();
      break;
   case PartymanSplitterVertical:
      retval = settings->value( "SplitterVertical", false ).toBool();
      break;
   case PartymanTrayIcon:
      retval = settings->value( "TrayIcon", false ).toBool();
      break;
   case PartymanTrayIconBubble:
      retval = settings->value( "TrayIconBubble", true ).toBool();
      break;
   default:
      qFatal( "illegal PartymanBool value" );
   }
   return retval;
}


void Settings::setValue( PartymanDouble id, double value )
{
   MySettings *settings = cpSettings->get( "Partyman" );
   switch( id )
   {
   case PartymanNormalizeValue:
      settings->setValue( "NormalizeValue", value );
      break;
   case PartymanTrayIconBubbleTime:
      settings->setValue( "TrayIconBubbleTime", value );
      break;
   default:
      qFatal( "illegal PartymanDouble value" );
   }
   settings->endGroup();
}


double Settings::value( PartymanDouble id )
{
   double retval;
   MySettings *settings = cpSettings->get( "Partyman" );
   switch( id )
   {
   case PartymanNormalizeValue:
      retval = settings->value( "NormalizeValue", 0.4 ).toDouble();
      break;
   case PartymanTrayIconBubbleTime:
      retval = settings->value( "TrayIconBubbleTime", 4.0 ).toDouble();
      break;
   default:
      qFatal( "illegal PartymanDouble value" );
   }
   return retval;
}


void Settings::setValue( PartymanInt id, int value )
{
   MySettings *settings = cpSettings->get( "Partyman" );
   switch( id )
   {
   case PartymanCrossfadeTime:
      settings->setValue( "CrossfadeTime", value );
      break;
   case PartymanCurrentTab:
      settings->setValue( "CurrentTab", value );
      break;
   case PartymanDerMixDport:
      settings->setValue( "DerMixDport", value );
      break;
   case PartymanNormalizeMode:
      settings->setValue( "NormalizeMode", value );
      break;
   case PartymanPlayNotAgainCount:
      settings->setValue( "PlayNotAgainCount", value );
      break;
   case PartymanRandomTries:
      settings->setValue( "RandomTries", value );
      break;
   case PartymanTrayIconBubbleIcon:
      settings->setValue( "TrayIconBubbleIcon", value );
      break;
   default:
      qFatal( "illegal PartymanInt value" );
   }
   settings->endGroup();
}


int Settings::value( PartymanInt id )
{
   int retval;
   MySettings *settings = cpSettings->get( "Partyman" );
   switch( id )
   {
   case PartymanCrossfadeTime:
      retval = settings->value( "CrossfadeTime", 10 ).toInt();
      break;
   case PartymanCurrentTab:
      retval = settings->value( "CurrentTab", 0 ).toInt();
      break;
   case PartymanDerMixDport:
      retval = settings->value( "DerMixDport", 8888 ).toInt();
      break;
   case PartymanNormalizeMode:
      retval = settings->value( "NormalizeMode", 0 ).toInt();
      break;
   case PartymanPlayNotAgainCount:
      retval = settings->value( "PlayNotAgainCount", 10 ).toInt();
      break;
   case PartymanRandomTries:
      retval = settings->value( "RandomTries", 10 ).toInt();
      break;
   case PartymanTrayIconBubbleIcon:
      retval = settings->value( "TrayIconBubbleIcon", 0 ).toInt();
      break;
   default:
      qFatal( "illegal PartymanInt value" );
   }
   return retval;
}


void Settings::setValue( RubberbandmanString id, const QString &value )
{
   MySettings *settings = cpSettings->get( "Rubberbandman" );
   switch( id )
   {
   case RubberbandmanPlayingPattern:
      settings->setValue( "PlayingPattern", value );
      break;
   case RubberbandmanRootDirectory:
      settings->setValue( "RootDirectory", value );
      break;
   case RubberbandmanWithTrackNr:
      settings->setValue( "WithTrackNr", value );
      break;
   case RubberbandmanWithoutTrackNr:
      settings->setValue( "WithoutTrackNr", value );
      break;
   default:
      qFatal( "illegal RubberbandmanString value" );
   }
   settings->endGroup();
}


QString Settings::value( RubberbandmanString id )
{
   QString retval;
   MySettings *settings = cpSettings->get( "Rubberbandman" );
   switch( id )
   {
   case RubberbandmanPlayingPattern:
      retval = settings->value( "PlayingPattern", "NP: |$ARTIST| - |$TITLE|" ).toString();
      break;
   case RubberbandmanRootDirectory:
      retval = settings->value( "RootDirectory", "/" ).toString();
      break;
   case RubberbandmanWithTrackNr:
      retval = settings->value( "WithTrackNr", "|$ARTIST| - |$TITLE|" ).toString();
      break;
   case RubberbandmanWithoutTrackNr:
      retval = settings->value( "WithoutTrackNr", "(|#2TRACKNUMBER|)|$ARTIST| - |$TITLE|" ).toString();
      break;
   default:
      qFatal( "illegal RubberbandmanString value" );
   }
   return retval;
}


void Settings::setValue( RubberbandmanStringList id, const QStringList &value )
{
   MySettings *settings = cpSettings->get( "Rubberbandman" );
   switch( id )
   {
   case RubberbandmanFileExtensions:
      settings->setValue( "FileExtensions", value );
      break;
   default:
      qFatal( "illegal RubberbandmanStringList value" );
   }
   settings->endGroup();
}


QStringList Settings::value( RubberbandmanStringList id )
{
   QStringList retval;
   MySettings *settings = cpSettings->get( "Rubberbandman" );
   switch( id )
   {
   case RubberbandmanFileExtensions:
      retval = settings->value( "FileExtensions", QStringList() << "*.mp3" << "*.ogg" << "*.oga" << "*.flac" ).toStringList();
      break;
   default:
      qFatal( "illegal RubberbandmanStringList value" );
   }
   return retval;
}


void Settings::setValue( RubberbandmanBool id, bool value )
{
   MySettings *settings = cpSettings->get( "Rubberbandman" );
   switch( id )
   {
   case RubberbandmanAutoRescan:
      settings->setValue( "AutoRescan", value );
      break;
   default:
      qFatal( "illegal RubberbandmanBool value" );
   }
   settings->endGroup();
}


bool Settings::value( RubberbandmanBool id )
{
   bool retval;
   MySettings *settings = cpSettings->get( "Rubberbandman" );
   switch( id )
   {
   case RubberbandmanAutoRescan:
      retval = settings->value( "AutoRescan", true ).toBool();
      break;
   default:
      qFatal( "illegal RubberbandmanBool value" );
   }
   return retval;
}


void Settings::setValue( RubberbandmanInt id, int value )
{
   MySettings *settings = cpSettings->get( "Rubberbandman" );
   switch( id )
   {
   case RubberbandmanCurrentTab:
      settings->setValue( "CurrentTab", value );
      break;
   default:
      qFatal( "illegal RubberbandmanInt value" );
   }
   settings->endGroup();
}


int Settings::value( RubberbandmanInt id )
{
   int retval;
   MySettings *settings = cpSettings->get( "Rubberbandman" );
   switch( id )
   {
   case RubberbandmanCurrentTab:
      retval = settings->value( "CurrentTab", 0 ).toInt();
      break;
   default:
      qFatal( "illegal RubberbandmanInt value" );
   }
   return retval;
}


void Settings::setValue( StrippedString id, const QString &value )
{
   MySettings *settings = cpSettings->get( "Stripped" );
   switch( id )
   {
   case StrippedCreatePattern:
      settings->setValue( "CreatePattern", value );
      break;
   case StrippedDevice:
      settings->setValue( "Device", value );
      break;
   case StrippedDirectory:
      settings->setValue( "Directory", value );
      break;
   case StrippedStyleSheet:
      settings->setValue( "StyleSheet", value );
      break;
   default:
      qFatal( "illegal StrippedString value" );
   }
   settings->endGroup();
}


QString Settings::value( StrippedString id )
{
   QString retval;
   MySettings *settings = cpSettings->get( "Stripped" );
   switch( id )
   {
   case StrippedCreatePattern:
      retval = settings->value( "CreatePattern", "|$ALBUMARTIST|/|$ALBUM|/(|#2TRACKNUMBER|)|$ARTIST| - |$TITLE|" ).toString();
      break;
   case StrippedDevice:
      retval = settings->value( "Device", "/dev/cdrom" ).toString();
      break;
   case StrippedDirectory:
      retval = settings->value( "Directory", QDir::current().absolutePath() ).toString();
      break;
   case StrippedStyleSheet:
      retval = settings->value( "StyleSheet" ).toString();
      break;
   default:
      qFatal( "illegal StrippedString value" );
   }
   return retval;
}


void Settings::setValue( StrippedBool id, bool value )
{
   MySettings *settings = cpSettings->get( "Stripped" );
   switch( id )
   {
   case StrippedAutoEject:
      settings->setValue( "AutoEject", value );
      break;
   case StrippedAutoEnqueue:
      settings->setValue( "AutoEnqueue", value );
      break;
   case StrippedAutoFreeDB:
      settings->setValue( "AutoFreeDB", value );
      break;
   case StrippedCDTextLatin1:
      settings->setValue( "CDTextLatin1", value );
      break;
   case StrippedShowStats:
      settings->setValue( "ShowStats", value );
      break;
   default:
      qFatal( "illegal StrippedBool value" );
   }
   settings->endGroup();
}


bool Settings::value( StrippedBool id )
{
   bool retval;
   MySettings *settings = cpSettings->get( "Stripped" );
   switch( id )
   {
   case StrippedAutoEject:
      retval = settings->value( "AutoEject", false ).toBool();
      break;
   case StrippedAutoEnqueue:
      retval = settings->value( "AutoEnqueue", false ).toBool();
      break;
   case StrippedAutoFreeDB:
      retval = settings->value( "AutoFreeDB", true ).toBool();
      break;
   case StrippedCDTextLatin1:
      retval = settings->value( "CDTextLatin1", false ).toBool();
      break;
   case StrippedShowStats:
      retval = settings->value( "ShowStats", false ).toBool();
      break;
   default:
      qFatal( "illegal StrippedBool value" );
   }
   return retval;
}


void Settings::cleanup()
{
   remove( "Listener" );
   remove( "SLARTCommunication" );
   remove( "StyleSheet" );
   remove( "UDPListenerPort" );
   remove( "flv2mpeg4", "Funkytown" );
   remove( "UseGlobalStyleSheetFile", "Global" );
   remove( "Next", "Innuendo" );
   remove( "Play", "Innuendo" );
   remove( "Stop", "Innuendo" );
   remove( "ButtonRows", "Karmadrome" );
   remove( "DatabaseFilename", "Partyman" );
}
