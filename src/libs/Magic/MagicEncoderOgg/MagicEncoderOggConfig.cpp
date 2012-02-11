/*
 * src/apps/Magic/MagicEncoderOgg/MagicEncoderOggConfig.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "MagicEncoderOggConfig.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <MySettings.hpp>
#include <ScrollLine.hpp>

/* local headers */
#include "MagicEncoderOgg.hpp"


#include <Trace.hpp>

MagicEncoderOggConfig::MagicEncoderOggConfig( MagicEncoderOgg *encoder, QWidget *parent, QAbstractButton *button )
: MagicEncoderConfig( parent, button )
, mpEncoder( encoder )
, mpQuality( new QDoubleSpinBox( this ) )
{
   mpQuality->setSingleStep( 0.01 );
   mpQuality->setMinimum( 0.0 );
   mpQuality->setMaximum( 1.0 );
   mpQuality->setToolTip( tr("0 = low & short, 1 = high & long") );

   QGridLayout *mainLayout = new QGridLayout( this );
   mainLayout->setRowStretch( 4, 1 );
   mainLayout->setColumnStretch( 1, 1 );
   mainLayout->setContentsMargins( 3, 3, 3, 3 );
   mainLayout->addWidget( mpUseEncoder, 0, 0, 1, 3 );
   mainLayout->addWidget( mpDirOverride, 1, 0, 1, 3 );
   mainLayout->addWidget( new QLabel( tr("Base Directory:"), this ), 2, 0 );
   mainLayout->addWidget( mpDirEdit, 2, 1 );
   mainLayout->addWidget( mpDotButton, 2, 2 );
   mainLayout->addWidget( new QLabel( tr("Quality:"), this ), 3, 0 );
   mainLayout->addWidget( mpQuality, 3, 1, 1, 2 );

   setLayout( mainLayout );

   readSettings();
}


MagicEncoderOggConfig::~MagicEncoderOggConfig()
{
}


void MagicEncoderOggConfig::readSettings()
{
   MySettings settings;
   settings.beginGroup( mpEncoder->mName );
   mpEncoder->mUseEncoder  = settings.VALUE_USE_ENCODER;
   mpEncoder->mDirOverride = settings.VALUE_DIRECTORY_OVERRIDE;
   mpEncoder->mDirectory   = settings.VALUE_DIRECTORY;
   mpEncoder->mQuality     = settings.VALUE_OGGQUALITY;
   mpUseEncoder->setChecked( mpEncoder->mUseEncoder );
   mpDirOverride->setChecked( mpEncoder->mDirOverride );
   mpDirEdit->setText( mpEncoder->mDirectory );
   mpQuality->setValue( mpEncoder->mQuality );
   settings.endGroup();
}


void MagicEncoderOggConfig::writeSettings()
{
   MySettings settings;
   settings.beginGroup( mpEncoder->mName );
   mpEncoder->mUseEncoder  = mpUseEncoder->isChecked();
   mpEncoder->mDirOverride = mpDirOverride->isChecked();
   mpEncoder->mDirectory   = mpDirEdit->text();
   mpEncoder->mQuality     = mpQuality->value();
   settings.setValue( "UseEncoder", mpEncoder->mUseEncoder );
   settings.setValue( "DirectoryOverride", mpEncoder->mDirOverride );
   settings.setValue( "Directory", mpEncoder->mDirectory );
   settings.setValue( "OggQuality", mpEncoder->mQuality );
   settings.endGroup();
}
