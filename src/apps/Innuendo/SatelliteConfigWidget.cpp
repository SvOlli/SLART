/**
 * src/apps/Sorcerer/SatelliteConfigWidget.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "SatelliteConfigWidget.hpp"

/* system headers */

/* Qt headers */
#include <QtGui>

/* local library headers */
#include <MySettings.hpp>
#include <Satellite.hpp>

/* local headers */


SatelliteConfigWidget::SatelliteConfigWidget( QWidget *parent )
: QWidget( parent )
, mButtonList()
{
   QStringList names;
   names << "Innuendo"
         << "Partyman"
         << "Rubberbandman"
         << "Karmadrome"
         << "Stripped"
         << "Funkytown"
         << "Notorious"
         << "Creep"
         ;

   QBoxLayout *layout = new QVBoxLayout( this );

   layout->addWidget( new QLabel( tr("Checked applications will participate in communication"), this ) );

   for( int i = 0; i < names.size(); i++ )
   {
      QCheckBox *cb = new QCheckBox( names.at(i), this );
      mButtonList.append( cb );
      layout->addWidget( cb );
   }

   QBoxLayout *buttonLayout = new QHBoxLayout();
   buttonLayout->setContentsMargins( 0, 0, 0, 0 );
   QPushButton *fullCom = new QPushButton( tr("Full Communication"), this );
   QPushButton *noCom = new QPushButton( tr("No Communication"), this );
   buttonLayout->addWidget( fullCom );
   buttonLayout->addWidget( noCom );
   layout->addLayout( buttonLayout );

   connect( mButtonList.at(0), SIGNAL(clicked(bool)),
            this, SIGNAL(useInnuendoClicked(bool)) );
   connect( fullCom, SIGNAL(clicked()),
            this, SLOT(setAll()) );
   connect( noCom, SIGNAL(clicked()),
            this, SLOT(setNone()) );

   setLayout( layout );
}

SatelliteConfigWidget::~SatelliteConfigWidget()
{
}


void SatelliteConfigWidget::readSettings()
{
   for( int i = 0; i < mButtonList.size(); i++ )
   {
      mButtonList.at(i)->setChecked( MySettings( mButtonList.at(i)->text() ).VALUE_USE_SATELLITE );
   }
   emit useInnuendoClicked( mButtonList.at(0)->isChecked() );
}


void SatelliteConfigWidget::writeSettings()
{
   for( int i = 0; i < mButtonList.size(); i++ )
   {
      MySettings( mButtonList.at(i)->text() ).setValue( "UseSatellite", mButtonList.at(i)->isChecked() );
   }
}


void SatelliteConfigWidget::setInnuendoClicked( bool isSet )
{
   mButtonList.at(0)->setChecked( isSet );
}


void SatelliteConfigWidget::setAll( bool isSet )
{
   for( int i = 0; i < mButtonList.size(); i++ )
   {
      mButtonList.at(i)->setChecked( isSet );
   }
   emit useInnuendoClicked( isSet );
   emit fullOrNoCommunication();
}


void SatelliteConfigWidget::setNone()
{
   setAll( false );
}
