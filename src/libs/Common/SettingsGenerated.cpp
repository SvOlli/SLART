#include "Settings.hpp"
#include <QApplication>
#include <QDir>
#include <QPoint>
#include <QSize>


void Settings::setValue( CommonPoint id, const QPoint &value )
{
   QSettings *settings = cpSettings->get();
   switch( id )
   {
   case CommonMainWindowPosition:
      settings->setValue( "MainWindowPosition", value );
      return;
   default:
      qFatal( "illegal CommonPoint value" );
   }
}


QPoint Settings::value( CommonPoint id )
{
   QSettings *settings = cpSettings->get();
   switch( id )
   {
   case CommonMainWindowPosition:
      return settings->value( "MainWindowPosition" ).toPoint();
   default:
      qFatal( "illegal CommonPoint value" );
      return QPoint();
   }
}


void Settings::setValue( CommonSize id, const QSize &value )
{
   QSettings *settings = cpSettings->get();
   switch( id )
   {
   case CommonMainWindowSize:
      settings->setValue( "MainWindowSize", value );
      return;
   default:
      qFatal( "illegal CommonSize value" );
   }
}


QSize Settings::value( CommonSize id )
{
   QSettings *settings = cpSettings->get();
   switch( id )
   {
   case CommonMainWindowSize:
      return settings->value( "MainWindowSize" ).toSize();
   default:
      qFatal( "illegal CommonSize value" );
      return QSize();
   }
}


void Settings::setValue( CommonString id, const QString &value )
{
   QSettings *settings = cpSettings->get();
   switch( id )
   {
   case CommonDirectory:
      settings->setValue( "Directory", value );
      return;
   case CommonStyleSheetFile:
      settings->setValue( "StyleSheetFile", value );
      return;
   default:
      qFatal( "illegal CommonString value" );
   }
}


QString Settings::value( CommonString id )
{
   QSettings *settings = cpSettings->get();
   switch( id )
   {
   case CommonDirectory:
      return settings->value( "Directory", QDir::currentPath() ).toString();
   case CommonStyleSheetFile:
      return settings->value( "StyleSheetFile" ).toString();
   default:
      qFatal( "illegal CommonString value" );
      return QString();
   }
}


void Settings::setValue( CommonBool id, bool value )
{
   QSettings *settings = cpSettings->get();
   switch( id )
   {
   case CommonUseGlobalStyleSheetFile:
      settings->setValue( "UseGlobalStyleSheetFile", value );
      return;
   case CommonUseSatellite:
      settings->setValue( "UseSatellite", value );
      return;
   default:
      qFatal( "illegal CommonBool value" );
   }
}


bool Settings::value( CommonBool id )
{
   QSettings *settings = cpSettings->get();
   switch( id )
   {
   case CommonUseGlobalStyleSheetFile:
      return settings->value( "UseGlobalStyleSheetFile" ).toBool();
   case CommonUseSatellite:
      return settings->value( "UseSatellite", false ).toBool();
   default:
      qFatal( "illegal CommonBool value" );
      return bool();
   }
}


void Settings::setValue( MagicFLACString id, const QString &value )
{
   QSettings *settings = cpSettings->get();
   switch( id )
   {
   case MagicFLACDirectory:
      settings->setValue( "FLAC/Directory", value );
      return;
   default:
      qFatal( "illegal MagicFLACString value" );
   }
}


QString Settings::value( MagicFLACString id )
{
   QSettings *settings = cpSettings->get();
   switch( id )
   {
   case MagicFLACDirectory:
      return settings->value( "FLAC/Directory", QDir::current().absolutePath() ).toString();
   default:
      qFatal( "illegal MagicFLACString value" );
      return QString();
   }
}


void Settings::setValue( MagicFLACBool id, bool value )
{
   QSettings *settings = cpSettings->get();
   switch( id )
   {
   case MagicFLACDirectoryOverride:
      settings->setValue( "FLAC/DirectoryOverride", value );
      return;
   case MagicFLACFlacUseOga:
      settings->setValue( "FLAC/FlacUseOga", value );
      return;
   case MagicFLACUseEncoder:
      settings->setValue( "FLAC/UseEncoder", value );
      return;
   default:
      qFatal( "illegal MagicFLACBool value" );
   }
}


bool Settings::value( MagicFLACBool id )
{
   QSettings *settings = cpSettings->get();
   switch( id )
   {
   case MagicFLACDirectoryOverride:
      return settings->value( "FLAC/DirectoryOverride", false ).toBool();
   case MagicFLACFlacUseOga:
      return settings->value( "FLAC/FlacUseOga", false ).toBool();
   case MagicFLACUseEncoder:
      return settings->value( "FLAC/UseEncoder", false ).toBool();
   default:
      qFatal( "illegal MagicFLACBool value" );
      return bool();
   }
}


