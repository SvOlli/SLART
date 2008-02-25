/**
 * ButtonsWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef BUTTONSWIDGET_HPP
#define BUTTONSWIDGET_HPP BUTTONSWIDGET_HPP

#include <QWidget>
#include <QList>

class QGridLayout;
class QPushButton;
class QSignalMapper;

class ButtonsWidget : public QWidget
{
Q_OBJECT

public:
   ButtonsWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );

   void updateButtons( const QStringList &list );

public slots:

signals:
   void clicked( const QString &name );

private:
   ButtonsWidget( const ButtonsWidget &other );
   ButtonsWidget &operator=( const ButtonsWidget &other );

   QGridLayout          *mpMainLayout;
   QSignalMapper        *mpSignalMapper;
   QList<QPushButton*>  mButtonList;
};

#endif
