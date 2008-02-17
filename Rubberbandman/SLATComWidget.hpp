/**
 * SLATComWidget.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef SLATCOMWIDGET_HPP
#define SLATCOMWIDGET_HPP SLATCOMWIDGET_HPP

#include <QWidget>
#include <QDir>

#include "SLATCom.hpp"

class QString;
class InfoEdit;
class QPushButton;

class SLATComWidget : public QWidget
{
Q_OBJECT

public:
   SLATComWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );

public slots:
   void handleSLAT( const QStringList &message );
   void readConfig();

private:
   SLATComWidget( const SLATComWidget &other );
   SLATComWidget &operator=( const SLATComWidget &other );

   InfoEdit       *mpInfoEdit;
   SLATCom        mSLATCom;
};

#endif
