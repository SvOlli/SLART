/**
 * FileSysTreeModel.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef FILESYSTREEMODEL_HPP
#define FILESYSTREEMODEL_HPP FILESYSTREEMODEL_HPP

#include <QWidget>
#include <QAbstractItemModel>
#include <QString>


class QItemSelection;
class FileSysTreeItem;

class FileSysTreeModel : public QAbstractItemModel
{
Q_OBJECT

public:
   FileSysTreeModel( const QString &rootPath, QObject *parent = 0 );
   ~FileSysTreeModel();

   QVariant data(const QModelIndex &index, int role) const;
   Qt::ItemFlags flags(const QModelIndex &index) const;
   QVariant headerData(Qt::Orientation orientation, int role = Qt::DisplayRole) const;
   QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
   QModelIndex parent(const QModelIndex &index) const;
   int rowCount(const QModelIndex &parent = QModelIndex()) const;
   int columnCount(const QModelIndex &parent = QModelIndex()) const;
   void setupModelData(const QString &rootPath);

public slots:
   QString getPath( const QModelIndex & index );
   void currentChanged( const QModelIndex & current, const QModelIndex & previous );

signals:
   void activated( const QModelIndex &data );
   void expandIndex( const QModelIndex &data );

private:
   FileSysTreeModel( const FileSysTreeModel &other );
   FileSysTreeModel &operator=( const FileSysTreeModel &other );

   FileSysTreeItem *mpRootItem;
   QString         mRootPath;
};

#endif
