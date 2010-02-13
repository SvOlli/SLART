/**
 * src/apps/Sorcerer/SatelliteConfigWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef SATELLITECONFIGWIDGET_HPP
#define SATELLITECONFIGWIDGET_HPP SATELLITECONFIGWIDGET_HPP

#include <QWidget>

class QCheckBox;
class QSignalMapper;

class SatelliteConfigWidget : public QWidget
{
Q_OBJECT

public:
   SatelliteConfigWidget( QWidget *parent = 0 );
   virtual ~SatelliteConfigWidget();
   /*  */
   void readSettings();
   /*  */
   void writeSettings();

public slots:
   /*  */
   void setInnuendoClicked( bool isSet );
   /*  */
   void setAll( bool isSet = true );
   /*  */
   void setNone();

signals:
   /*  */
   void useInnuendoClicked( bool isSet );
   /*  */
   void fullOrNoCommunication();

private:
   SatelliteConfigWidget( const SatelliteConfigWidget &other );
   SatelliteConfigWidget &operator=( const SatelliteConfigWidget &other );

   QList<QCheckBox*>    mButtonList;
};

#endif
