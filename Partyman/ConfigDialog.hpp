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


class ConfigDialog : public QDialog
{
Q_OBJECT

public:
   ConfigDialog( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   bool execWithDialog();

public slots:
   void readSettings();
   void writeSettings();
   void setM3uFileName();
   void handleDerMixDrun( bool checked );
   void handleUDPListen( bool checked );
   void handleNormalizeMode( int mode );
   void handleConnected( bool connected );

signals:
   void configChanged();

private:
   ConfigDialog( const ConfigDialog &other );
   ConfigDialog &operator=( const ConfigDialog &other );

   QLabel         *mpDerMixDhostLabel;
   QLineEdit      *mpDerMixDhost;
   QLabel         *mpDerMixDportLabel;
   QSpinBox       *mpDerMixDport;
   QCheckBox      *mpDerMixDlog;
   QCheckBox      *mpDerMixDrun;
   QLabel         *mpDerMixDcmdLabel;
   QLineEdit      *mpDerMixDcmd;
   QLabel         *mpDerMixDparamsLabel;
   QLineEdit      *mpDerMixDparams;
   QCheckBox      *mpAutoConnect;
   QSpinBox       *mpCrossfadeTime;
   QCheckBox      *mpSLARTCommunication;
   QSpinBox       *mpUDPListenerPort;
   QComboBox      *mpNormalizeMode;
   QDoubleSpinBox *mpNormalizeValue;
   MyClipboard    *mpClipboard;
   QLineEdit      *mpLogCmd;
   QPushButton    *mpM3uFileName;
   QCheckBox      *mpAnimateViews;
};

#endif
