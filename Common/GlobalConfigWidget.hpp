/**
 * GlobalConfigWidget.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef GLBOALCONFIGWIDGET_HPP
#define GLBOALCONFIGWIDGET_HPP GLBOALCONFIGWIDGET_HPP

#include <QGroupBox>

class QCheckBox;
class QComboBox;
class QLabel;


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
   /* show clipboard options on panel */
   void showClipboard( bool allow = true );
   /* show animation option on panel */
   void showAnimate( bool allow = true );
   /* show normalize options on panel */
   void showNormalize( bool allow = true );
   
private:
   QLabel       *mpClipboardLabel;
   QComboBox    *mpClipboardSelection;
   QCheckBox    *mpAnimateViews;
   QCheckBox    *mpNormalizeCase;
   QCheckBox    *mpNormalizeSpaces;
};

#endif
