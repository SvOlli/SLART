/*
 * src/apps/Funkytown/FunkytownConfigDialog.hpp
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
class QLabel;
class QListWidget;
class QPushButton;
class QTextBrowser;

/* forward declaration of local classes */
class GlobalConfigWidget;
class ProxyWidget;


/*!
  \addtogroup Funkytown

  @{
*/

/*!
 \brief \todo complete documentation

 \dotfile "graphs/apps/Funkytown/FunkytownConfigDialog_connect.dot" "Connect Graph"
*/
class FunkytownConfigDialog : public QDialog
{
   Q_OBJECT

public:
   FunkytownConfigDialog( QWidget *parent = 0 );

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
    \brief send a message to log widget

   */
   void logMessage( const QString &message );
   /*!
    \brief handle the clear button

   */
   void handleClear();

private:
   FunkytownConfigDialog( const FunkytownConfigDialog &that );
   FunkytownConfigDialog &operator=( const FunkytownConfigDialog &that );

   ProxyWidget        *mpProxyWidget;
   QListWidget        *mpLogList;
   QTextBrowser       *mpHelpText;
   GlobalConfigWidget *mpGlobalConfigWidget;
   QCheckBox          *mpOverwrite;
   QCheckBox          *mpCoverArt;
   QCheckBox          *mpTollKeep;
   QLabel             *mpDownloadedFiles;
   QLabel             *mpDownloadedBytes;
   QPushButton        *mpClearButton;
};

/*! @} */

#endif
