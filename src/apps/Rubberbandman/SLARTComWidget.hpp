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
#include "TrackInfo.hpp"

class QCheckBox;
class QPushButton;
class QString;

class Database;
class InfoEdit;


class SLARTComWidget : public QWidget
{
Q_OBJECT
   
public:
   SLARTComWidget( Database *database, QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   
public slots:
   /* handle SLART message to read tags of current track */
   void handleSLART( const QStringList &message );
   /* handle the now playing button */
   void handleNowPlaying();
   /* handle the show in filesystem button */
   void handleShowInFilesystem();
   /* handle the "Get Random Track" button */
   void handleGetRandom();
   /* restart SLART receiver */
   void readConfig();
   
signals:
   /* emit track path from show in filesystem button */
   void showInFilesystem( const QString &path );
   /* emit that partyman did an update on the config */
   void partymanConfigUpdate();
   
private:
   SLARTComWidget( const SLARTComWidget &other );
   SLARTComWidget &operator=( const SLARTComWidget &other );
   
   Database       *mpDatabase;
   InfoEdit       *mpInfoEdit;
   QPushButton    *mpNowPlaying;
   QPushButton    *mpShowInFilesystem;
   QPushButton    *mpGetRandom;
   SLARTCom       mSLARTCom;
   TrackInfo      mTrackInfo;
};

#endif
