/**
 * src/libs/Common/Settings.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef SETTINGS_HPP
#define SETTINGS_HPP SETTINGS_HPP

/* base class */
#include <QObject>

/* system headers */

/* Qt headers */
#include <QCoreApplication>
#include <QDir>
#include <QPoint>
#include <QSize>

/* local library headers */
#include "MySettings.hpp"

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */

/*!
  \addtogroup Common
  @{
  */

/*!
 \brief abstraction layer

*/
class Settings : public QObject
{
   Q_OBJECT

public:

   /* generated code start */
   /*!
    \brief enum for value() and setValue()

    automatically generated
    */
   enum CommonPoint
   {
      CommonMainWindowPosition
   };
   /*!
    \brief enum for value() and setValue()

    automatically generated
    */
   enum CommonSize
   {
      CommonMainWindowSize
   };
   /*!
    \brief enum for value() and setValue()

    automatically generated
    */
   enum CommonString
   {
      CommonStyleSheetFile
   };
   /*!
    \brief enum for value() and setValue()

    automatically generated
    */
   enum CommonBool
   {
      CommonUseGlobalStyleSheetFile,
      CommonUseSatellite
   };
   /*!
    \brief enum for value() and setValue()

    automatically generated
    */
   enum MagicFLACString
   {
      MagicFLACDirectory
   };
   /*!
    \brief enum for value() and setValue()

    automatically generated
    */
   enum MagicFLACBool
   {
      MagicFLACDirectoryOverride,
      MagicFLACFlacUseOga,
      MagicFLACUseEncoder
   };
   /*!
    \brief enum for value() and setValue()

    automatically generated
    */
   enum MagicFLACInt
   {
      MagicFLACFlacQuality
   };
   /*!
    \brief enum for value() and setValue()

    automatically generated
    */
   enum Magicmp3String
   {
      Magicmp3Directory
   };
   /*!
    \brief enum for value() and setValue()

    automatically generated
    */
   enum Magicmp3Bool
   {
      Magicmp3DirectoryOverride,
      Magicmp3UseEncoder,
      Magicmp3UseLatin1,
      Magicmp3VBRQuality
   };
   /*!
    \brief enum for value() and setValue()

    automatically generated
    */
   enum MagicoggString
   {
      MagicoggDirectory
   };
   /*!
    \brief enum for value() and setValue()

    automatically generated
    */
   enum MagicoggBool
   {
      MagicoggDirectoryOverride,
      MagicoggUseEncoder
   };
   /*!
    \brief enum for value() and setValue()

    automatically generated
    */
   enum MagicoggDouble
   {
      MagicoggOggQuality
   };
   /*!
    \brief enum for value() and setValue()

    automatically generated
    */
   enum MagicwavString
   {
      MagicwavDirectory
   };
   /*!
    \brief enum for value() and setValue()

    automatically generated
    */
   enum MagicwavBool
   {
      MagicwavDirectoryOverride,
      MagicwavUseEncoder
   };
   /*!
    \brief enum for value() and setValue()

    automatically generated
    */
   enum FunkytownString
   {
      FunkytownUserAgent
   };
   /*!
    \brief enum for value() and setValue()

    automatically generated
    */
   enum FunkytownBool
   {
      FunkytownCoverArt,
      FunkytownOverwrite,
      FunkytownTollKeep
   };
   /*!
    \brief enum for value() and setValue()

    automatically generated
    */
   enum FunkytownUlonglong
   {
      FunkytownBytes
   };
   /*!
    \brief enum for value() and setValue()

    automatically generated
    */
   enum FunkytownUint
   {
      FunkytownFiles
   };
   /*!
    \brief enum for value() and setValue()

    automatically generated
    */
   enum GlobalString
   {
      GlobalMusicBase,
      GlobalSatelliteHost,
      GlobalStyleSheetFile,
      GlobalVersion
   };
   /*!
    \brief enum for value() and setValue()

    automatically generated
    */
   enum GlobalBool
   {
      GlobalAnimateViews,
      GlobalNormalizeCase,
      GlobalNormalizeSpaces,
      GlobalShowCleanupDialog
   };
   /*!
    \brief enum for value() and setValue()

    automatically generated
    */
   enum GlobalInt
   {
      GlobalClipboardMode,
      GlobalDoubleClickInterval,
      GlobalSatellitePort
   };
   /*!
    \brief enum for value() and setValue()

    automatically generated
    */
   enum GlobalHTTPProxyString
   {
      GlobalHTTPProxyHost,
      GlobalHTTPProxyLogin,
      GlobalHTTPProxyPassword,
      GlobalHTTPProxyPort
   };
   /*!
    \brief enum for value() and setValue()

    automatically generated
    */
   enum GlobalHTTPProxyBool
   {
      GlobalHTTPProxyAuth,
      GlobalHTTPProxyEnable
   };
   /*!
    \brief enum for value() and setValue()

    automatically generated
    */
   enum InnuendoStringList
   {
      InnuendoStartup
   };
   /*!
    \brief enum for value() and setValue()

    automatically generated
    */
   enum InnuendoInt
   {
      InnuendoBufferSize
   };
   /*!
    \brief enum for value() and setValue()

    automatically generated
    */
   enum KarmadromeString
   {
      KarmadromeExportDirectory,
      KarmadromeImportDirectory
   };
   /*!
    \brief enum for value() and setValue()

    automatically generated
    */
   enum KarmadromeBool
   {
      KarmadromeClearBeforeImport,
      KarmadromeExportAsRelative,
      KarmadromeRandomizeExport,
      KarmadromeUseCheckBoxes
   };
   /*!
    \brief enum for value() and setValue()

    automatically generated
    */
   enum KarmadromeInt
   {
      KarmadromeNumberOfColumns
   };
   /*!
    \brief enum for value() and setValue()

    automatically generated
    */
   enum NotoriousString
   {
      NotoriousDatabaseFile
   };
   /*!
    \brief enum for value() and setValue()

    automatically generated
    */
   enum PartymanPoint
   {
      PartymanSplitterSizes
   };
   /*!
    \brief enum for value() and setValue()

    automatically generated
    */
   enum PartymanString
   {
      PartymanDerMixDcmd,
      PartymanDerMixDhost,
      PartymanListPattern,
      PartymanLogCmd,
      PartymanNamePattern,
      PartymanPlayFolder,
      PartymanPlayerPattern,
      PartymanSearch,
      PartymanTrayIconPattern
   };
   /*!
    \brief enum for value() and setValue()

    automatically generated
    */
   enum PartymanStringList
   {
      PartymanPlayedArtists,
      PartymanPlaylist
   };
   /*!
    \brief enum for value() and setValue()

    automatically generated
    */
   enum PartymanBool
   {
      PartymanAutoConnect,
      PartymanCountSkip,
      PartymanDerMixDlog,
      PartymanDerMixDrun,
      PartymanPlayOnlyFavorite,
      PartymanPlayOnlyLeastPlayed,
      PartymanSplitterVertical,
      PartymanTrayIcon,
      PartymanTrayIconBubble
   };
   /*!
    \brief enum for value() and setValue()

    automatically generated
    */
   enum PartymanDouble
   {
      PartymanNormalizeValue,
      PartymanTrayIconBubbleTime
   };
   /*!
    \brief enum for value() and setValue()

    automatically generated
    */
   enum PartymanInt
   {
      PartymanCrossfadeTime,
      PartymanCurrentTab,
      PartymanDerMixDport,
      PartymanNormalizeMode,
      PartymanPlayNotAgainCount,
      PartymanRandomTries,
      PartymanTrayIconBubbleIcon
   };
   /*!
    \brief enum for value() and setValue()

    automatically generated
    */
   enum RubberbandmanString
   {
      RubberbandmanPlayingPattern,
      RubberbandmanRootDirectory,
      RubberbandmanWithTrackNr,
      RubberbandmanWithoutTrackNr
   };
   /*!
    \brief enum for value() and setValue()

    automatically generated
    */
   enum RubberbandmanStringList
   {
      RubberbandmanFileExtensions
   };
   /*!
    \brief enum for value() and setValue()

    automatically generated
    */
   enum RubberbandmanBool
   {
      RubberbandmanAutoRescan
   };
   /*!
    \brief enum for value() and setValue()

    automatically generated
    */
   enum RubberbandmanInt
   {
      RubberbandmanCurrentTab
   };
   /*!
    \brief enum for value() and setValue()

    automatically generated
    */
   enum StrippedString
   {
      StrippedCreatePattern,
      StrippedDevice,
      StrippedDirectory,
      StrippedStyleSheet
   };
   /*!
    \brief enum for value() and setValue()

    automatically generated
    */
   enum StrippedBool
   {
      StrippedAutoEject,
      StrippedAutoEnqueue,
      StrippedAutoFreeDB,
      StrippedCDTextLatin1,
      StrippedShowStats
   };
   /*!
    \brief set a value in registry

    automatically generated
    \param id id of registry key
    \param value to set
    */
   static void setValue( CommonPoint id, const QPoint &value );

