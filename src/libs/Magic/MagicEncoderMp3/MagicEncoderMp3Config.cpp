/*
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
#include <QAction>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>

/* local library headers */
#include <ScrollLine.hpp>
#include <Settings.hpp>

/* local headers */
#include "MagicEncoderMp3.hpp"


MagicEncoderMp3Config::MagicEncoderMp3Config( MagicEncoderMp3 *encoder,
                                              QWidget *parent,
                                              QAction *toggleEnableAction )
: MagicEncoderConfig( parent, toggleEnableAction )
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
   bool enabled = Settings::value( Settings::Magicmp3UseEncoder );
   mpEncoder->mpToggleEnableAction->setChecked( enabled );
   mpEncoder->mDirOverride = Settings::value( Settings::Magicmp3DirectoryOverride );
   mpEncoder->mDirectory   = Settings::value( Settings::Magicmp3Directory );
   mpEncoder->mUseLatin1   = Settings::value( Settings::Magicmp3UseLatin1 );
   mpEncoder->mQuality     = Settings::value( Settings::Magicmp3VBRQuality );
   mpUseEncoder->setChecked( enabled );
   mpDirOverride->setChecked( mpEncoder->mDirOverride );
   mpDirEdit->setText( mpEncoder->mDirectory );
   mpQuality->setValue( mpEncoder->mQuality );
   mpUseLatin1->setChecked( mpEncoder->mUseLatin1 );
}


void MagicEncoderMp3Config::writeSettings()
{
   bool enabled = mpUseEncoder->isChecked();
   mpEncoder->mpToggleEnableAction->setChecked( enabled );
   mpEncoder->mDirOverride = mpDirOverride->isChecked();
   mpEncoder->mDirectory   = mpDirEdit->text();
   mpEncoder->mQuality     = mpQuality->value();
   mpEncoder->mUseLatin1   = mpUseLatin1->isChecked();
   Settings::setValue( Settings::Magicmp3UseEncoder, enabled );
   Settings::setValue( Settings::Magicmp3DirectoryOverride, mpEncoder->mDirOverride );
   Settings::setValue( Settings::Magicmp3Directory, mpEncoder->mDirectory );
   Settings::setValue( Settings::Magicmp3VBRQuality, mpEncoder->mQuality );
   Settings::setValue( Settings::Magicmp3UseLatin1, mpEncoder->mUseLatin1 );
}