void Settings::setValue( MagicFLACInt id, int value )
{
   QSettings *settings = cpSettings->get();
   switch( id )
   {
   case MagicFLACFlacQuality:
      settings->setValue( "FLAC/FlacQuality", value );
      return;
   default:
      qFatal( "illegal MagicFLACInt value" );
   }
}


int Settings::value( MagicFLACInt id )
{
   QSettings *settings = cpSettings->get();
   switch( id )
   {
   case MagicFLACFlacQuality:
      return settings->value( "FLAC/FlacQuality", 5 ).toInt();
   default:
      qFatal( "illegal MagicFLACInt value" );
      return int();
   }
}


void Settings::setValue( Magicmp3String id, const QString &value )
{
   QSettings *settings = cpSettings->get();
   switch( id )
   {
   case Magicmp3Directory:
      settings->setValue( "mp3/Directory", value );
      return;
   default:
      qFatal( "illegal Magicmp3String value" );
   }
}


QString Settings::value( Magicmp3String id )
{
   QSettings *settings = cpSettings->get();
   switch( id )
   {
   case Magicmp3Directory:
      return settings->value( "mp3/Directory", QDir::current().absolutePath() ).toString();
   default:
      qFatal( "illegal Magicmp3String value" );
      return QString();
   }
}


void Settings::setValue( Magicmp3Bool id, bool value )
{
   QSettings *settings = cpSettings->get();
   switch( id )
   {
   case Magicmp3DirectoryOverride:
      settings->setValue( "mp3/DirectoryOverride", value );
      return;
   case Magicmp3UseEncoder:
      settings->setValue( "mp3/UseEncoder", value );
      return;
   case Magicmp3UseLatin1:
      settings->setValue( "mp3/UseLatin1", value );
      return;
   case Magicmp3VBRQuality:
      settings->setValue( "mp3/VBRQuality", value );
      return;
   default:
      qFatal( "illegal Magicmp3Bool value" );
   }
}


bool Settings::value( Magicmp3Bool id )
{
   QSettings *settings = cpSettings->get();
   switch( id )
   {
   case Magicmp3DirectoryOverride:
      return settings->value( "mp3/DirectoryOverride", false ).toBool();
   case Magicmp3UseEncoder:
      return settings->value( "mp3/UseEncoder", false ).toBool();
   case Magicmp3UseLatin1:
      return settings->value( "mp3/UseLatin1", false ).toBool();
   case Magicmp3VBRQuality:
      return settings->value( "mp3/VBRQuality", 4.0 ).toBool();
   default:
      qFatal( "illegal Magicmp3Bool value" );
      return bool();
   }
}


void Settings::setValue( MagicoggString id, const QString &value )
{
   QSettings *settings = cpSettings->get();
   switch( id )
   {
   case MagicoggDirectory:
      settings->setValue( "ogg/Directory", value );
      return;
   default:
      qFatal( "illegal MagicoggString value" );
   }
}


QString Settings::value( MagicoggString id )
{
   QSettings *settings = cpSettings->get();
   switch( id )
   {
   case MagicoggDirectory:
      return settings->value( "ogg/Directory", QDir::current().absolutePath() ).toString();
   default:
      qFatal( "illegal MagicoggString value" );
      return QString();
   }
}


void Settings::setValue( MagicoggBool id, bool value )
{
   QSettings *settings = cpSettings->get();
   switch( id )
   {
   case MagicoggDirectoryOverride:
      settings->setValue( "ogg/DirectoryOverride", value );
      return;
   case MagicoggUseEncoder:
      settings->setValue( "ogg/UseEncoder", value );
      return;
   default:
      qFatal( "illegal MagicoggBool value" );
   }
}


bool Settings::value( MagicoggBool id )
{
   QSettings *settings = cpSettings->get();
   switch( id )
   {
   case MagicoggDirectoryOverride:
      return settings->value( "ogg/DirectoryOverride", false ).toBool();
   case MagicoggUseEncoder:
      return settings->value( "ogg/UseEncoder", false ).toBool();
   default:
      qFatal( "illegal MagicoggBool value" );
      return bool();
   }
}


void Settings::setValue( MagicoggDouble id, double value )
{
   QSettings *settings = cpSettings->get();
   switch( id )
   {
   case MagicoggOggQuality:
      settings->setValue( "ogg/OggQuality", value );
      return;
   default:
      qFatal( "illegal MagicoggDouble value" );
   }
}


double Settings::value( MagicoggDouble id )
{
   QSettings *settings = cpSettings->get();
   switch( id )
   {
   case MagicoggOggQuality:
      return settings->value( "ogg/OggQuality", 0.4 ).toDouble();
   default:
      qFatal( "illegal MagicoggDouble value" );
      return double();
   }
}


void Settings::setValue( MagicwavString id, const QString &value )
{
   QSettings *settings = cpSettings->get();
   switch( id )
   {
   case MagicwavDirectory:
      settings->setValue( "wav/Directory", value );
      return;
   default:
      qFatal( "illegal MagicwavString value" );
   }
}


