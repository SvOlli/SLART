/*
 * src/apps/Magic/MagicEncoder/MagicEncoderConfig.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "MagicEncoderConfig.hpp"

/* system headers */

/* Qt headers */
#include <QAction>
#include <QCheckBox>
#include <QCompleter>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QLineEdit>
#include <QPushButton>


/* local library headers */
#include <ScrollLine.hpp>

/* local headers */

#include <QtDebug>

MagicEncoderConfig::MagicEncoderConfig( QWidget *parent, QAction *toggleEnableAction )
: QWidget( parent )
, mpUseEncoder( new QCheckBox( tr("Use This Encoder"), this ) )
, mpDirOverride( new QCheckBox( tr("Override Base Directory"), this ) )
, mpDirEdit( new QLineEdit( this ) )
, mpDotButton( new QPushButton( "...", this ) )
{
   QCompleter *completer = new QCompleter( this );
   QFileSystemModel *dirsModel = new QFileSystemModel( completer );
   dirsModel->setFilter( QDir::NoDotAndDotDot | QDir::AllDirs );
   mpDirEdit->setCompleter( completer );

   /* evil hack */
   mpDotButton->setMaximumWidth( mpDotButton->height() );

   toggleEnableAction->setCheckable( true );
   connect( mpUseEncoder, SIGNAL(toggled(bool)),
            toggleEnableAction, SLOT(setChecked(bool)) );
   connect( toggleEnableAction, SIGNAL(toggled(bool)),
            mpUseEncoder, SLOT(setChecked(bool)) );
   connect( mpDotButton, SIGNAL(clicked()),
            this, SLOT(selectDirectory()) );
}


MagicEncoderConfig::~MagicEncoderConfig()
{
}


void MagicEncoderConfig::selectDirectory()
{
   //qDebug() << mpDirEdit->text();
   QFileDialog fileDialog( mpDirEdit );

   fileDialog.setOption( QFileDialog::ShowDirsOnly, true );
   fileDialog.setDirectory( mpDirEdit->text() );
   fileDialog.setReadOnly( false );

   if( fileDialog.exec() )
   {
      QString result( fileDialog.selectedFiles().at(0) );
      mpDirEdit->setText( result );
   }
}
