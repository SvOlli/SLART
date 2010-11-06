/**
 * src/Partyman/TrackInfoListModel.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
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
   TrackInfoListModel( Database *database, QObject *parent = 0 );

   /* get number of elements in list */
   int rowCount( const QModelIndex &parent = QModelIndex() ) const;

   /* get data for display, reimplemented to match model requirements */
   QVariant data( const QModelIndex &index, int role ) const;
   /* set the data */
   bool setData( const QModelIndex &index, const QVariant &value, int role = Qt::DisplayRole );

   /* reimplemented to match model requirements */
   Qt::ItemFlags flags(const QModelIndex &index) const;

   /* insert empty rows */
   bool insertRows( int row, int count, const QModelIndex &parent = QModelIndex() );
   /* remove rows and drop their data */
   bool removeRows( int row, int count, const QModelIndex &parent = QModelIndex() );

   /* get the model data */
   TrackInfoList list() const;
   /* set the model data */
   void setList( const TrackInfoList &list );

   /* reimplemented to match model requirements */
   QStringList mimeTypes() const;
   /* reimplemented for drag'n'drop */
   Qt::DropActions supportedDragActions() const;
   /* reimplemented for drag'n'drop */
   Qt::DropActions supportedDropActions() const;
   /* get the mime data, reimplemented to match model requirements */
   QMimeData *mimeData( const QModelIndexList &indexes ) const;
   /* handle mime data of dropped objects */
   bool dropMimeData( const QMimeData *data, Qt::DropAction action,
                      int row, int column, const QModelIndex &parent );

public slots:
   /* re-read configuration */
   void readSettings();

private:
   TrackInfoListModel( const TrackInfoListModel &other );
   TrackInfoListModel &operator=( const TrackInfoListModel &other );

   Database       *mpDatabase;
   bool           mLocalMode;
   QString        mDisplayPattern;
   TrackInfoList  mList;
};

#endif
