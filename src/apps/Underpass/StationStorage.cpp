/*
 * src/libs/Underpass/StationStorage.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "StationStorage.hpp"

/* system headers */

/* Qt headers */
#include <QStringList>
#include <QTimer>

/* local library headers */
#include <Settings.hpp>

/* local headers */


StationStorage::StationStorage( QObject *parent )
: QObject( parent )
, mpSettings( Settings::get() )
, mpTimer( new QTimer( this ) )
, mListChanged( false )
, mPrefix( "Stations/%1" )
, mStation()
, mUrl()
, mPlayer()
{
   mpTimer->setSingleShot( true );
   mpTimer->setInterval( 333 );
   connect( mpTimer, SIGNAL(timeout()),
            this, SLOT(save()) );
}


StationStorage::~StationStorage()
{
}


QStringList StationStorage::stationList() const
{
   QStringList stations;
   const QString prefix( mPrefix.arg(QString()) );
   foreach( const QString &entry, mpSettings->allKeys() )
   {
      if( entry.startsWith( prefix ) )
      {
         stations << entry.mid( prefix.size() );
      }
   }
   return stations;
}


QStringList StationStorage::playerList() const
{
   QStringList players( Settings::value( Settings::UnderpassPlayers ) );
   /* temporary until settings dialog takes care of this */
   Settings::setValue( Settings::UnderpassPlayers, players );

   return players;
}


QString StationStorage::station() const
{
   return mStation;
}


void StationStorage::setStation( const QString &name )
{
   const QString data( mpSettings->value( mPrefix.arg(name) ).toString() );
   int pipePos = data.indexOf('|');
   if( pipePos >= 0 )
   {
      mStation = name;
      mPlayer  = data.left( pipePos );
      mUrl     = data.mid( pipePos + 1 );
   }
   else
   {
      mUrl.clear();
   }
}


QString StationStorage::player() const
{
   return mPlayer;
}


void StationStorage::setPlayer( const QString &text )
{
   mPlayer = text;
   mpTimer->start();
}


QString StationStorage::url() const
{
   return mUrl;
}


void StationStorage::setUrl( const QString &text )
{
   mUrl = text;
   mpTimer->start();
}


bool StationStorage::stationListChanged()
{
   bool retval = mListChanged;
   mListChanged = false;
   return retval;
}


void StationStorage::save()
{
   addStation( mStation, mPlayer, mUrl );
}


void StationStorage::addStation( const QString &name,
                                 const QString &player,
                                 const QString &url )
{
   mpSettings->setValue( mPrefix.arg(name), player + '|' + url );
}


void StationStorage::removeStation( const QString &name )
{
   mpSettings->remove( mPrefix.arg(name) );
   mListChanged = true;
}


void StationStorage::createDefaults()
{
   QFile f( ":/Underpass/Stations.txt" );
   f.open( QFile::ReadOnly );
   const QStringList stationEntries( QString::fromUtf8( f.readAll().constData() ).split('\n') );
   f.close();

   foreach( const QString &station, stationEntries )
   {
      const QStringList elements( station.split('|') );
      if( elements.count() == 3 )
      {
         const QString &name   = elements.at(0);
         const QString &player = elements.at(1);
         const QString &url    = elements.at(2);
         QString data( mpSettings->value( mPrefix.arg(name) ).toString() );
         if( data.isEmpty() )
         {
            addStation( name, player, url );
            mListChanged = true;
         }
      }
      else
      {
         qWarning( "Malformed line in config file %s", qPrintable(f.fileName()) );
      }
   }
}
