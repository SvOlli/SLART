/**
 * ConfigDialog.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef CONFIGDIALOG_HPP
#define CONFIGDIALOG_HPP CONFIGDIALOG_HPP

#include <QDialog>
class QSpinBox;
class QLabel;
class QLineEdit;
class QCheckBox;
class QComboBox;
class QDoubleSpinBox;
class MyClipboard;
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
   /* handle "start DerMixD" checkbox */
   void handleDerMixDrun( bool checked );
   /* handle "SLARTCom" checkbox */
   void handleUDPListen( bool checked );
   /* handle "Nomalize" combobox */
   void handleNormalizeMode( int mode );

signals:
   void configChanged();

private:
   ConfigDialog( const ConfigDialog &other );
   ConfigDialog &operator=( const ConfigDialog &other );

   QLabel               *mpDerMixDhostLabel;
   QLineEdit            *mpDerMixDhost;
   QLabel               *mpDerMixDportLabel;
   QSpinBox             *mpDerMixDport;
   QCheckBox            *mpDerMixDlog;
   QCheckBox            *mpDerMixDrun;
   QLabel               *mpDerMixDcmdLabel;
   QLineEdit            *mpDerMixDcmd;
   QLabel               *mpDerMixDparamsLabel;
   QLineEdit            *mpDerMixDparams;
   QCheckBox            *mpAutoConnect;
   QSpinBox             *mpCrossfadeTime;
   QCheckBox            *mpSLARTCommunication;
   QSpinBox             *mpUDPListenerPort;
   QComboBox            *mpNormalizeMode;
   QDoubleSpinBox       *mpNormalizeValue;
   MyClipboard          *mpClipboard;
   QLineEdit            *mpLogCmd;
   QCheckBox            *mpCountSkip;
   QLineEdit            *mpNamePattern;
   QLineEdit            *mpPlayerPattern;
   QLineEdit            *mpListPattern;
   GlobalConfigWidget   *mpGlobalSettings;
};

#endif
