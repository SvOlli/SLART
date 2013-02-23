/*
 * src/tests/TestAppWebServer/MainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP MAINWIDGET_HPP

/* base class */
#include <QSplitter>

/* system headers */

/* Qt headers */
#include <QByteArray>
#include <QList>
#include <QMap>
#include <QUrl>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QFileSystemModel;
class QGridLayout;
class QLabel;
class QLineEdit;
class QListWidget;
class QModelIndex;
class QSignalMapper;
class QTreeView;

/* forward declaration of local classes */
class Downloader;
class DownloadJobCoverAmazonDE;
class ImageWidget;

class MainWidget : public QSplitter
{
   Q_OBJECT

public:
   MainWidget( QWidget *parent = 0 );
   virtual ~MainWidget();

public slots:
   void requestFromLine();

   void showCovers( const QByteArray &data, const QVariant &payload );

   void entryClicked( const QModelIndex &index );

   void saveImage( QWidget *widget );

signals:

private:
   Q_DISABLE_COPY( MainWidget )

   Downloader                       *mpDownloader;
   DownloadJobCoverAmazonDE         *mpAmazon;
   QGridLayout                      *mpLayout;
   QTreeView                        *mpFileSysTree;
   QFileSystemModel                 *mpFileSysModel;
   QLineEdit                        *mpLineEdit;
   ImageWidget                      *mpImage;
   QLineEdit                        *mpMessage;
   QSignalMapper                    *mpSignalMapper;
   QMap<QWidget*,QUrl>              mDataMap;
   int                              mNumColumns;
};

#endif