/**
 * src/apps/Magic/MagicEncoderFlac/MagicEncoderFlacConfig.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "MagicEncoderFlacConfig.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <MySettings.hpp>
#include <ScrollLine.hpp>

/* local headers */
#include "MagicEncoderFlac.hpp"


MagicEncoderFlacConfig::MagicEncoderFlacConfig( MagicEncoderFlac *encoder, QWidget *parent, QAbstractButton *button )
: MagicEncoderConfig( parent, button )
, mpEncoder( encoder )
, mpQuality( new QSpinBox( this ) )
, mpUseOga( new QCheckBox( tr("Use Ogg Container"), this ) )
{
   mpQuality->setSingleStep( 1 );
   mpQuality->setMinimum( 0 );
   mpQuality->setMaximum( 8 );
   mpQuality->setToolTip( tr("0 = fastest; 8 = best") );

   QGridLayout *mainLayout = new QGridLayout( this );
   mainLayout->setRowStretch( 5, 1 );
   mainLayout->setColumnStretch( 1, 1 );
   mainLayout->setContentsMargins( 3, 3, 3, 3 );
   mainLayout->addWidget( mpUseEncoder, 0, 0, 1, 3 );
   mainLayout->addWidget( mpDirOverride, 1, 0, 1, 3 );
   mainLayout->addWidget( new QLabel( tr("Base Directory:"), this ), 2, 0 );
   mainLayout->addWidget( mpDirectory, 2, 1 );
   mainLayout->addWidget( mpDotButton, 2, 2 );
   mainLayout->addWidget( new QLabel( tr("Compression Level:"), this ), 3, 0 );
   mainLayout->addWidget( mpQuality, 3, 1, 1, 2 );
   mainLayout->addWidget( mpUseOga,  4, 0, 1, 3 );

   setLayout( mainLayout );

   readSettings();
}


MagicEncoderFlacConfig::~MagicEncoderFlacConfig()
{
}


void MagicEncoderFlacConfig::readSettings()
{
   MySettings settings;
   settings.beginGroup( mpEncoder->mName );
   mpEncoder->mUseEncoder  = settings.VALUE_USE_ENCODER;
   mpEncoder->mDirOverride = settings.VALUE_DIRECTORY_OVERRIDE;
   mpEncoder->mDirectory   = settings.VALUE_DIRECTORY;
   mpEncoder->mQuality     = settings.VALUE_FLACQUALITY;
   mpEncoder->mUseOga      = settings.VALUE_FLACUSEOGA;
   mpUseEncoder->setChecked( mpEncoder->mUseEncoder );
   mpExternalUseEncoder->setChecked( mpEncoder->mUseEncoder );
   mpDirOverride->setChecked( mpEncoder->mDirOverride );
   mpDirectory->setText( mpEncoder->mDirectory );
   mpQuality->setValue( mpEncoder->mQuality );
   mpUseOga->setChecked( mpEncoder->mUseOga );
   settings.endGroup();
}


void MagicEncoderFlacConfig::writeSettings()
{
   MySettings settings;
   settings.beginGroup( mpEncoder->mName );
   mpEncoder->mUseEncoder  = mpUseEncoder->isChecked();
   mpEncoder->mDirOverride = mpDirOverride->isChecked();
   mpEncoder->mDirectory   = mpDirectory->text();
   mpEncoder->mQuality     = mpQuality->value();
   mpEncoder->mUseOga      = mpUseOga->isChecked();
   settings.setValue( "UseEncoder", mpEncoder->mUseEncoder );
   settings.setValue( "DirectoryOverride", mpEncoder->mDirOverride );
   settings.setValue( "Directory", mpEncoder->mDirectory );
   settings.setValue( "FlacQuality", mpEncoder->mQuality );
   settings.setValue( "FlacUseOga", mpEncoder->mUseOga );
   settings.endGroup();
}
