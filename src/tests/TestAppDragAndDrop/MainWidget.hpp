/*
 * \file src/tests/TestAppDragAndDrop/MainWidget.hpp
 * \author written by Sven Oliver Moll
 *
 * \copyright
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP MAINWIDGET_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */
#include <QByteArray>
#include <QList>

/* local library headers */
#include <TrackInfo.hpp>

/* local headers */

/* forward declaration of Qt classes */
class QComboBox;
class QMimeData;
class QPushButton;
class QSignalMapper;
class QTextBrowser;

/* forward declaration of local classes */


class MainWidget : public QWidget
{
   Q_OBJECT

public:
   MainWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   virtual ~MainWidget();

protected:
   /*!
    \brief for implementing dropping

   */
   void dragEnterEvent( QDragEnterEvent *event );

   /*!
    \brief for implementing dropping

   */
   void dropEvent( QDropEvent *event );

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
   Q_DISABLE_COPY( MainWidget )

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

#endif