QString Settings::value( MagicwavString id )
{
   QSettings *settings = cpSettings->get();
   switch( id )
   {
   case MagicwavDirectory:
      return settings->value( "wav/Directory", QDir::current().absolutePath() ).toString();
   default:
      qFatal( "illegal MagicwavString value" );
      return QString();
   }
}


void Settings::setValue( MagicwavBool id, bool value )
{
   QSettings *settings = cpSettings->get();
   switch( id )
   {
   case MagicwavDirectoryOverride:
      settings->setValue( "wav/DirectoryOverride", value );
      return;
   case MagicwavUseEncoder:
      settings->setValue( "wav/UseEncoder", value );
      return;
   default:
      qFatal( "illegal MagicwavBool value" );
   }
}


bool Settings::value( MagicwavBool id )
{
   QSettings *settings = cpSettings->get();
   switch( id )
   {
   case MagicwavDirectoryOverride:
      return settings->value( "wav/DirectoryOverride", false ).toBool();
   case MagicwavUseEncoder:
      return settings->value( "wav/UseEncoder", false ).toBool();
   default:
      qFatal( "illegal MagicwavBool value" );
      return bool();
   }
}


void Settings::setValue( FunkytownString id, const QString &value )
{
   QSettings *settings = cpSettings->get( "Funkytown" );
   switch( id )
   {
   case FunkytownUserAgent:
      settings->setValue( "UserAgent", value );
      return;
   default:
      qFatal( "illegal FunkytownString value" );
   }
}


QString Settings::value( FunkytownString id )
{
   QSettings *settings = cpSettings->get( "Funkytown" );
   switch( id )
   {
   case FunkytownUserAgent:
      return settings->value( "UserAgent", "Shockwave Flash" ).toString();
   default:
      qFatal( "illegal FunkytownString value" );
      return QString();
   }
}


void Settings::setValue( FunkytownBool id, bool value )
{
   QSettings *settings = cpSettings->get( "Funkytown" );
   switch( id )
   {
   case FunkytownCoverArt:
      settings->setValue( "CoverArt", value );
      return;
   case FunkytownOverwrite:
      settings->setValue( "Overwrite", value );
      return;
   case FunkytownTollKeep:
      settings->setValue( "TollKeep", value );
      return;
   default:
      qFatal( "illegal FunkytownBool value" );
   }
}


bool Settings::value( FunkytownBool id )
{
   QSettings *settings = cpSettings->get( "Funkytown" );
   switch( id )
   {
   case FunkytownCoverArt:
      return settings->value( "CoverArt", false ).toBool();
   case FunkytownOverwrite:
      return settings->value( "Overwrite", false ).toBool();
   case FunkytownTollKeep:
      return settings->value( "TollKeep", true ).toBool();
   default:
      qFatal( "illegal FunkytownBool value" );
      return bool();
   }
}


void Settings::setValue( FunkytownUlonglong id, qulonglong value )
{
   QSettings *settings = cpSettings->get( "Funkytown" );
   switch( id )
   {
   case FunkytownBytes:
      settings->setValue( "Bytes", value );
      return;
   default:
      qFatal( "illegal FunkytownUlonglong value" );
   }
}


qulonglong Settings::value( FunkytownUlonglong id )
{
   QSettings *settings = cpSettings->get( "Funkytown" );
   switch( id )
   {
   case FunkytownBytes:
      return settings->value( "Bytes", 0 ).toULongLong();
   default:
      qFatal( "illegal FunkytownUlonglong value" );
      return qulonglong();
   }
}


void Settings::setValue( FunkytownUint id, uint value )
{
   QSettings *settings = cpSettings->get( "Funkytown" );
   switch( id )
   {
   case FunkytownFiles:
      settings->setValue( "Files", value );
      return;
   default:
      qFatal( "illegal FunkytownUint value" );
   }
}


uint Settings::value( FunkytownUint id )
{
   QSettings *settings = cpSettings->get( "Funkytown" );
   switch( id )
   {
   case FunkytownFiles:
      return settings->value( "Files", 0 ).toUInt();
   default:
      qFatal( "illegal FunkytownUint value" );
      return uint();
   }
}


void Settings::setValue( GlobalString id, const QString &value )
{
   QSettings *settings = cpSettings->get( "Global" );
   switch( id )
   {
   case GlobalMusicBase:
      settings->setValue( "MusicBase", value );
      return;
   case GlobalSatelliteHost:
      settings->setValue( "SatelliteHost", value );
      return;
   case GlobalStyleSheetFile:
      settings->setValue( "StyleSheetFile", value );
      return;
   case GlobalVersion:
      settings->setValue( "Version", value );
      return;
   default:
      qFatal( "illegal GlobalString value" );
   }
}


QString Settings::value( GlobalString id )
{
   QSettings *settings = cpSettings->get( "Global" );
   switch( id )
   {
   case GlobalMusicBase:
      return settings->value( "MusicBase", "/" ).toString();
   case GlobalSatelliteHost:
      return settings->value( "SatelliteHost", "127.0.0.1" ).toString();
   case GlobalStyleSheetFile:
      return settings->value( "StyleSheetFile" ).toString();
   case GlobalVersion:
      return settings->value( "Version" ).toString();
   default:
      qFatal( "illegal GlobalString value" );
      return QString();
   }
}


