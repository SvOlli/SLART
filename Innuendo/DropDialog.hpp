/**
 * DropDialog.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef DROPDIALOG_HPP
#define DROPDIALOG_HPP DROPDIALOG_HPP

#include <QDialog>
#include <QStringList>
class QSpinBox;
class QLabel;
class QLineEdit;
class QCheckBox;
class QComboBox;
class QTextBrowser;
class QSignalMapper;
class QMimeData;
class ProxyWidget;
class ConfigNotifyWidget;
class GlobalConfigWidget;


class DropDialog : public QDialog
{
Q_OBJECT

public:
   DropDialog( QWidget *parent = 0, Qt::WindowFlags flags = 0 );

protected:
   /* for implementing dropping */
   void dragEnterEvent( QDragEnterEvent *event );
public:
   /* for implementing dropping */
   void dropEvent( QDropEvent *event );

public slots:
   void mimeDataByIndex( int index );

private:
   DropDialog( const DropDialog &other );
   DropDialog &operator=( const DropDialog &other );
   /*  */
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
