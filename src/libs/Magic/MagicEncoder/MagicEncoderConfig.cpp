/**
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
#include <QtGui>

/* local library headers */
#include <ScrollLine.hpp>

/* local headers */

#include <QtDebug>

MagicEncoderConfig::MagicEncoderConfig( QWidget *parent, QAbstractButton *button )
: QWidget( parent )
, mpExternalUseEncoder( button )
, mpUseEncoder( new QCheckBox( tr("Use This Encoder"), this ) )
, mpDirOverride( new QCheckBox( tr("Override Base Directory"), this ) )
, mpDirEdit( new QLineEdit( this ) )
, mpDotButton( new QPushButton( "...", this ) )
{
   QCompleter *completer = new QCompleter( this );
   completer->setModel( new QDirModel( QStringList(),
                                       QDir::NoDotAndDotDot | QDir::AllDirs,
                                       QDir::Name,
                                       completer ) );
   mpDirEdit->setCompleter( completer );

   /* evil hack */
   mpDotButton->setMaximumWidth( mpDotButton->height() );

   if( button )
   {
      connect( button, SIGNAL(clicked(bool)),
               mpUseEncoder, SLOT(setChecked(bool)) );
      connect( mpUseEncoder, SIGNAL(clicked(bool)),
               button, SLOT(setChecked(bool)) );
   }
   connect( mpDotButton, SIGNAL(clicked()),
            this, SLOT(selectDirectory()) );
}


MagicEncoderConfig::~MagicEncoderConfig()
{
}


void MagicEncoderConfig::selectDirectory()
{
   qDebug() << mpDirEdit->text();
   QFileDialog fileDialog( mpDirEdit );

   fileDialog.setFileMode( QFileDialog::DirectoryOnly );
   fileDialog.setDirectory( mpDirEdit->text() );
   fileDialog.setReadOnly( false );

   if( fileDialog.exec() )
   {
      QString result( fileDialog.selectedFiles().at(0) );
      mpDirEdit->setText( result );
   }
}
