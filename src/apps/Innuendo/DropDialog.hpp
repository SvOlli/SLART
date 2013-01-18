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
   Q_DISABLE_COPY( DropDialog )

   /*!
    \brief handle the drop

   */
   void handleDropData();
   /*!
    \brief analyse the mime data

   */
   void getMimeData( const QMimeData *mimeData );

   QComboBox       *mpMimeTypes; /*!< \brief \todo TODO */
   QPushButton     *mpClipboard; /*!< \brief \todo TODO */
   QPushButton     *mpColor; /*!< \brief \todo TODO */
   QPushButton     *mpHtml; /*!< \brief \todo TODO */
   QPushButton     *mpText; /*!< \brief \todo TODO */
   QPushButton     *mpUrls; /*!< \brief \todo TODO */
   QTextBrowser    *mpTextBrowser; /*!< \brief \todo TODO */
   QSignalMapper   *mpSignalMapper; /*!< \brief \todo TODO */
   QStringList     mMimeDataCache; /*!< \brief \todo TODO */
};

/*! @} */

#endif
