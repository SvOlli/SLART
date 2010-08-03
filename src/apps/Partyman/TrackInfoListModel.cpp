/**
 * src/Partyman/TrackInfoListModel.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "TrackInfoListModel.hpp"

/* system headers */

/* Qt headers */
#include <QMimeData>
#include <QUrl>

/* local library headers */
#include <Database.hpp>
#include <MySettings.hpp>

/* local headers */
#include "ConfigDialog.hpp"


TrackInfoListModel::TrackInfoListModel( Database *database, QObject *parent )
: QAbstractListModel( parent )
, mpDatabase( database )
, mLocalMode( true )
, mDisplayPattern()
, mList()
{
   configChanged();
}


void TrackInfoListModel::configChanged()
{
   MySettings settings;
   mDisplayPattern = settings.VALUE_LISTPATTERN;
   mLocalMode      = settings.VALUE_DERMIXDRUN;
}


int TrackInfoListModel::rowCount( const QModelIndex &parent ) const
{
   if( parent.isValid() )
   {
      return 0;
   }

   return mList.count();
}


QVariant TrackInfoListModel::data( const QModelIndex &index, int role ) const
{
   if( (index.row() < 0) || (index.row() >= mList.size()) )
   {
      return QVariant();
   }

   switch( role )
   {
   case Qt::DisplayRole:
   case Qt::EditRole:
      {
         return mList.at( index.row() ).displayString( mDisplayPattern );
      }
   case Qt::ToolTipRole:
      {
         return mList.at( index.row() ).filePath();
      }
   default:
      return QVariant();
   }
}


Qt::ItemFlags TrackInfoListModel::flags( const QModelIndex &index ) const
{
   /* TODO: set flags according to remote or local mode */
   if( !index.isValid() )
   {
      return Qt::ItemIsDropEnabled;
   }

   return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled;
}


bool TrackInfoListModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
   if( (index.row() >= 0) && (index.row() < mList.size()) &&
       ( (role == Qt::EditRole) || (role == Qt::DisplayRole) ) )
   {
      TrackInfo ti;
      if( !mpDatabase->getTrackInfo( &ti, value.toString() ) )
      {
         QFileInfo fi( value.toString() );
         ti.mDirectory = fi.absolutePath();
         ti.mFileName  = fi.fileName();
      }
      mList.replace( index.row(), ti );
      emit dataChanged( index, index );
      return true;
   }
   return false;
}


bool TrackInfoListModel::insertRows( int row, int count, const QModelIndex &parent )
{
   if( (count < 1) || (row < 0) || (row > rowCount(parent)) )
   {
      return false;
   }

   beginInsertRows( QModelIndex(), row, row + count - 1 );

   for ( int i = 0; i < count; i++ )
   {
      mList.insert( row, TrackInfo() );
   }

   endInsertRows();

   return true;
}


bool TrackInfoListModel::removeRows( int row, int count, const QModelIndex &parent )
{
   if( (count <= 0) || (row < 0) || ((row + count) > rowCount(parent)) )
   {
      return false;
   }

   beginRemoveRows( QModelIndex(), row, row + count - 1 );

   for( int i = 0; i < count; i++ )
   {
      mList.removeAt( row );
   }

   endRemoveRows();

   return true;
}


TrackInfoList TrackInfoListModel::list() const
{
   return mList;
}


void TrackInfoListModel::setList( const TrackInfoList &list )
{
   mList = list;
   reset();
}


Qt::DropActions TrackInfoListModel::supportedDragActions() const
{
   return Qt::MoveAction;
}


Qt::DropActions TrackInfoListModel::supportedDropActions() const
{
   return Qt::MoveAction | Qt::CopyAction;
}


QStringList TrackInfoListModel::mimeTypes() const
{
   QStringList types;
   types << "text/uri-list";
   return types;
}


QMimeData *TrackInfoListModel::mimeData( const QModelIndexList &indexes ) const
{
   QMimeData *mimeData = new QMimeData();

   QList<QUrl> urls;

   for( int i = 0; i < indexes.size(); i++ )
   {
      if( indexes.at(i).isValid() )
      {
         QString text = data( indexes.at(i), Qt::ToolTipRole ).toString();
         if( !text.isEmpty() )
         {
            urls << QUrl::fromLocalFile( text );
         }
      }
   }
   mimeData->setUrls( urls );

   return mimeData;
}


bool TrackInfoListModel::dropMimeData( const QMimeData *data, Qt::DropAction action,
                                       int row, int column, const QModelIndex &parent )
{
   if( action == Qt::IgnoreAction )
   {
      return true;
   }

   if( !data->hasUrls() )
   {
      return false;
   }

   if( column > 0 )
   {
      return false;
   }

   int beginRow;
   if( row != -1 )
   {
      beginRow = row;
   }
   else if( parent.isValid() )
   {
      beginRow = parent.row();
   }
   else
   {
      beginRow = rowCount();
   }

   QModelIndex idx;
   QList<QUrl> newUrls( data->urls() );
   insertRows( beginRow, newUrls.size() );
   for( int i = 0; i < newUrls.size(); i++ )
   {
      idx = index( beginRow + i );
      setData( idx, newUrls.at(i).toLocalFile() );
   }

   return true;
}