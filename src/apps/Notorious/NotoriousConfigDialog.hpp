/*
 * src/apps/Notorious/NotoriousConfigDialog.hpp
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
#include <QElapsedTimer>

/* local library headers */
#include <ProxyWidget.hpp>

/* local headers */

/* forward declaration of Qt classes */
class QCheckBox;
class QLabel;
class QListWidget;
class QPushButton;
class QTextBrowser;

/* forward declaration of local classes */
class FreeDBImport;
class PathEnterWidget;


/*!
  \addtogroup Notorious

  @{
*/

/*!
 \brief \todo complete documentation

 \dotfile "graphs/apps/Notorious/NotoriousConfigDialog_connect.dot" "Connect Graph"
*/
class NotoriousConfigDialog : public QDialog
{
   Q_OBJECT

public:
   NotoriousConfigDialog( QWidget *parent = 0 );

public slots:
   /*!
    \brief execute the dialog

   */
   int exec();
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
    \brief run or stop an import job

   */
   void handleImport();
   /*!
    \brief pop up requester for selecting base dir

   */
   void setFileName();
   /*!
    \brief update buttons to show if base dir is valid

   */
   void checkValidFile( const QString &fileName );
   /*!
    \brief handle the progress of an import

   */
   void handleProgress( unsigned count, const char *filename );

private:
   Q_DISABLE_COPY( NotoriousConfigDialog )

   FreeDBImport      *mpFreeDBImport;
   QListWidget       *mpLogList;
   PathEnterWidget   *mpImportFile;
   QLabel            *mpCount;
   QLabel            *mpFileName;
   QLabel            *mpTimeSpent;
   QPushButton       *mpImportButton;
   QElapsedTimer     mTimeSpent;
};

/*! @} */

#endif
