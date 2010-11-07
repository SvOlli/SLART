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

   foreach( const QString &name, names )
   {
      QCheckBox *cb = new QCheckBox( name, this );
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
   foreach( QAbstractButton *button, mButtonList )
   {
      button->setChecked( MySettings( button->text() ).VALUE_USE_SATELLITE );
   }
   emit useInnuendoClicked( mButtonList.at(0)->isChecked() );
}


void SatelliteConfigWidget::writeSettings()
{
   foreach( QAbstractButton *button, mButtonList )
   {
      MySettings( button->text() ).setValue( "UseSatellite", button->isChecked() );
   }
}


void SatelliteConfigWidget::setInnuendoClicked( bool isSet )
{
   mButtonList.at(0)->setChecked( isSet );
}


void SatelliteConfigWidget::setAll( bool isSet )
{
   foreach( QAbstractButton *button, mButtonList )
   {
      button->setChecked( isSet );
   }
   emit useInnuendoClicked( isSet );
   emit fullOrNoCommunication();
}


void SatelliteConfigWidget::setNone()
{
   setAll( false );
}