   /*!
    \brief get a value in registry

    automatically generated
    \return value in registry
    \param id id of registry key
    */
   static QPoint value( CommonPoint id );

   /*!
    \brief set a value in registry

    automatically generated
    \param id id of registry key
    \param value to set
    */
   static void setValue( CommonSize id, const QSize &value );

   /*!
    \brief get a value in registry

    automatically generated
    \return value in registry
    \param id id of registry key
    */
   static QSize value( CommonSize id );

   /*!
    \brief set a value in registry

    automatically generated
    \param id id of registry key
    \param value to set
    */
   static void setValue( CommonString id, const QString &value );

   /*!
    \brief get a value in registry

    automatically generated
    \return value in registry
    \param id id of registry key
    */
   static QString value( CommonString id );

   /*!
    \brief set a value in registry

    automatically generated
    \param id id of registry key
    \param value to set
    */
   static void setValue( CommonBool id, bool value );

   /*!
    \brief get a value in registry

    automatically generated
    \return value in registry
    \param id id of registry key
    */
   static bool value( CommonBool id );

   /*!
    \brief set a value in registry

    automatically generated
    \param id id of registry key
    \param value to set
    */
   static void setValue( MagicFLACString id, const QString &value );

   /*!
    \brief get a value in registry

    automatically generated
    \return value in registry
    \param id id of registry key
    */
   static QString value( MagicFLACString id );