void Settings::setValue( GlobalBool id, bool value )
{
   QSettings *settings = cpSettings->get( "Global" );
   switch( id )
   {
   case GlobalAnimateViews:
      settings->setValue( "AnimateViews", value );
      return;
   case GlobalNormalizeCase:
      settings->setValue( "NormalizeCase", value );
      return;
   case GlobalNormalizeSpaces:
      settings->setValue( "NormalizeSpaces", value );
      return;
   case GlobalShowCleanupDialog:
      settings->setValue( "ShowCleanupDialog", value );
      return;
   default:
      qFatal( "illegal GlobalBool value" );
   }
}


bool Settings::value( GlobalBool id )
{
   QSettings *settings = cpSettings->get( "Global" );
   switch( id )
   {
   case GlobalAnimateViews:
      return settings->value( "AnimateViews", false ).toBool();
   case GlobalNormalizeCase:
      return settings->value( "NormalizeCase", false ).toBool();
   case GlobalNormalizeSpaces:
      return settings->value( "NormalizeSpaces", false ).toBool();
   case GlobalShowCleanupDialog:
      return settings->value( "ShowCleanupDialog" ).toBool();
   default:
      qFatal( "illegal GlobalBool value" );
      return bool();
   }
}


void Settings::setValue( GlobalInt id, int value )
{
   QSettings *settings = cpSettings->get( "Global" );
   switch( id )
   {
   case GlobalClipboardMode:
      settings->setValue( "ClipboardMode", value );
      return;
   case GlobalDoubleClickInterval:
      settings->setValue( "DoubleClickInterval", value );
      return;
   case GlobalSatellitePort:
      settings->setValue( "SatellitePort", value );
      return;
   default:
      qFatal( "illegal GlobalInt value" );
   }
}


int Settings::value( GlobalInt id )
{
   QSettings *settings = cpSettings->get( "Global" );
   switch( id )
   {
   case GlobalClipboardMode:
      return settings->value( "ClipboardMode", 0 ).toInt();
   case GlobalDoubleClickInterval:
      return settings->value( "DoubleClickInterval", QApplication::doubleClickInterval() ).toInt();
   case GlobalSatellitePort:
      return settings->value( "SatellitePort", 24222 ).toInt();
   default:
      qFatal( "illegal GlobalInt value" );
      return int();
   }
}


void Settings::setValue( GlobalHTTPProxyString id, const QString &value )
{
   QSettings *settings = cpSettings->get( "Global" );
   switch( id )
   {
   case GlobalHTTPProxyHost:
      settings->setValue( "HTTPProxy/Host", value );
      return;
   case GlobalHTTPProxyLogin:
      settings->setValue( "HTTPProxy/Login", value );
      return;
   case GlobalHTTPProxyPassword:
      settings->setValue( "HTTPProxy/Password", value );
      return;
   case GlobalHTTPProxyPort:
      settings->setValue( "HTTPProxy/Port", value );
      return;
   default:
      qFatal( "illegal GlobalHTTPProxyString value" );
   }
}


QString Settings::value( GlobalHTTPProxyString id )
{
   QSettings *settings = cpSettings->get( "Global" );
   switch( id )
   {
   case GlobalHTTPProxyHost:
      return settings->value( "HTTPProxy/Host" ).toString();
   case GlobalHTTPProxyLogin:
      return settings->value( "HTTPProxy/Login" ).toString();
   case GlobalHTTPProxyPassword:
      return settings->value( "HTTPProxy/Password" ).toString();
   case GlobalHTTPProxyPort:
      return settings->value( "HTTPProxy/Port", "8080" ).toString();
   default:
      qFatal( "illegal GlobalHTTPProxyString value" );
      return QString();
   }
}


void Settings::setValue( GlobalHTTPProxyBool id, bool value )
{
   QSettings *settings = cpSettings->get( "Global" );
   switch( id )
   {
   case GlobalHTTPProxyAuth:
      settings->setValue( "HTTPProxy/Auth", value );
      return;
   case GlobalHTTPProxyEnable:
      settings->setValue( "HTTPProxy/Enable", value );
      return;
   default:
      qFatal( "illegal GlobalHTTPProxyBool value" );
   }
}


bool Settings::value( GlobalHTTPProxyBool id )
{
   QSettings *settings = cpSettings->get( "Global" );
   switch( id )
   {
   case GlobalHTTPProxyAuth:
      return settings->value( "HTTPProxy/Auth", false ).toBool();
   case GlobalHTTPProxyEnable:
      return settings->value( "HTTPProxy/Enable", false ).toBool();
   default:
      qFatal( "illegal GlobalHTTPProxyBool value" );
      return bool();
   }
}


