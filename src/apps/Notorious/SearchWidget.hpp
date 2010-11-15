/**
 * src/apps/Notorious/SearchWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef SEARCHWIDGET_HPP
#define SEARCHWIDGET_HPP SEARCHWIDGET_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */
#include <QTime>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QCheckBox;
class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QString;

/* forward declaration of local classes */


class SearchWidget : public QWidget
{
Q_OBJECT
   
public:
   SearchWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   const QString &getSQL() { return mSQL; };

public slots:
   void generateSQL();
   void checkSubmit();
   void query1Running( bool r );
   void query2Running( bool r );

signals:
   void submit( const QString &sql );

private:
   SearchWidget( const SearchWidget &that );
   SearchWidget &operator=( const SearchWidget &that );
   QString time2frames( const QString &time );
   
   const QChar mStar;
   const QChar mPercent;
   const QChar mQuote;
   const QString mDoubleQuote;
   
   QLineEdit   *mpTitle;
   QCheckBox   *mpExact;
   QComboBox   *mpTrack;
   QLineEdit   *mpMinTime;
   QLineEdit   *mpMaxTime;
   QPushButton *mpSearchButton;
//   QPushButton *mpSettingsButton;
   QLabel      *mpSQLClause;
   QLabel      *mpQuery1Info;
   QLabel      *mpQuery2Info;
   QTime       mTime1;
   QTime       mTime2;
   QString     mSQL;
};

#endif
