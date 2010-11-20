/**
 * src/apps/Magic/MagicEncoderWav/MagicEncoderWavConfig.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "MagicEncoderWavConfig.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <MySettings.hpp>
#include <ScrollLine.hpp>

/* local headers */
#include "MagicEncoderWav.hpp"


MagicEncoderWavConfig::MagicEncoderWavConfig( MagicEncoderWav *encoder, QWidget *parent, QAbstractButton *button )
: MagicEncoderConfig( parent, button )
, mpEncoder( encoder )
{
   QGridLayout *mainLayout = new QGridLayout( this );
   mainLayout->setContentsMargins( 3, 3, 3, 3 );
   mainLayout->addWidget( mpUseEncoder, 0, 0, 1, 3 );
   mainLayout->addWidget( mpDirOverride, 1, 0, 1, 3 );
   mainLayout->addWidget( new QLabel( tr("Base Directory:") ), 2, 0 );
   mainLayout->addWidget( mpDirectory, 2, 1 );
   mainLayout->addWidget( mpDotButton, 2, 2 );
   mainLayout->setColumnStretch( 1, 1 );
   mainLayout->setRowStretch( 3, 1 );

   setLayout( mainLayout );

   readSettings();
}


MagicEncoderWavConfig::~MagicEncoderWavConfig()
{
}


void MagicEncoderWavConfig::readSettings()
{
   MySettings settings;
   settings.beginGroup( mpEncoder->mName );
   mpEncoder->mUseEncoder  = settings.VALUE_USE_ENCODER;
   mpEncoder->mDirOverride = settings.VALUE_DIRECTORY_OVERRIDE;
   mpEncoder->mDirectory   = settings.VALUE_DIRECTORY;
   mpUseEncoder->setChecked( mpEncoder->mUseEncoder );
   mpDirOverride->setChecked( mpEncoder->mDirOverride );
   mpDirectory->setText( mpEncoder->mDirectory );
   settings.endGroup();
}


void MagicEncoderWavConfig::writeSettings()
{
   MySettings settings;
   settings.beginGroup( mpEncoder->mName );
   mpEncoder->mUseEncoder  = mpUseEncoder->isChecked();
   mpEncoder->mDirOverride = mpDirOverride->isChecked();
   mpEncoder->mDirectory   = mpDirectory->text();
   settings.setValue( "UseEncoder", mpEncoder->mUseEncoder );
   settings.setValue( "DirectoryOverride", mpEncoder->mDirOverride );
   settings.setValue( "Directory", mpEncoder->mDirectory );
   settings.endGroup();
}
