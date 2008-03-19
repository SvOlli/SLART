/**
 * FileSysBrowser.cpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include "FileSysBrowser.hpp"
#include "MySettings.hpp"

#include <QtGui>
#include <QString>

#include "Trace.hpp"


FileSysBrowser::FileSysBrowser( QWidget *parent, Qt::WindowFlags flags )
: QWidget( parent, flags )
, mpRootDir( new QLineEdit( this ) )
, mpSetButton( new QPushButton( tr("Set"), this ) )
, mpView( new QTreeView( this ) )
, mpModel( new QDirModel( this ) )
{
   MySettings settings;
   QStringList filterNames;
   filterNames << "*.mp3" << "*.ogg";
   mpModel->setNameFilters( filterNames );
   mpModel->setFilter( QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files );
   mpModel->setSorting( QDir::Name | QDir::DirsFirst | QDir::IgnoreCase | QDir::LocaleAware );
   mpModel->setLazyChildCount( true );
   
   mpView->setModel( mpModel );
   
//   connect(mpView, SIGNAL(context(QModelIndex)), this, SLOT(addEntries(QModelIndex)));
   QGridLayout *layout = new QGridLayout( this );
   layout->addWidget( new QLabel(tr("Root:")), 0, 0 );
   layout->addWidget( mpRootDir,               0, 1 );
   layout->addWidget( mpSetButton,             0, 2 );
   layout->addWidget( mpView,                  1, 0, 1, 3 );
   layout->setColumnStretch( 1, 1 );
   layout->setMargin( 2 );
   setLayout(layout);
   
   mpView->header()->hide();
   mpView->setColumnHidden( 1, true );
   mpView->setColumnHidden( 2, true );
   mpView->setColumnHidden( 3, true );
   
   mpRootDir->setText( settings.value( "RootDirectory", QString("/") ).toString() );
   mpView->setRootIndex( mpModel->index( mpRootDir->text() ) );
   mpView->setAnimated( true );
   connect( mpView, SIGNAL(clicked(const QModelIndex&)),
            this, SLOT(entryClicked(const QModelIndex&)) );
   connect( this, SIGNAL(clickedDir(const QString&)),
            mpRootDir, SLOT( setText(const QString&)) );
   connect( mpRootDir, SIGNAL(returnPressed()),
            this, SLOT(handleRootDir()) );
   connect( mpSetButton, SIGNAL(clicked()),
            this, SLOT(handleRootDir()) );
}


void FileSysBrowser::entryClicked( const QModelIndex &index )
{
   QString path( mpModel->filePath(index) );
   
   if( QFileInfo( path ).isDir() )
   {
      emit clickedDir( path );
   }
   emit clicked( path );
}


void FileSysBrowser::handleRootDir()
{
   MySettings().setValue( "RootDirectory", mpRootDir->text() );
   mpView->setRootIndex( mpModel->index( mpRootDir->text() ) );
}
