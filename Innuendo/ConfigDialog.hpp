/**
 * ConfigDialog.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef CONFIGDIALOG_HPP
#define CONFIGDIALOG_HPP CONFIGDIALOG_HPP

#include <QDialog>

class QCheckBox;
class QComboBox;
class QDoubleSpinBox;
class QLabel;
class QLineEdit;
class QSpinBox;

class ProxyWidget;
class ConfigNotifyWidget;
class GlobalConfigWidget;


class ConfigDialog : public QDialog
{
Q_OBJECT
   
public:
   ConfigDialog( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   
public slots:
   /* execute the dialog */
   void exec();
   /* read settings from storage */
   void readSettings();
   /* write settings to storage */
   void writeSettings();
   
signals:
   void configChanged();
   
private:
   ConfigDialog( const ConfigDialog &other );
   ConfigDialog &operator=( const ConfigDialog &other );
   
   ConfigNotifyWidget   *mpNotifyWidget;
   GlobalConfigWidget   *mpGlobalConfigWidget;
   ProxyWidget          *mpProxyWidget;
   QSpinBox             *mpBufferSize;
   QPushButton          *mpAutostartPartyman;
   QPushButton          *mpAutostartKarmadrome;
   QPushButton          *mpAutostartRubberbandman;
   QPushButton          *mpAutostartStripped;
   QPushButton          *mpAutostartFunkytown;
   QPushButton          *mpAutostartCreep;
};

#endif
