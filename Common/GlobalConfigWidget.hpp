/**
 * GlobalConfigWidget.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef GLBOALCONFIGWIDGET_HPP
#define GLBOALCONFIGWIDGET_HPP GLBOALCONFIGWIDGET_HPP

#include <QGroupBox>

class QLabel;
class QComboBox;
class QCheckBox;

class GlobalConfigWidget : public QGroupBox
{
Q_OBJECT
   
public:
   GlobalConfigWidget( QWidget *parent = 0 );
   virtual ~GlobalConfigWidget();
   void readSettings();
   void writeSettings();
   static void setClipboard( const QString &text );
   static QString getClipboard();
   void showClipboard( bool allow = true );
   void showAnimate( bool allow = true );
   void showNormalize( bool allow = true );

public slots:

signals:

private:
   QLabel       *mpClipboardLabel;
   QComboBox    *mpClipboardSelection;
   QCheckBox    *mpAnimateViews;
   QCheckBox    *mpNormalizeCase;
   QCheckBox    *mpNormalizeSpaces;
};

#endif
