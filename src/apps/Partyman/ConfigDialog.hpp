/*
 * src/apps/Partyman/ConfigDialog.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef CONFIGDIALOG_HPP
#define CONFIGDIALOG_HPP CONFIGDIALOG_HPP

/* base class */
#include <QDialog>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QCheckBox;
class QComboBox;
class QDoubleSpinBox;
class QLabel;
class QLineEdit;
class QSpinBox;

/* forward declaration of local classes */
class Database;
class GlobalConfigWidget;
class MyClipboard;


/*!
  \addtogroup Partyman

  @{
*/

/*!
 \brief \todo complete documentation

 \dotfile "graphs/apps/Rubberbandman/ConfigDialog_connect.dot" "Connect Graph"
*/
class ConfigDialog : public QDialog
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param database database to use
    \param parent parent widget
    \param flags window flags
   */
   ConfigDialog( Database *database, QWidget *parent = 0, Qt::WindowFlags flags = 0 );

   /*!
    \brief destructor

   */
   virtual ~ConfigDialog();

public slots:
   /*!
    \brief execute the dialog

   */
   void exec();
   /*!
    \brief read settings from storage

   */
   void readSettings();
   /*!
    \brief write settings to storage

   */
   void writeSettings();
   /*!
    \brief handle "start DerMixD" checkbox

   */
   void handleDerMixDrun( bool checked );
   /*!
    \brief handle "Nomalize" combobox

   */
   void handleNormalizeMode( int mode );
   /*!
    \brief handle "Show Tray Icon" checkbox

   */
   void handleShowTrayIcon( bool checked );
   /*!
    \brief activate kiosk mode

   */
   void handleStartKiosk();

signals:
   /*!
    \brief configuration has changed

   */
   void configChanged();
   /*!
    \brief update of browser tree requested

   */
   void updateBrowser();

private:
   ConfigDialog( const ConfigDialog &that );
   ConfigDialog &operator=( const ConfigDialog &that );


   Database             *mpDatabase;
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
   QComboBox            *mpNormalizeMode;
   QDoubleSpinBox       *mpNormalizeValue;
   MyClipboard          *mpClipboard;
   QLineEdit            *mpLogCmd;
   QCheckBox            *mpCountSkip;
   QCheckBox            *mpTrayIcon;
   QCheckBox            *mpTrayIconBubble;
   QDoubleSpinBox       *mpTrayIconBubbleTime;
   QCheckBox            *mpPlayOnlyFavorite;
   QCheckBox            *mpPlayOnlyLeastPlayed;
   QComboBox            *mpPlayFolder;
   QSpinBox             *mpPlayNotAgainCount;
   QLineEdit            *mpNamePattern;
   QLineEdit            *mpPlayerPattern;
   QLineEdit            *mpListPattern;
   QLineEdit            *mpTrayIconPattern;
   QPushButton          *mpUpdateBrowserButton;
   QPushButton          *mpStartKioskButton;
   GlobalConfigWidget   *mpGlobalSettings;
};

/*! @} */

#endif
