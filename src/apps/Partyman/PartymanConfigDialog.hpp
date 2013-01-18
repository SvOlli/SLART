/*
 * src/apps/Partyman/PartymanConfigDialog.hpp
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

 \dotfile "graphs/apps/Partyman/PartymanConfigDialog_connect.dot" "Connect Graph"
*/
class PartymanConfigDialog : public QDialog
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param database database to use
    \param parent parent widget
    \param flags window flags
   */
   PartymanConfigDialog( Database *database, QWidget *parent = 0, Qt::WindowFlags flags = 0 );

   /*!
    \brief destructor

   */
   virtual ~PartymanConfigDialog();

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
   Q_DISABLE_COPY( PartymanConfigDialog )

   Database             *mpDatabase; /*!< \brief \todo TODO */
   QLabel               *mpDerMixDhostLabel; /*!< \brief \todo TODO */
   QLineEdit            *mpDerMixDhost; /*!< \brief \todo TODO */
   QLabel               *mpDerMixDportLabel; /*!< \brief \todo TODO */
   QSpinBox             *mpDerMixDport; /*!< \brief \todo TODO */
   QCheckBox            *mpDerMixDlog; /*!< \brief \todo TODO */
   QCheckBox            *mpDerMixDrun; /*!< \brief \todo TODO */
   QLabel               *mpDerMixDcmdLabel; /*!< \brief \todo TODO */
   QLineEdit            *mpDerMixDcmd; /*!< \brief \todo TODO */
   QLabel               *mpDerMixDparamsLabel; /*!< \brief \todo TODO */
   QLineEdit            *mpDerMixDparams; /*!< \brief \todo TODO */
   QCheckBox            *mpAutoConnect; /*!< \brief \todo TODO */
   QSpinBox             *mpCrossfadeTime; /*!< \brief \todo TODO */
   QComboBox            *mpNormalizeMode; /*!< \brief \todo TODO */
   QDoubleSpinBox       *mpNormalizeValue; /*!< \brief \todo TODO */
   MyClipboard          *mpClipboard; /*!< \brief \todo TODO */
   QLineEdit            *mpLogCmd; /*!< \brief \todo TODO */
   QCheckBox            *mpCountSkip; /*!< \brief \todo TODO */
   QCheckBox            *mpTrayIcon; /*!< \brief \todo TODO */
   QCheckBox            *mpTrayIconBubble; /*!< \brief \todo TODO */
   QDoubleSpinBox       *mpTrayIconBubbleTime; /*!< \brief \todo TODO */
   QCheckBox            *mpPlayOnlyFavorite; /*!< \brief \todo TODO */
   QCheckBox            *mpPlayOnlyLeastPlayed; /*!< \brief \todo TODO */
   QComboBox            *mpPlayFolder; /*!< \brief \todo TODO */
   QSpinBox             *mpPlayNotAgainCount; /*!< \brief \todo TODO */
   QLineEdit            *mpNamePattern; /*!< \brief \todo TODO */
   QLineEdit            *mpPlayerPattern; /*!< \brief \todo TODO */
   QLineEdit            *mpListPattern; /*!< \brief \todo TODO */
   QLineEdit            *mpTrayIconPattern; /*!< \brief \todo TODO */
   QPushButton          *mpUpdateBrowserButton; /*!< \brief \todo TODO */
   QPushButton          *mpStartKioskButton; /*!< \brief \todo TODO */
   GlobalConfigWidget   *mpGlobalSettings; /*!< \brief \todo TODO */
};

/*! @} */

#endif
