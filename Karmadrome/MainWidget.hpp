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

   void updateLists();

public slots:
   void addToList( const QString &msg );
   void handleSLART( const QStringList &message );
   void handleAdd();
   void handleRemove( QAction *action );
   void handleSettings();
   void handleReadButton();
   void handleWriteButton();
   void labelReadWriteButtons();

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
