/*
 * src/libs/Common/GlobalConfigWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef GLOBALCONFIGWIDGET_HPP
#define GLOBALCONFIGWIDGET_HPP GLOBALCONFIGWIDGET_HPP

#include <QGroupBox>

class QCheckBox;
class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QSpinBox;


/*!
  \addtogroup Common

  @{
*/

/*!
 \brief TODO

 \dotfile "graphs/libs/Common/GlobalConfigWidget_connect.dot" "Connect Graph"
*/
class GlobalConfigWidget : public QWidget
{
   Q_OBJECT

public:
   GlobalConfigWidget( QWidget *parent = 0 );
   virtual ~GlobalConfigWidget();

   /*!
    \brief read settings

   */
   void readSettings();
   /*!
    \brief write settings

   */
   void writeSettings();
   /*!
    \brief write to the clipboard (copy and/or selection buffer) according to settings

   */
   static void setClipboard( const QString &text );
   /*!
    \brief read from clipboard (copy or selection buffer) according to settings

   */
   static QString getClipboard();
   /*!
    \brief remove/replace chars illegal for a fileName

   */
   static QString correctFileName( QString fileName, bool withDir );
   /*!
    \brief show clipboard options on panel

   */
   void showClipboard( bool allow = true );
   /*!
    \brief show animation option on panel

   */
   void showAnimate( bool allow = true );
   /*!
    \brief show normalize options on panel

   */
   void showNormalize( bool allow = true );
   /*!
    \brief show doubleclick interal options on panel

   */
   void showDoubleClickInterval( bool allow = true );

public slots:
   /*!
    \brief change the filename according to checkbox

   */
   void updateStyleSheetFileName();
   /*!
    \brief handle the click of the dots button

   */
   void selectFile();
   /*!
    \brief set the "use satellite" checkbox

   */
   void setSatelliteClicked( bool isSet );

signals:
   /*!
    \brief signalize that the "use satellite" checkbox has been clicked

   */
   void useSatelliteClicked( bool isSet );

private:
   Q_DISABLE_COPY( GlobalConfigWidget )

   QCheckBox    *mpUseSatellite;
   QLabel       *mpSatellitePortLabel;
   QSpinBox     *mpSatellitePort;
   QCheckBox    *mpUseGlobalStyleSheetFile;
   QLineEdit    *mpStyleSheetFileName;
   QPushButton  *mpDotButton;
   QLabel       *mpClipboardLabel;
   QComboBox    *mpClipboardSelection;
   QCheckBox    *mpAnimateViews;
   QCheckBox    *mpNormalizeCase;
   QCheckBox    *mpNormalizeSpaces;
   QLabel       *mpDoubleClickLabel;
   QSpinBox     *mpDoubleClickInterval;
};

/*! @} */

/* defaults */
#define VALUE_ANIMATEVIEWS             value("AnimateViews", false).toBool()
#define VALUE_CLIPBOARDMODE            value("ClipboardMode", 0).toInt()
#define VALUE_DOUBLECLICKINTERVAL      value("DoubleClickInterval", QApplication::doubleClickInterval() ).toInt()
#define VALUE_NORMALIZECASE            value("NormalizeCase", false ).toBool()
#define VALUE_NORMALIZESPACES          value("NormalizeSpaces", false ).toBool()
#define VALUE_USEGLOBALSTYLESHEETFILE  value("UseGlobalStyleSheetFile", true).toBool()
#define VALUE_STYLESHEETFILE           value("StyleSheetFile", QString()).toString()

#endif
