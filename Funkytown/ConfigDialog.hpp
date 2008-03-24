/**
 * ConfigDialog.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef CONFIGDIALOG_HPP
#define CONFIGDIALOG_HPP CONFIGDIALOG_HPP

#include <QDialog>
#include "ProxyWidget.hpp"

class QGridLayout;
class QPushButton;
class QListWidget;


class ConfigDialog : public QDialog
{
Q_OBJECT
   
public:
   ConfigDialog( QWidget *parent = 0 );
   ProxyWidget *proxyWidget();

public slots:
   void exec();
   void readSettings();
   void writeSettings();
   void logMessage( const QString &message );

signals:
   void closing( bool closing );

private:
   QGridLayout  *mpLayout;
   ProxyWidget  *mpProxyWidget;
   QListWidget  *mpLogList;
};

#endif
