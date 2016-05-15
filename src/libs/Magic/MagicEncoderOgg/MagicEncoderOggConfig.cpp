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
#include "MagicEncoderOgg.hpp"


#include <Trace.hpp>

MagicEncoderOggConfig::MagicEncoderOggConfig( MagicEncoderOgg *encoder,
                                              QWidget *parent,
                                              QAction *toggleEnableAction )
: MagicEncoderConfig( parent, toggleEnableAction )
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
   bool enabled = Settings::value( Settings::MagicoggUseEncoder );
   mpEncoder->mpToggleEnableAction->setChecked( enabled );
   mpEncoder->mDirOverride = Settings::value( Settings::MagicoggDirectoryOverride );
   mpEncoder->mDirectory   = Settings::value( Settings::MagicoggDirectory );
   mpEncoder->mQuality     = Settings::value( Settings::MagicoggOggQuality );
   mpUseEncoder->setChecked( enabled );
   mpDirOverride->setChecked( mpEncoder->mDirOverride );
   mpDirEdit->setText( mpEncoder->mDirectory );
   mpQuality->setValue( mpEncoder->mQuality );
}


void MagicEncoderOggConfig::writeSettings()
{
   bool enabled = mpUseEncoder->isChecked();
   mpEncoder->mpToggleEnableAction->setChecked( enabled );
   mpEncoder->mDirOverride = mpDirOverride->isChecked();
   mpEncoder->mDirectory   = mpDirEdit->text();
   mpEncoder->mQuality     = mpQuality->value();
   Settings::setValue( Settings::MagicoggUseEncoder, enabled );
   Settings::setValue( Settings::MagicoggDirectoryOverride, mpEncoder->mDirOverride );
   Settings::setValue( Settings::MagicoggDirectory, mpEncoder->mDirectory );
   Settings::setValue( Settings::MagicoggOggQuality, mpEncoder->mQuality );
}
