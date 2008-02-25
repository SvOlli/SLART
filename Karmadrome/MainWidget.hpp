/**
 * MainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP MAINWIDGET_HPP

#include <QWidget>

#include "SLATCom.hpp"

class QIcon;
class QString;
class ButtonsWidget;
class QLineEdit;


class MainWidget : public QWidget
{
Q_OBJECT

public:
   MainWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );

public slots:
   void addToList( const QString &msg );
   void handleSLAT( const QStringList &message );
   void handleSettings();

signals:
   void requestChangeTitle( const QIcon &icon, const QString &title );

private:
   MainWidget( const MainWidget &other );
   MainWidget &operator=( const MainWidget &other );

   QLineEdit      *mpFileName;
   ButtonsWidget  *mpListButtons;

   SLATCom      mSLATCom;
};

#endif
