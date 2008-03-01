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
class Encoder;
class QComboBox;
class QPushButton;
class QHBoxLayout;


class MainWidget : public QWidget
{
Q_OBJECT

public:
   MainWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );

public slots:
   void changeEncoder( int id );
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
   QPushButton     *mpCancelButton;
   QHBoxLayout     *mpButtonLayout;
   QComboBox       *mpDevicesBox;
   QPushButton     *mpTocButton;
   QPushButton     *mpRipButton;
   QPushButton     *mpEjectButton;
   QComboBox       *mpEncodersBox;
   QList<Encoder*> mEncoders;
};

#endif
