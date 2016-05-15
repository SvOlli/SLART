/*
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
#include "MagicEncoderWav.hpp"


MagicEncoderWavConfig::MagicEncoderWavConfig( MagicEncoderWav *encoder,
                                              QWidget *parent,
                                              QAction *toggleEnableAction )
: MagicEncoderConfig( parent, toggleEnableAction )
, mpEncoder( encoder )
{
   QGridLayout *mainLayout = new QGridLayout( this );
   mainLayout->setContentsMargins( 3, 3, 3, 3 );
   mainLayout->addWidget( mpUseEncoder, 0, 0, 1, 3 );
   mainLayout->addWidget( mpDirOverride, 1, 0, 1, 3 );
   mainLayout->addWidget( new QLabel( tr("Base Directory:") ), 2, 0 );
   mainLayout->addWidget( mpDirEdit, 2, 1 );
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
   bool enabled = Settings::value( Settings::MagicwavUseEncoder );
   mpEncoder->mpToggleEnableAction->setChecked( enabled );
   mpEncoder->mDirOverride = Settings::value( Settings::MagicwavDirectoryOverride );
   mpEncoder->mDirectory   = Settings::value( Settings::MagicwavDirectory );
   mpUseEncoder->setChecked( enabled );
   mpDirOverride->setChecked( mpEncoder->mDirOverride );
   mpDirEdit->setText( mpEncoder->mDirectory );
}


void MagicEncoderWavConfig::writeSettings()
{
   bool enabled = mpUseEncoder->isChecked();
   mpEncoder->mpToggleEnableAction->setChecked( enabled );
   mpEncoder->mDirOverride = mpDirOverride->isChecked();
   mpEncoder->mDirectory   = mpDirEdit->text();
   Settings::setValue( Settings::MagicwavUseEncoder, enabled );
   Settings::setValue( Settings::MagicwavDirectoryOverride, mpEncoder->mDirOverride );
   Settings::setValue( Settings::MagicwavDirectory, mpEncoder->mDirectory );
}