void Settings::setValue( InnuendoStringList id, const QStringList &value )
{
   QSettings *settings = cpSettings->get( "Innuendo" );
   switch( id )
   {
   case InnuendoStartup:
      settings->setValue( "Startup", value );
      return;
   default:
      qFatal( "illegal InnuendoStringList value" );
   }
}


QStringList Settings::value( InnuendoStringList id )
{
   QSettings *settings = cpSettings->get( "Innuendo" );
   switch( id )
   {
   case InnuendoStartup:
      return settings->value( "Startup" ).toStringList();
   default:
      qFatal( "illegal InnuendoStringList value" );
      return QStringList();
   }
}


void Settings::setValue( InnuendoInt id, int value )
{
   QSettings *settings = cpSettings->get( "Innuendo" );
   switch( id )
   {
   case InnuendoBufferSize:
      settings->setValue( "BufferSize", value );
      return;
   default:
      qFatal( "illegal InnuendoInt value" );
   }
}


int Settings::value( InnuendoInt id )
{
   QSettings *settings = cpSettings->get( "Innuendo" );
   switch( id )
   {
   case InnuendoBufferSize:
      return settings->value( "BufferSize", 500 ).toInt();
   default:
      qFatal( "illegal InnuendoInt value" );
      return int();
   }
}


void Settings::setValue( KarmadromeString id, const QString &value )
{
   QSettings *settings = cpSettings->get( "Karmadrome" );
   switch( id )
   {
   case KarmadromeExportDirectory:
      settings->setValue( "ExportDirectory", value );
      return;
   case KarmadromeImportDirectory:
      settings->setValue( "ImportDirectory", value );
      return;
   default:
      qFatal( "illegal KarmadromeString value" );
   }
}


QString Settings::value( KarmadromeString id )
{
   QSettings *settings = cpSettings->get( "Karmadrome" );
   switch( id )
   {
   case KarmadromeExportDirectory:
      return settings->value( "ExportDirectory" ).toString();
   case KarmadromeImportDirectory:
      return settings->value( "ImportDirectory" ).toString();
   default:
      qFatal( "illegal KarmadromeString value" );
      return QString();
   }
}


void Settings::setValue( KarmadromeBool id, bool value )
{
   QSettings *settings = cpSettings->get( "Karmadrome" );
   switch( id )
   {
   case KarmadromeClearBeforeImport:
      settings->setValue( "ClearBeforeImport", value );
      return;
   case KarmadromeExportAsRelative:
      settings->setValue( "ExportAsRelative", value );
      return;
   case KarmadromeRandomizeExport:
      settings->setValue( "RandomizeExport", value );
      return;
   case KarmadromeUseCheckBoxes:
      settings->setValue( "UseCheckBoxes", value );
      return;
   default:
      qFatal( "illegal KarmadromeBool value" );
   }
}


bool Settings::value( KarmadromeBool id )
{
   QSettings *settings = cpSettings->get( "Karmadrome" );
   switch( id )
   {
   case KarmadromeClearBeforeImport:
      return settings->value( "ClearBeforeImport", false ).toBool();
   case KarmadromeExportAsRelative:
      return settings->value( "ExportAsRelative", false ).toBool();
   case KarmadromeRandomizeExport:
      return settings->value( "RandomizeExport" ).toBool();
   case KarmadromeUseCheckBoxes:
      return settings->value( "UseCheckBoxes", false ).toBool();
   default:
      qFatal( "illegal KarmadromeBool value" );
      return bool();
   }
}


void Settings::setValue( KarmadromeInt id, int value )
{
   QSettings *settings = cpSettings->get( "Karmadrome" );
   switch( id )
   {
   case KarmadromeNumberOfColumns:
      settings->setValue( "NumberOfColumns", value );
      return;
   default:
      qFatal( "illegal KarmadromeInt value" );
   }
}


int Settings::value( KarmadromeInt id )
{
   QSettings *settings = cpSettings->get( "Karmadrome" );
   switch( id )
   {
   case KarmadromeNumberOfColumns:
      return settings->value( "NumberOfColumns", 3 ).toInt();
   default:
      qFatal( "illegal KarmadromeInt value" );
      return int();
   }
}


void Settings::setValue( NotoriousString id, const QString &value )
{
   QSettings *settings = cpSettings->get( "Notorious" );
   switch( id )
   {
   case NotoriousDatabaseFile:
      settings->setValue( "DatabaseFile", value );
      return;
   default:
      qFatal( "illegal NotoriousString value" );
   }
}


QString Settings::value( NotoriousString id )
{
   QSettings *settings = cpSettings->get( "Notorious" );
   switch( id )
   {
   case NotoriousDatabaseFile:
      return settings->value( "DatabaseFile" ).toString();
   default:
      qFatal( "illegal NotoriousString value" );
      return QString();
   }
}


void Settings::setValue( PartymanPoint id, const QPoint &value )
{
   QSettings *settings = cpSettings->get( "Partyman" );
   switch( id )
   {
   case PartymanSplitterSizes:
      settings->setValue( "SplitterSizes", value );
      return;
   default:
      qFatal( "illegal PartymanPoint value" );
   }
}


