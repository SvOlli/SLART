/**
 * src/Partyman/TrackInfoListModel.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef TRACKINFOLISTMODEL_HPP
#define TRACKINFOLISTMODEL_HPP TRACKINFOLISTMODEL_HPP

/* base class */
#include <QAbstractListModel>

/* system headers */

/* Qt headers */
#include <QString>

/* local library headers */
#include <TrackInfo.hpp>

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */
class Database;


class TrackInfoListModel : public QAbstractListModel
{
    Q_OBJECT

public:
   /*  */
   TrackInfoListModel( Database *database, QObject *parent = 0 );

   /*  */
   int rowCount( const QModelIndex &parent = QModelIndex() ) const;

   /*  */
   QVariant data( const QModelIndex &index, int role ) const;
   /*  */
   bool setData( const QModelIndex &index, const QVariant &value, int role = Qt::DisplayRole );

   /*  */
   Qt::ItemFlags flags(const QModelIndex &index) const;

   /*  */
   bool insertRows( int row, int count, const QModelIndex &parent = QModelIndex() );
   /*  */
   bool removeRows( int row, int count, const QModelIndex &parent = QModelIndex() );

   /*  */
   TrackInfoList list() const;
   /*  */
   void setList( const TrackInfoList &list );

   /*  */
   QStringList mimeTypes() const;
   /*  */
   Qt::DropActions supportedDragActions() const;
   /*  */
   Qt::DropActions supportedDropActions() const;
   /*  */
   QMimeData *mimeData( const QModelIndexList &indexes ) const;
   /*  */
   bool dropMimeData( const QMimeData *data, Qt::DropAction action,
                      int row, int column, const QModelIndex &parent );

public slots:
   void configChanged();

private:
   TrackInfoListModel( const TrackInfoListModel &other );
   TrackInfoListModel &operator=( const TrackInfoListModel &other );

   Database       *mpDatabase;
   bool           mLocalMode;
   QString        mDisplayPattern;
   TrackInfoList  mList;
};

#endif
