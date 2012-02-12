/*
 * src/apps/Partyman/FileSysTreeModel.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef FILESYSTREEMODEL_HPP
#define FILESYSTREEMODEL_HPP FILESYSTREEMODEL_HPP

/* base class */
#include <QAbstractItemModel>

/* system headers */

/* Qt headers */
#include <QStringList>
#include <QWidget>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */
class FileSysTreeItem;


/*!
  \addtogroup Partyman

  @{
*/

/*!
 \brief \todo complete documentation

*/
class FileSysTreeModel : public QAbstractItemModel
{
   Q_OBJECT

public:
   FileSysTreeModel( QObject *parent = 0 );
   virtual ~FileSysTreeModel();

   /*!
    \brief clear the complete model

   */
   void clear();
   /*!
    \brief return the data of an index

   */
   QVariant data( const QModelIndex &index, int role ) const;
   /*!
    \brief set item flags to enabled

   */
   Qt::ItemFlags flags(const QModelIndex &index) const;
   /*!
    \brief for dragging

   */
   Qt::DropActions supportedDragActions() const;
   /*!
    \brief for dragging: set the mime data of files

   */
   virtual QMimeData *mimeData( const QModelIndexList &indexes ) const;
   /*!
    \brief for dragging: set the mime type to file urls

   */
   virtual QStringList mimeTypes() const;
   /*!
    \brief return index of a specific point

   */
   QModelIndex index( int row, int column, const QModelIndex &parent = QModelIndex() ) const;
   /*!
    \brief return parent of an index

   */
   QModelIndex parent(const QModelIndex &index) const;
   /*!
    \brief return number of rows of an index

   */
   int rowCount( const QModelIndex &parent = QModelIndex() ) const;
   /*!
    \brief return number of columns of an index

   */
   int columnCount( const QModelIndex &parent = QModelIndex() ) const;
   /*!
    \brief set the complete model data

   */
   void setModelData( const QStringList &filenames );
   /*!
    \brief add one data

   */
   void addModelData( const QString &filename );
   /*!
    \brief get the full path of an index

   */
   QString getPath( const QModelIndex &index ) const;

private:
   FileSysTreeModel( const FileSysTreeModel &that );
   FileSysTreeModel &operator=( const FileSysTreeModel &that );

   FileSysTreeItem *mpRootItem;
};

/*! @} */

#endif
