/**
 * src/libs/Common/GlobalConfigWidget.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
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


class GlobalConfigWidget : public QWidget
{
Q_OBJECT
   
public:
   GlobalConfigWidget( QWidget *parent = 0 );
   virtual ~GlobalConfigWidget();
   
   /* read settings */
   void readSettings();
   /* write settings */
   void writeSettings();
   /* write to the clipboard (copy and/or selection buffer) according to settings */
   static void setClipboard( const QString &text );
   /* read from clipboard (copy or selection buffer) according to settings */
   static QString getClipboard();
   /* remove/replace chars illegal for a fileName */
   static QString correctFileName( QString fileName, bool withDir );
   /* show clipboard options on panel */
   void showClipboard( bool allow = true );
   /* show animation option on panel */
   void showAnimate( bool allow = true );
   /* show normalize options on panel */
   void showNormalize( bool allow = true );
   /* show doubleclick interal options on panel */
   void showDoubleClickInterval( bool allow = true );

public slots:
   /* change the filename according to checkbox */
   void updateStyleSheetFileName();
   /* handle the click of the dots button */
   void selectFile();
   /*  */
   void setSatelliteClicked( bool isSet );
   
signals:
   /*  */
   void useSatelliteClicked( bool isSet );

private:
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

/* defaults */
#define VALUE_ANIMATEVIEWS             value("AnimateViews", false).toBool()
#define VALUE_CLIPBOARDMODE            value("ClipboardMode", 0).toInt()
#define VALUE_DOUBLECLICKINTERVAL      value("DoubleClickInterval", QApplication::doubleClickInterval() ).toInt()
#define VALUE_NORMALIZECASE            value("NormalizeCase", false ).toBool()
#define VALUE_NORMALIZESPACES          value("NormalizeSpaces", false ).toBool()
#define VALUE_USEGLOBALSTYLESHEETFILE  value("UseGlobalStyleSheetFile", true).toBool()
#define VALUE_STYLESHEETFILE           value("StyleSheetFile", QString()).toString()

#endif
