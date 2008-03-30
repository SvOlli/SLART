/**
 * MainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP MAINWIDGET_HPP

#include <QWidget>
#include <QList>

class CDReader;
class CDToc;
class CDDB;
class CDEdit;
class QComboBox;
class QPushButton;
class QHBoxLayout;
class ConfigDialog;


class MainWidget : public QWidget
{
Q_OBJECT

public:
   MainWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );

public slots:
   void setDownloadDir();
   void eject();
   void working( bool allowCancel = true );
   void finished();

signals:
   /* request a new icon and title */
   void requestChangeTitle( const QIcon &icon, const QString &title );

private:
   MainWidget( const MainWidget &other );
   MainWidget &operator=( const MainWidget &other );
   
   QPushButton     *mpDirButton;
   CDToc           *mpToc;
   CDDB            *mpCDDB;
   CDEdit          *mpCDEdit;
   CDReader        *mpCDReader;
   ConfigDialog    *mpConfigDialog;
   QPushButton     *mpSettingsButton;
   QPushButton     *mpCancelButton;
   QHBoxLayout     *mpButtonLayout;
   QPushButton     *mpTocButton;
   QPushButton     *mpCDTextButton;
   QPushButton     *mpRipButton;
   QPushButton     *mpEjectButton;
};

#endif