   /*!
    \brief set a value in registry

    automatically generated
    \param id id of registry key
    \param value to set
    */
   static void setValue( MagicFLACBool id, bool value );

   /*!
    \brief get a value in registry

    automatically generated
    \return value in registry
    \param id id of registry key
    */
   static bool value( MagicFLACBool id );

   /*!
    \brief set a value in registry

    automatically generated
    \param id id of registry key
    \param value to set
    */
   static void setValue( MagicFLACInt id, int value );

   /*!
    \brief get a value in registry

    automatically generated
    \return value in registry
    \param id id of registry key
    */
   static int value( MagicFLACInt id );

   /*!
    \brief set a value in registry

    automatically generated
    \param id id of registry key
    \param value to set
    */
   static void setValue( Magicmp3String id, const QString &value );

   /*!
    \brief get a value in registry

    automatically generated
    \return value in registry
    \param id id of registry key
    */
   static QString value( Magicmp3String id );

   /*!
    \brief set a value in registry

    automatically generated
    \param id id of registry key
    \param value to set
    */
   static void setValue( Magicmp3Bool id, bool value );

   /*!
    \brief get a value in registry

    automatically generated
    \return value in registry
    \param id id of registry key
    */
   static bool value( Magicmp3Bool id );

   /*!
    \brief set a value in registry

    automatically generated
    \param id id of registry key
    \param value to set
    */
   static void setValue( MagicoggString id, const QString &value );

   /*!
    \brief get a value in registry

    automatically generated
    \return value in registry
    \param id id of registry key
    */
   static QString value( MagicoggString id );

   /*!
    \brief set a value in registry

    automatically generated
    \param id id of registry key
    \param value to set
    */
   static void setValue( MagicoggBool id, bool value );

   /*!
    \brief get a value in registry

    automatically generated
    \return value in registry
    \param id id of registry key
    */
   static bool value( MagicoggBool id );

   /*!
    \brief set a value in registry

    automatically generated
    \param id id of registry key
    \param value to set
    */
   static void setValue( MagicoggDouble id, double value );

   /*!
    \brief get a value in registry

    automatically generated
    \return value in registry
    \param id id of registry key
    */
   static double value( MagicoggDouble id );

   /*!
    \brief set a value in registry

    automatically generated
    \param id id of registry key
    \param value to set
    */
   static void setValue( MagicwavString id, const QString &value );

   /*!
    \brief get a value in registry

    automatically generated
    \return value in registry
    \param id id of registry key
    */
   static QString value( MagicwavString id );

   /*!
    \brief set a value in registry

    automatically generated
    \param id id of registry key
    \param value to set
    */
   static void setValue( MagicwavBool id, bool value );

   /*!
    \brief get a value in registry

    automatically generated
    \return value in registry
    \param id id of registry key
    */
   static bool value( MagicwavBool id );

   /*!
    \brief set a value in registry

    automatically generated
    \param id id of registry key
    \param value to set
    */
   static void setValue( FunkytownString id, const QString &value );

   /*!
    \brief get a value in registry

    automatically generated
    \return value in registry
    \param id id of registry key
    */
   static QString value( FunkytownString id );

