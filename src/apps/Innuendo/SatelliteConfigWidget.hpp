/*
 * src/apps/Sorcerer/SatelliteConfigWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef SATELLITECONFIGWIDGET_HPP
#define SATELLITECONFIGWIDGET_HPP SATELLITECONFIGWIDGET_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QCheckBox;
class QSignalMapper;

/* forward declaration of local classes */



/*!
  \addtogroup Innuendo

  @{
*/

/*!
 \brief TODO

 \dotfile "graphs/apps/Innuendo/SatelliteConfigWidget_connect.dot" "Connect Graph"
*/
class SatelliteConfigWidget : public QWidget
{
   Q_OBJECT

public:
   SatelliteConfigWidget( QWidget *parent = 0 );
   virtual ~SatelliteConfigWidget();
   /*!
    \brief read the settings

   */
   void readSettings();
   /*!
    \brief write the settings

   */
   void writeSettings();

public slots:
   /*!
    \brief handshake from global "use satellite"

   */
   void setInnuendoClicked( bool isSet );
   /*!
    \brief handle "Full Communication"

   */
   void setAll( bool isSet = true );
   /*!
    \brief handle "No Communication"

   */
   void setNone();

signals:
   /*!
    \brief handshake to global "use satellite"

   */
   void useInnuendoClicked( bool isSet );
   /*!
    \brief signal full or no communication clicked

   */
   void fullOrNoCommunication();

private:
   SatelliteConfigWidget( const SatelliteConfigWidget &that );
   SatelliteConfigWidget &operator=( const SatelliteConfigWidget &that );

   QList<QCheckBox*>    mButtonList;
};

/*! @} */

#endif