QPoint Settings::value( PartymanPoint id )
{
   QSettings *settings = cpSettings->get( "Partyman" );
   switch( id )
   {
   case PartymanSplitterSizes:
      return settings->value( "SplitterSizes" ).toPoint();
   default:
      qFatal( "illegal PartymanPoint value" );
      return QPoint();
   }
}


void Settings::setValue( PartymanString id, const QString &value )
{
   QSettings *settings = cpSettings->get( "Partyman" );
   switch( id )
   {
   case PartymanDerMixDcmd:
      settings->setValue( "DerMixDcmd", value );
      return;
   case PartymanDerMixDhost:
      settings->setValue( "DerMixDhost", value );
      return;
   case PartymanListPattern:
      settings->setValue( "ListPattern", value );
      return;
   case PartymanLogCmd:
      settings->setValue( "LogCmd", value );
      return;
   case PartymanNamePattern:
      settings->setValue( "NamePattern", value );
      return;
   case PartymanPlayFolder:
      settings->setValue( "PlayFolder", value );
      return;
   case PartymanPlayerPattern:
      settings->setValue( "PlayerPattern", value );
      return;
   case PartymanSearch:
      settings->setValue( "Search", value );
      return;
   case PartymanTrayIconPattern:
      settings->setValue( "TrayIconPattern", value );
      return;
   default:
      qFatal( "illegal PartymanString value" );
   }
}


QString Settings::value( PartymanString id )
{
   QSettings *settings = cpSettings->get( "Partyman" );
   switch( id )
   {
   case PartymanDerMixDcmd:
      return settings->value( "DerMixDcmd", "dermixd" ).toString();
   case PartymanDerMixDhost:
      return settings->value( "DerMixDhost", "localhost" ).toString();
   case PartymanListPattern:
      return settings->value( "ListPattern", "(|$PLAYTIME|)|$ARTIST| - |$TITLE|" ).toString();
   case PartymanLogCmd:
      return settings->value( "LogCmd" ).toString();
   case PartymanNamePattern:
      return settings->value( "NamePattern", QApplication::applicationName() + ": " + "|$TITLE|" ).toString();
   case PartymanPlayFolder:
      return settings->value( "PlayFolder" ).toString();
   case PartymanPlayerPattern:
      return settings->value( "PlayerPattern", "|$ARTIST| - |$TITLE|" ).toString();
   case PartymanSearch:
      return settings->value( "Search" ).toString();
   case PartymanTrayIconPattern:
      return settings->value( "TrayIconPattern", "|$ARTIST|#n|$TITLE|#n|$ALBUM|" ).toString();
   default:
      qFatal( "illegal PartymanString value" );
      return QString();
   }
}


void Settings::setValue( PartymanStringList id, const QStringList &value )
{
   QSettings *settings = cpSettings->get( "Partyman" );
   switch( id )
   {
   case PartymanPlayedArtists:
      settings->setValue( "PlayedArtists", value );
      return;
   case PartymanPlaylist:
      settings->setValue( "Playlist", value );
      return;
   default:
      qFatal( "illegal PartymanStringList value" );
   }
}


QStringList Settings::value( PartymanStringList id )
{
   QSettings *settings = cpSettings->get( "Partyman" );
   switch( id )
   {
   case PartymanPlayedArtists:
      return settings->value( "PlayedArtists" ).toStringList();
   case PartymanPlaylist:
      return settings->value( "Playlist" ).toStringList();
   default:
      qFatal( "illegal PartymanStringList value" );
      return QStringList();
   }
}


void Settings::setValue( PartymanBool id, bool value )
{
   QSettings *settings = cpSettings->get( "Partyman" );
   switch( id )
   {
   case PartymanAutoConnect:
      settings->setValue( "AutoConnect", value );
      return;
   case PartymanCountSkip:
      settings->setValue( "CountSkip", value );
      return;
   case PartymanDerMixDlog:
      settings->setValue( "DerMixDlog", value );
      return;
   case PartymanDerMixDrun:
      settings->setValue( "DerMixDrun", value );
      return;
   case PartymanPlayOnlyFavorite:
      settings->setValue( "PlayOnlyFavorite", value );
      return;
   case PartymanPlayOnlyLeastPlayed:
      settings->setValue( "PlayOnlyLeastPlayed", value );
      return;
   case PartymanSplitterVertical:
      settings->setValue( "SplitterVertical", value );
      return;
   case PartymanTrayIcon:
      settings->setValue( "TrayIcon", value );
      return;
   case PartymanTrayIconBubble:
      settings->setValue( "TrayIconBubble", value );
      return;
   default:
      qFatal( "illegal PartymanBool value" );
   }
}


