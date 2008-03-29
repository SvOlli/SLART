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


class ConfigDialog : public QDialog
{
Q_OBJECT

public:
   ConfigDialog( QWidget *parent = 0, Qt::WindowFlags flags = 0 );

public slots:
   void exec();
   void updateWithTrackNr( const QString &text );
   void updateWithoutTrackNr( const QString &text );
   void updatePlayingPattern( const QString &text );
   void readSettings();
   void writeSettings();

signals:
   void configChanged();

private:
   ConfigDialog( const ConfigDialog &other );
   ConfigDialog &operator=( const ConfigDialog &other );

   GlobalConfigWidget   *mpGlobalConfigWidget;
   QLabel               *mpWithTrackNrLabel;
   QLineEdit            *mpWithTrackNr;
   QLabel               *mpWithTrackNrExample;
   QLabel               *mpWithoutTrackNrLabel;
   QLineEdit            *mpWithoutTrackNr;
   QLabel               *mpWithoutTrackNrExample;
   QLabel               *mpPlayingPatternLabel;
   QLineEdit            *mpPlayingPattern;
   QLabel               *mpPlayingPatternExample;
   TagList              mTagList;
};

#endif
