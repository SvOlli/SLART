/**
 * src/apps/Innuendo/DropDialog.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef DROPDIALOG_HPP
#define DROPDIALOG_HPP DROPDIALOG_HPP

#include <QDialog>

#include <QStringList>

class QCheckBox;
class QComboBox;
class QLabel;
class QLineEdit;
class QMimeData;
class QSignalMapper;
class QSpinBox;
class QTextBrowser;

class ConfigNotifyWidget;
class GlobalConfigWidget;
class ProxyWidget;


class DropDialog : public QDialog
{
Q_OBJECT

public:
   DropDialog( QWidget *parent = 0, Qt::WindowFlags flags = 0 );

   /* for implementing dropping */
   void dropEvent( QDropEvent *event );

protected:
   /* for implementing dropping */
   void dragEnterEvent( QDragEnterEvent *event );

public slots:
   /* handle the dropdown list containing mime types */
   void mimeDataByIndex( int index );

private:
   DropDialog( const DropDialog &other );
   DropDialog &operator=( const DropDialog &other );
   /* handle the drop */
   void handleDropData();

   QComboBox       *mpMimeTypes;
   QPushButton     *mpColor;
   QPushButton     *mpHtml;
   QPushButton     *mpImage;
   QPushButton     *mpText;
   QPushButton     *mpUrls;
   QTextBrowser    *mpTextBrowser;
   QSignalMapper   *mpSignalMapper;
   QStringList     mMimeDataCache;
};

#endif
