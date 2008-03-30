/**
 * ConfigDialog.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef CONFIGDIALOG_HPP
#define CONFIGDIALOG_HPP CONFIGDIALOG_HPP

#include <QDialog>
#include "TagList.hpp"
class QSpinBox;
class QLabel;
class QLineEdit;
class QCheckBox;
class QComboBox;
class QDoubleSpinBox;
class ProxyWidget;
class ConfigNotifyWidget;
class GlobalConfigWidget;
class Encoder;
class CDReader;


class ConfigDialog : public QDialog
{
Q_OBJECT

public:
   ConfigDialog( CDReader *cdreader, QWidget *parent = 0, Qt::WindowFlags flags = 0 );

public slots:
   /* execute the dialog */
   void exec();
   /* read settings from storage */
   void readSettings();
   /* write settings to storage */
   void writeSettings();
   /* dis-/enable settings according to selected encoder */
   void changeEncoder( int id );
   /* update example */
   void updatePattern( const QString &text );

signals:
   void configChanged();

private:
   ConfigDialog( const ConfigDialog &other );
   ConfigDialog &operator=( const ConfigDialog &other );

   CDReader             *mpCDReader;
   GlobalConfigWidget   *mpGlobalConfigWidget;
   QLabel               *mpDevicesLabel;
   QComboBox            *mpDevicesBox;
   QLabel               *mpEncodersLabel;
   QComboBox            *mpEncodersBox;
   QLabel               *mpPatternLabel;
   QLineEdit            *mpPattern;
   QLabel               *mpPatternExample;
   TagList              mTagList;
   QList<Encoder*>      mEncoders;
};

#endif
