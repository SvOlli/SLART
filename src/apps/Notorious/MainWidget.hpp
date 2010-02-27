/**
 * src/apps/Notorious/MainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP MAINWIDGET_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QLineEdit;
class QPushButton;

/* forward declaration of local classes */
class ConfigDialog;
class FreeDB;
class Foundlist;
class SearchWidget;


class MainWidget : public QWidget
{
Q_OBJECT
   
public:
   MainWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );

protected:
   
public slots:
   void runQuery( const QString &query );
   void handleClicked( const QString &category, const QString &id );
   
signals:
   
private:
   MainWidget( const MainWidget &other );
   MainWidget &operator=( const MainWidget &other );
   
   FreeDB       *mpFreeDB;
   SearchWidget *mpSearch;
   Foundlist    *mpFound;
   Foundlist    *mpCDInfo;
   ConfigDialog *mpConfig;
   QPushButton  *mpSettingsButton;
};

#endif
