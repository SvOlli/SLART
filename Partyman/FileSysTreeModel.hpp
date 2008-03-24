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


class FileSysTreeItem;

class FileSysTreeModel : public QAbstractItemModel
{
Q_OBJECT

public:
   FileSysTreeModel( QObject *parent = 0 );
   virtual ~FileSysTreeModel();

   void clear();
   QVariant data(const QModelIndex &index, int role) const;
   Qt::ItemFlags flags(const QModelIndex &index) const;
   QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
   QModelIndex parent(const QModelIndex &index) const;
   int rowCount(const QModelIndex &parent = QModelIndex()) const;
   int columnCount(const QModelIndex &parent = QModelIndex()) const;
   void setModelData( const QStringList &filenames );
   void addModelData( const QString &filename );
   QString getPath( const QModelIndex &index ) const;

private:
   FileSysTreeModel( const FileSysTreeModel &other );
   FileSysTreeModel &operator=( const FileSysTreeModel &other );

   FileSysTreeItem *mpRootItem;
};

#endif
