/**
 * FileSysBrowser.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "FileSysBrowser.hpp"

#include <QtGui>
#include <QString>

//#include "FileSysTreeView.hpp"
#include "FileSysTreeModel.hpp"

#include "Trace.hpp"


FileSysBrowser::FileSysBrowser( const QDir &rootDir, QWidget *parent, Qt::WindowFlags flags )
: QWidget( parent, flags )
, mRootDir( rootDir )
, mpFileSysTreeView( 0 )
, mpFileSysTreeModel( 0 )
{
   construct();
}


FileSysBrowser::FileSysBrowser( const QString &rootPath, QWidget *parent, Qt::WindowFlags f )
: QWidget( parent, f )
, mRootDir( )
, mpFileSysTreeView( 0 )
, mpFileSysTreeModel( 0 )
{
   mRootDir.setPath( rootPath );
   construct();
}


void FileSysBrowser::construct()
{
//   mpFileSysTreeView  = new FileSysTreeView( this );
   mpFileSysTreeView  = new QTreeView( this );
   mpFileSysTreeModel = new FileSysTreeModel( mRootDir.absolutePath(), this );
   
   mpFileSysTreeView->header()->hide();
   mpFileSysTreeView->setModel( mpFileSysTreeModel );
//   connect(mpFileSysTreeView, SIGNAL(context(QModelIndex)), this, SLOT(addEntries(QModelIndex)));
   QHBoxLayout *layout = new QHBoxLayout( this );
   layout->addWidget( mpFileSysTreeView );
   layout->setMargin( 2 );
   setLayout(layout);
   
   connect(mpFileSysTreeView, SIGNAL(clicked(QModelIndex)), this, SLOT(entryClicked(QModelIndex)));
}


void FileSysBrowser::entryClicked( const QModelIndex &index )
{
   emit clicked(mpFileSysTreeModel->getPath(index));
}