   /*!
    \brief set a value in registry

    automatically generated
    \param id id of registry key
    \param value to set
    */
   static void setValue( FunkytownBool id, bool value );

   /*!
    \brief get a value in registry

    automatically generated
    \return value in registry
    \param id id of registry key
    */
   static bool value( FunkytownBool id );

   /*!
    \brief set a value in registry

    automatically generated
    \param id id of registry key
    \param value to set
    */
   static void setValue( FunkytownUlonglong id, qulonglong value );

   /*!
    \brief get a value in registry

    automatically generated
    \return value in registry
    \param id id of registry key
    */
   static qulonglong value( FunkytownUlonglong id );

   /*!
    \brief set a value in registry

    automatically generated
    \param id id of registry key
    \param value to set
    */
   static void setValue( FunkytownUint id, uint value );

   /*!
    \brief get a value in registry

    automatically generated
    \return value in registry
    \param id id of registry key
    */
   static uint value( FunkytownUint id );

   /*!
    \brief set a value in registry

    automatically generated
    \param id id of registry key
    \param value to set
    */
   static void setValue( GlobalString id, const QString &value );

   /*!
    \brief get a value in registry

    automatically generated
    \return value in registry
    \param id id of registry key
    */
   static QString value( GlobalString id );

   /*!
    \brief set a value in registry

    automatically generated
    \param id id of registry key
    \param value to set
    */
   static void setValue( GlobalBool id, bool value );

   /*!
    \brief get a value in registry

    automatically generated
    \return value in registry
    \param id id of registry key
    */
   static bool value( GlobalBool id );

   /*!
    \brief set a value in registry

    automatically generated
    \param id id of registry key
    \param value to set
    */
   static void setValue( GlobalInt id, int value );

   /*!
    \brief get a value in registry

    automatically generated
    \return value in registry
    \param id id of registry key
    */
   static int value( GlobalInt id );

   /*!
    \brief set a value in registry

    automatically generated
    \param id id of registry key
    \param value to set
    */
   static void setValue( GlobalHTTPProxyString id, const QString &value );

   /*!
    \brief get a value in registry

    automatically generated
    \return value in registry
    \param id id of registry key
    */
   static QString value( GlobalHTTPProxyString id );

   /*!
    \brief set a value in registry

    automatically generated
    \param id id of registry key
    \param value to set
    */
   static void setValue( GlobalHTTPProxyBool id, bool value );

   /*!
    \brief get a value in registry

    automatically generated
    \return value in registry
    \param id id of registry key
    */
   static bool value( GlobalHTTPProxyBool id );

   /*!
    \brief set a value in registry

    automatically generated
    \param id id of registry key
    \param value to set
    */
   static void setValue( InnuendoStringList id, const QStringList &value );

   /*!
    \brief get a value in registry

    automatically generated
    \return value in registry
    \param id id of registry key
    */
   static QStringList value( InnuendoStringList id );

   /*!
    \brief set a value in registry

    automatically generated
    \param id id of registry key
    \param value to set
    */
   static void setValue( InnuendoInt id, int value );

   /*!
    \brief get a value in registry

    automatically generated
    \return value in registry
    \param id id of registry key
    */
   static int value( InnuendoInt id );

   /*!
    \brief set a value in registry

    automatically generated
    \param id id of registry key
    \param value to set
    */
   static void setValue( KarmadromeString id, const QString &value );

   /*!
    \brief get a value in registry

    automatically generated
    \return value in registry
    \param id id of registry key
    */
   static QString value( KarmadromeString id );

   /*!
    \brief set a value in registry

    automatically generated
    \param id id of registry key
    \param value to set
    */
   static void setValue( KarmadromeBool id, bool value );

   /*!
    \brief get a value in registry

    automatically generated
    \return value in registry
    \param id id of registry key
    */
   static bool value( KarmadromeBool id );

   /*!
    \brief set a value in registry

    automatically generated
    \param id id of registry key
    \param value to set
    */
   static void setValue( KarmadromeInt id, int value );

   /*!
    \brief get a value in registry

    automatically generated
    \return value in registry
    \param id id of registry key
    */
   static int value( KarmadromeInt id );

   /*!
    \brief set a value in registry

    automatically generated
    \param id id of registry key
    \param value to set
    */
   static void setValue( NotoriousString id, const QString &value );

   /*!
    \brief get a value in registry

    automatically generated
    \return value in registry
    \param id id of registry key
    */
   static QString value( NotoriousString id );

