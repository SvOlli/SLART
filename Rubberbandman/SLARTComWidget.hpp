/**
 * SLARTComWidget.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef SLARTCOMWIDGET_HPP
#define SLARTCOMWIDGET_HPP SLARTCOMWIDGET_HPP

#include <QWidget>
#include <QDir>

#include "SLARTCom.hpp"

class QString;
class InfoEdit;
class QPushButton;

class SLARTComWidget : public QWidget
{
Q_OBJECT

public:
   SLARTComWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );

public slots:
   void handleSLART( const QStringList &message );
   void handleNowPlaying();
   void handleShowInFilesystem();
   void readConfig();

signals:
   void showInFilesystem( const QString &path );

private:
   SLARTComWidget( const SLARTComWidget &other );
   SLARTComWidget &operator=( const SLARTComWidget &other );

   InfoEdit       *mpInfoEdit;
   QPushButton    *mpNowPlaying;
   QPushButton    *mpShowInFilesystem;
   SLARTCom       mSLARTCom;
};

#endif
