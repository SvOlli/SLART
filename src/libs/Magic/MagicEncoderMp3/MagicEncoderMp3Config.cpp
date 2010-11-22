/**
 * src/apps/Magic/MagicEncoderMp3/MagicEncoderMp3Config.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "MagicEncoderMp3Config.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <MySettings.hpp>
#include <ScrollLine.hpp>

/* local headers */
#include "MagicEncoderMp3.hpp"


MagicEncoderMp3Config::MagicEncoderMp3Config( MagicEncoderMp3 *encoder, QWidget *parent, QAbstractButton *button )
: MagicEncoderConfig( parent, button )
, mpEncoder( encoder )
, mpQuality( new QDoubleSpinBox( this ) )
, mpUseLatin1( new QCheckBox( tr("Use Latin1 instead of Utf-8 for tags"), this ) )
{
   mpQuality->setDecimals( 1 );
   mpQuality->setSingleStep( 0.1 );
   mpQuality->setMinimum( 0.0 );
   mpQuality->setMaximum( 9.9 );
   mpQuality->setToolTip( tr("0 = high & long, 9 = low & short") );

   QGridLayout *mainLayout = new QGridLayout( this );
   mainLayout->setContentsMargins( 3, 3, 3, 3 );
   mainLayout->addWidget( mpUseEncoder, 0, 0, 1, 3 );
   mainLayout->addWidget( mpDirOverride, 1, 0, 1, 3 );
   mainLayout->addWidget( new QLabel( tr("Base Directory:"), this ), 2, 0 );
   mainLayout->addWidget( mpDirEdit, 2, 1 );
   mainLayout->addWidget( mpDotButton, 2, 2 );
   mainLayout->addWidget( new QLabel( tr("VBR Quality:"), this ), 3, 0 );
   mainLayout->addWidget( mpQuality, 3, 1, 1, 2 );
   mainLayout->addWidget( mpUseLatin1, 4, 0, 1, 3 );
   mainLayout->setColumnStretch( 1, 1 );
   mainLayout->setRowStretch( 5, 1 );

   setLayout( mainLayout );

   readSettings();
}


MagicEncoderMp3Config::~MagicEncoderMp3Config()
{
}


void MagicEncoderMp3Config::readSettings()
{
   MySettings settings;
   settings.beginGroup( mpEncoder->mName );
   mpEncoder->mUseEncoder  = settings.VALUE_USE_ENCODER;
   mpEncoder->mDirOverride = settings.VALUE_DIRECTORY_OVERRIDE;
   mpEncoder->mDirectory   = settings.VALUE_DIRECTORY;
   mpEncoder->mUseLatin1   = settings.VALUE_USE_LATIN1;
   mpEncoder->mQuality     = settings.VALUE_VBRQUALITY;
   mpUseEncoder->setChecked( mpEncoder->mUseEncoder );
   mpDirOverride->setChecked( mpEncoder->mDirOverride );
   mpDirEdit->setText( mpEncoder->mDirectory );
   mpQuality->setValue( mpEncoder->mQuality );
   mpUseLatin1->setChecked( mpEncoder->mUseLatin1 );
   settings.endGroup();
}


void MagicEncoderMp3Config::writeSettings()
{
   MySettings settings;
   settings.beginGroup( mpEncoder->mName );
   mpEncoder->mUseEncoder  = mpUseEncoder->isChecked();
   mpEncoder->mDirOverride = mpDirOverride->isChecked();
   mpEncoder->mDirectory   = mpDirEdit->text();
   mpEncoder->mQuality     = mpQuality->value();
   mpEncoder->mUseLatin1   = mpUseLatin1->isChecked();
   settings.setValue( "UseEncoder", mpEncoder->mUseEncoder );
   settings.setValue( "DirectoryOverride", mpEncoder->mDirOverride );
   settings.setValue( "Directory", mpEncoder->mDirectory );
   settings.setValue( "OggQuality", mpEncoder->mQuality );
   settings.setValue( "UseLatin1", mpEncoder->mUseLatin1 );
   settings.endGroup();
}
