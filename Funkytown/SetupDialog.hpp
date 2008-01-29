/**
 * SetupDialog.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#include <QDialog>
#include "ProxyWidget.hpp"

class QGridLayout;
class QPushButton;
class QListWidget;


class SetupDialog : public QDialog
{
Q_OBJECT
   
public:
   SetupDialog( QWidget *parent = 0 );
   virtual ~SetupDialog();
   ProxyWidget *proxyWidget();

public slots:
   void logMessage( const QString &message );

signals:
   void closing( bool closing );

private:
   QGridLayout  *mpLayout;
   ProxyWidget  *mpProxyWidget;
   QListWidget  *mpLogList;
   QPushButton  *mpCloseButton;
   QPushButton  *mpAboutQtButton;
};