bool Settings::value( PartymanBool id )
{
   QSettings *settings = cpSettings->get( "Partyman" );
   switch( id )
   {
   case PartymanAutoConnect:
      return settings->value( "AutoConnect", false ).toBool();
   case PartymanCountSkip:
      return settings->value( "CountSkip", false ).toBool();
   case PartymanDerMixDlog:
      return settings->value( "DerMixDlog", false ).toBool();
   case PartymanDerMixDrun:
      return settings->value( "DerMixDrun", true ).toBool();
   case PartymanPlayOnlyFavorite:
      return settings->value( "PlayOnlyFavorite", false ).toBool();
   case PartymanPlayOnlyLeastPlayed:
      return settings->value( "PlayOnlyLeastPlayed", false ).toBool();
   case PartymanSplitterVertical:
      return settings->value( "SplitterVertical", false ).toBool();
   case PartymanTrayIcon:
      return settings->value( "TrayIcon", false ).toBool();
   case PartymanTrayIconBubble:
      return settings->value( "TrayIconBubble", true ).toBool();
   default:
      qFatal( "illegal PartymanBool value" );
      return bool();
   }
}


void Settings::setValue( PartymanDouble id, double value )
{
   QSettings *settings = cpSettings->get( "Partyman" );
   switch( id )
   {
   case PartymanNormalizeValue:
      settings->setValue( "NormalizeValue", value );
      return;
   case PartymanTrayIconBubbleTime:
      settings->setValue( "TrayIconBubbleTime", value );
      return;
   default:
      qFatal( "illegal PartymanDouble value" );
   }
}


double Settings::value( PartymanDouble id )
{
   QSettings *settings = cpSettings->get( "Partyman" );
   switch( id )
   {
   case PartymanNormalizeValue:
      return settings->value( "NormalizeValue", 0.4 ).toDouble();
   case PartymanTrayIconBubbleTime:
      return settings->value( "TrayIconBubbleTime", 4.0 ).toDouble();
   default:
      qFatal( "illegal PartymanDouble value" );
      return double();
   }
}


void Settings::setValue( PartymanInt id, int value )
{
   QSettings *settings = cpSettings->get( "Partyman" );
   switch( id )
   {
   case PartymanCrossfadeTime:
      settings->setValue( "CrossfadeTime", value );
      return;
   case PartymanCurrentTab:
      settings->setValue( "CurrentTab", value );
      return;
   case PartymanDerMixDport:
      settings->setValue( "DerMixDport", value );
      return;
   case PartymanNormalizeMode:
      settings->setValue( "NormalizeMode", value );
      return;
   case PartymanPlayNotAgainCount:
      settings->setValue( "PlayNotAgainCount", value );
      return;
   case PartymanRandomTries:
      settings->setValue( "RandomTries", value );
      return;
   case PartymanTrayIconBubbleIcon:
      settings->setValue( "TrayIconBubbleIcon", value );
      return;
   default:
      qFatal( "illegal PartymanInt value" );
   }
}


int Settings::value( PartymanInt id )
{
   QSettings *settings = cpSettings->get( "Partyman" );
   switch( id )
   {
   case PartymanCrossfadeTime:
      return settings->value( "CrossfadeTime", 10 ).toInt();
   case PartymanCurrentTab:
      return settings->value( "CurrentTab", 0 ).toInt();
   case PartymanDerMixDport:
      return settings->value( "DerMixDport", 8888 ).toInt();
   case PartymanNormalizeMode:
      return settings->value( "NormalizeMode", 0 ).toInt();
   case PartymanPlayNotAgainCount:
      return settings->value( "PlayNotAgainCount", 10 ).toInt();
   case PartymanRandomTries:
      return settings->value( "RandomTries", 10 ).toInt();
   case PartymanTrayIconBubbleIcon:
      return settings->value( "TrayIconBubbleIcon", 0 ).toInt();
   default:
      qFatal( "illegal PartymanInt value" );
      return int();
   }
}


void Settings::setValue( RubberbandmanString id, const QString &value )
{
   QSettings *settings = cpSettings->get( "Rubberbandman" );
   switch( id )
   {
   case RubberbandmanPlayingPattern:
      settings->setValue( "PlayingPattern", value );
      return;
   case RubberbandmanRootDirectory:
      settings->setValue( "RootDirectory", value );
      return;
   case RubberbandmanWithTrackNr:
      settings->setValue( "WithTrackNr", value );
      return;
   case RubberbandmanWithoutTrackNr:
      settings->setValue( "WithoutTrackNr", value );
      return;
   default:
      qFatal( "illegal RubberbandmanString value" );
   }
}


QString Settings::value( RubberbandmanString id )
{
   QSettings *settings = cpSettings->get( "Rubberbandman" );
   switch( id )
   {
   case RubberbandmanPlayingPattern:
      return settings->value( "PlayingPattern", "NP: |$ARTIST| - |$TITLE|" ).toString();
   case RubberbandmanRootDirectory:
      return settings->value( "RootDirectory", "/" ).toString();
   case RubberbandmanWithTrackNr:
      return settings->value( "WithTrackNr", "|$ARTIST| - |$TITLE|" ).toString();
   case RubberbandmanWithoutTrackNr:
      return settings->value( "WithoutTrackNr", "(|#2TRACKNUMBER|)|$ARTIST| - |$TITLE|" ).toString();
   default:
      qFatal( "illegal RubberbandmanString value" );
      return QString();
   }
}


