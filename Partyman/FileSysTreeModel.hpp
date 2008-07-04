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

   /* clear the complete model */
   void clear();
   /* return the data of an index */
   QVariant data( const QModelIndex &index, int role ) const;
   /* set item flags to enabled */
   Qt::ItemFlags flags(const QModelIndex &index) const;
   /* return index of a specific point */
   QModelIndex index( int row, int column, const QModelIndex &parent = QModelIndex() ) const;
   /* return parent of an index */
   QModelIndex parent(const QModelIndex &index) const;
   /* return number of rows of an index */
   int rowCount( const QModelIndex &parent = QModelIndex() ) const;
   /* return number of columns of an index */
   int columnCount( const QModelIndex &parent = QModelIndex() ) const;
   /* set the complete model data */
   void setModelData( const QStringList &filenames );
   /* add one data */
   void addModelData( const QString &filename );
   /* get the full path of an index */
   QString getPath( const QModelIndex &index ) const;

private:
   FileSysTreeModel( const FileSysTreeModel &other );
   FileSysTreeModel &operator=( const FileSysTreeModel &other );

   FileSysTreeItem *mpRootItem;
};

#endif
