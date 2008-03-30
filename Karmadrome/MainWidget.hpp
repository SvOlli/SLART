/**
 * MainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP MAINWIDGET_HPP

#include <QWidget>

#include "SLARTCom.hpp"

class QIcon;
class QString;
class QPushButton;
class QMenu;
class QLineEdit;
class QAction;
class ButtonsWidget;
class ConfigDialog;


class MainWidget : public QWidget
{
Q_OBJECT

public:
   MainWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );

public slots:
   /* add currently played track to the list */
   void addToList( const QString &msg );
   /* handle SLART message */
   void handleSLART( const QStringList &message );
   /* handle adding of new playlist */
   void handleAdd();
   /* handle removal of playlist */
   void handleRemove( QAction *action );
   /* handle "Read Clipboard" button */
   void handleReadButton();
   /* handle "Write Clipboard" button */
   void handleWriteButton();
   /* label "Read/Write Clipboard" buttons according to settings */
   void labelReadWriteButtons();
   /* update the lists containing the playlists */
   void updateLists();

signals:
   void requestChangeTitle( const QIcon &icon, const QString &title );

private:
   MainWidget( const MainWidget &other );
   MainWidget &operator=( const MainWidget &other );

   QLineEdit      *mpFileName;
   QPushButton    *mpReadButton;
   QPushButton    *mpWriteButton;
   ButtonsWidget  *mpListButtons;

   QPushButton    *mpSettingsButton;
   QPushButton    *mpAddButton;
   QPushButton    *mpRemoveButton;
   QMenu          *mpRemoveMenu;
   ConfigDialog   *mpConfigDialog;

   SLARTCom       mSLARTCom;
   QStringList    mPlaylists;
};

#endif
