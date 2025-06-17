/*
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
#include <QAction>
#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>

/* local library headers */
#include <ScrollLine.hpp>
#include <Settings.hpp>

/* local headers */
#include "MagicEncoderFlac.hpp"


MagicEncoderFlacConfig::MagicEncoderFlacConfig( MagicEncoderFlac *encoder,
                                                QWidget *parent,
                                                QAction *toggleEnableAction )
: MagicEncoderConfig( parent, toggleEnableAction )
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
   mainLayout->addWidget( mpDirEdit, 2, 1 );
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
   bool enabled = Settings::value( Settings::MagicFLACUseEncoder );
   mpEncoder->mpToggleEnableAction->setChecked( enabled );
   mpEncoder->mDirOverride = Settings::value( Settings::MagicFLACDirectoryOverride );
   mpEncoder->mDirectory   = Settings::value( Settings::MagicFLACDirectory );
   mpEncoder->mQuality     = Settings::value( Settings::MagicFLACFlacQuality );
   mpEncoder->mUseOga      = Settings::value( Settings::MagicFLACFlacUseOga );
   mpUseEncoder->setChecked( enabled );
   mpDirOverride->setChecked( mpEncoder->mDirOverride );
   mpDirEdit->setText( mpEncoder->mDirectory );
   mpQuality->setValue( mpEncoder->mQuality );
   mpUseOga->setChecked( mpEncoder->mUseOga );
   enableOverride( mpDirOverride->checkState() );
}


void MagicEncoderFlacConfig::writeSettings()
{
   bool enabled = mpUseEncoder->isChecked();
   mpEncoder->mpToggleEnableAction->setChecked( enabled );
   mpEncoder->mDirOverride = mpDirOverride->isChecked();
   mpEncoder->mDirectory   = mpDirEdit->text();
   mpEncoder->mQuality     = mpQuality->value();
   mpEncoder->mUseOga      = mpUseOga->isChecked();
   Settings::setValue( Settings::MagicFLACUseEncoder, enabled );
   Settings::setValue( Settings::MagicFLACDirectoryOverride, mpEncoder->mDirOverride );
   Settings::setValue( Settings::MagicFLACDirectory, mpEncoder->mDirectory );
   Settings::setValue( Settings::MagicFLACFlacQuality, mpEncoder->mQuality );
   Settings::setValue( Settings::MagicFLACFlacUseOga, mpEncoder->mUseOga );
}