   /*!
    \brief set a value in registry

    automatically generated
    \param id id of registry key
    \param value to set
    */
   static void setValue( PartymanPoint id, const QPoint &value );

   /*!
    \brief get a value in registry

    automatically generated
    \return value in registry
    \param id id of registry key
    */
   static QPoint value( PartymanPoint id );

   /*!
    \brief set a value in registry

    automatically generated
    \param id id of registry key
    \param value to set
    */
   static void setValue( PartymanString id, const QString &value );

   /*!
    \brief get a value in registry

    automatically generated
    \return value in registry
    \param id id of registry key
    */
   static QString value( PartymanString id );

   /*!
    \brief set a value in registry

    automatically generated
    \param id id of registry key
    \param value to set
    */
   static void setValue( PartymanStringList id, const QStringList &value );

   /*!
    \brief get a value in registry

    automatically generated
    \return value in registry
    \param id id of registry key
    */
   static QStringList value( PartymanStringList id );

   /*!
    \brief set a value in registry

    automatically generated
    \param id id of registry key
    \param value to set
    */
   static void setValue( PartymanBool id, bool value );

   /*!
    \brief get a value in registry

    automatically generated
    \return value in registry
    \param id id of registry key
    */
   static bool value( PartymanBool id );

   /*!
    \brief set a value in registry

    automatically generated
    \param id id of registry key
    \param value to set
    */
   static void setValue( PartymanDouble id, double value );

   /*!
    \brief get a value in registry

    automatically generated
    \return value in registry
    \param id id of registry key
    */
   static double value( PartymanDouble id );

   /*!
    \brief set a value in registry

    automatically generated
    \param id id of registry key
    \param value to set
    */
   static void setValue( PartymanInt id, int value );

   /*!
    \brief get a value in registry

    automatically generated
    \return value in registry
    \param id id of registry key
    */
   static int value( PartymanInt id );

   /*!
    \brief set a value in registry

    automatically generated
    \param id id of registry key
    \param value to set
    */
   static void setValue( RubberbandmanString id, const QString &value );

   /*!
    \brief get a value in registry

    automatically generated
    \return value in registry
    \param id id of registry key
    */
   static QString value( RubberbandmanString id );

   /*!
    \brief set a value in registry

    automatically generated
    \param id id of registry key
    \param value to set
    */
   static void setValue( RubberbandmanStringList id, const QStringList &value );

   /*!
    \brief get a value in registry

    automatically generated
    \return value in registry
    \param id id of registry key
    */
   static QStringList value( RubberbandmanStringList id );

   /*!
    \brief set a value in registry

    automatically generated
    \param id id of registry key
    \param value to set
    */
   static void setValue( RubberbandmanBool id, bool value );

   /*!
    \brief get a value in registry

    automatically generated
    \return value in registry
    \param id id of registry key
    */
   static bool value( RubberbandmanBool id );

   /*!
    \brief set a value in registry

    automatically generated
    \param id id of registry key
    \param value to set
    */
   static void setValue( RubberbandmanInt id, int value );

   /*!
    \brief get a value in registry

    automatically generated
    \return value in registry
    \param id id of registry key
    */
   static int value( RubberbandmanInt id );

   /*!
    \brief set a value in registry

    automatically generated
    \param id id of registry key
    \param value to set
    */
   static void setValue( StrippedString id, const QString &value );

   /*!
    \brief get a value in registry

    automatically generated
    \return value in registry
    \param id id of registry key
    */
   static QString value( StrippedString id );

   /*!
    \brief set a value in registry

    automatically generated
    \param id id of registry key
    \param value to set
    */
   static void setValue( StrippedBool id, bool value );

   /*!
    \brief get a value in registry

    automatically generated
    \return value in registry
    \param id id of registry key
    */
   static bool value( StrippedBool id );

   /*!
    \brief cleanup unused registry entries

    automatically generated
    */
   void cleanup();

   /* generated code end */

private:
   /*!
    \brief get MySettings pointer according to application name

    \param applicationName
   */
   static MySettings *get( const QString &applicationName = QCoreApplication::applicationName() );

   /*!
    \brief get MySettings pointer according to application name

    \param applicationName
   */
   static void remove( const QString &key,
                       const QString &applicationName = QString() );

   /*!
    \brief constructor

    \param parent parent QObject
   */
   Settings( QObject *parent = 0 );

   /*!
    \brief destructor

   */
   virtual ~Settings();

   QMap<QString,MySettings*>  mSettings; /*!< map for storing settings pointers */
   static Settings            *cpSettings; /*!< handle for static functions */
};

/*! @} */

#endif