void Settings::setValue( RubberbandmanStringList id, const QStringList &value )
{
   QSettings *settings = cpSettings->get( "Rubberbandman" );
   switch( id )
   {
   case RubberbandmanFileExtensions:
      settings->setValue( "FileExtensions", value );
      return;
   default:
      qFatal( "illegal RubberbandmanStringList value" );
   }
}


QStringList Settings::value( RubberbandmanStringList id )
{
   QSettings *settings = cpSettings->get( "Rubberbandman" );
   switch( id )
   {
   case RubberbandmanFileExtensions:
      return settings->value( "FileExtensions", QStringList() << "*.mp3" << "*.ogg" << "*.oga" << "*.flac" ).toStringList();
   default:
      qFatal( "illegal RubberbandmanStringList value" );
      return QStringList();
   }
}


void Settings::setValue( RubberbandmanBool id, bool value )
{
   QSettings *settings = cpSettings->get( "Rubberbandman" );
   switch( id )
   {
   case RubberbandmanAutoRescan:
      settings->setValue( "AutoRescan", value );
      return;
   default:
      qFatal( "illegal RubberbandmanBool value" );
   }
}


bool Settings::value( RubberbandmanBool id )
{
   QSettings *settings = cpSettings->get( "Rubberbandman" );
   switch( id )
   {
   case RubberbandmanAutoRescan:
      return settings->value( "AutoRescan", true ).toBool();
   default:
      qFatal( "illegal RubberbandmanBool value" );
      return bool();
   }
}


void Settings::setValue( RubberbandmanInt id, int value )
{
   QSettings *settings = cpSettings->get( "Rubberbandman" );
   switch( id )
   {
   case RubberbandmanCurrentTab:
      settings->setValue( "CurrentTab", value );
      return;
   default:
      qFatal( "illegal RubberbandmanInt value" );
   }
}


int Settings::value( RubberbandmanInt id )
{
   QSettings *settings = cpSettings->get( "Rubberbandman" );
   switch( id )
   {
   case RubberbandmanCurrentTab:
      return settings->value( "CurrentTab", 0 ).toInt();
   default:
      qFatal( "illegal RubberbandmanInt value" );
      return int();
   }
}


void Settings::setValue( StrippedString id, const QString &value )
{
   QSettings *settings = cpSettings->get( "Stripped" );
   switch( id )
   {
   case StrippedCreatePattern:
      settings->setValue( "CreatePattern", value );
      return;
   case StrippedDevice:
      settings->setValue( "Device", value );
      return;
   case StrippedDirectory:
      settings->setValue( "Directory", value );
      return;
   default:
      qFatal( "illegal StrippedString value" );
   }
}


QString Settings::value( StrippedString id )
{
   QSettings *settings = cpSettings->get( "Stripped" );
   switch( id )
   {
   case StrippedCreatePattern:
      return settings->value( "CreatePattern", "|$ALBUMARTIST|/|$ALBUM|/(|#2TRACKNUMBER|)|$ARTIST| - |$TITLE|" ).toString();
   case StrippedDevice:
      return settings->value( "Device", "/dev/cdrom" ).toString();
   case StrippedDirectory:
      return settings->value( "Directory", QDir::current().absolutePath() ).toString();
   default:
      qFatal( "illegal StrippedString value" );
      return QString();
   }
}


void Settings::setValue( StrippedBool id, bool value )
{
   QSettings *settings = cpSettings->get( "Stripped" );
   switch( id )
   {
   case StrippedAutoEject:
      settings->setValue( "AutoEject", value );
      return;
   case StrippedAutoEnqueue:
      settings->setValue( "AutoEnqueue", value );
      return;
   case StrippedAutoFreeDB:
      settings->setValue( "AutoFreeDB", value );
      return;
   case StrippedCDTextLatin1:
      settings->setValue( "CDTextLatin1", value );
      return;
   default:
      qFatal( "illegal StrippedBool value" );
   }
}


bool Settings::value( StrippedBool id )
{
   QSettings *settings = cpSettings->get( "Stripped" );
   switch( id )
   {
   case StrippedAutoEject:
      return settings->value( "AutoEject", false ).toBool();
   case StrippedAutoEnqueue:
      return settings->value( "AutoEnqueue", false ).toBool();
   case StrippedAutoFreeDB:
      return settings->value( "AutoFreeDB", true ).toBool();
   case StrippedCDTextLatin1:
      return settings->value( "CDTextLatin1", false ).toBool();
   default:
      qFatal( "illegal StrippedBool value" );
      return bool();
   }
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
   remove( "ShowStats", "Stripped" );
   remove( "StyleSheet", "Stripped" );
}
