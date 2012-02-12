/*
 * src/apps/Innuendo/DropDialog.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef DROPDIALOG_HPP
#define DROPDIALOG_HPP DROPDIALOG_HPP

/* base class */
#include <QDialog>

/* system headers */

/* Qt headers */
#include <QStringList>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QCheckBox;
class QComboBox;
class QLabel;
class QLineEdit;
class QMimeData;
class QSignalMapper;
class QSpinBox;
class QTextBrowser;

/* forward declaration of local classes */
class ConfigNotifyWidget;
class GlobalConfigWidget;
class ProxyWidget;


/*!
  \addtogroup Innuendo

  @{
*/

/*!
 \brief \todo complete documentation

 \dotfile "graphs/apps/Innuendo/DropDialog_connect.dot" "Connect Graph"
*/
class DropDialog : public QDialog
{
   Q_OBJECT

public:
   DropDialog( QWidget *parent = 0, Qt::WindowFlags flags = 0 );

   /*!
    \brief for implementing dropping

   */
   void dropEvent( QDropEvent *event );

protected:
   /*!
    \brief for implementing dropping

   */
   void dragEnterEvent( QDragEnterEvent *event );

public slots:
   /*!
    \brief handle the dropdown list containing mime types

   */
   void mimeDataByIndex( int index );
   /*!
    \brief target for clipboard button

   */
   void handleClipboard();

private:
   DropDialog( const DropDialog &that );
   DropDialog &operator=( const DropDialog &that );
   /*!
    \brief handle the drop

   */
   void handleDropData();
   /*!
    \brief analyse the mime data

   */
   void getMimeData( const QMimeData *mimeData );

   QComboBox       *mpMimeTypes;
   QPushButton     *mpClipboard;
   QPushButton     *mpColor;
   QPushButton     *mpHtml;
   QPushButton     *mpText;
   QPushButton     *mpUrls;
   QTextBrowser    *mpTextBrowser;
   QSignalMapper   *mpSignalMapper;
   QStringList     mMimeDataCache;
};

/*! @} */

#endif
