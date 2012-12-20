/*
 * \file src/tests/TestAppCodeEditor/MainWidget.cpp
 * \author written by Sven Oliver Moll
 *
 * \copyright
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "MainWidget.hpp"

/* system headers */

/* Qt headers */
#include <QCommonStyle>
#include <QGridLayout>
#include <QPushButton>

/* local library headers */

/* local headers */


MainWidget::MainWidget( QWidget *parent , Qt::WindowFlags flags )
: QWidget( parent, flags )
{
   QCommonStyle style;
   QGridLayout *layout = new QGridLayout( this );
   const int size = 5;
#if 0
   QString basePath(":/trolltech/styles/commonstyle/images/");

   QStringList entries( QDir(basePath).entryList(QStringList("*-16.png")) );
   int i = 0;
   foreach( const QString &entry, entries )
   {
      QPushButton *pb = new QPushButton( QIcon(basePath+entry), entry, this );
      layout->addWidget( pb, i / size, i % size );
      i++;
   }
#else
   for( int i = 0; i < 69; i++ )
   {
      QPushButton *pb = new QPushButton( style.standardIcon( static_cast<enum QStyle::StandardPixmap>(i) ),
                                         QString::number(i), this );
      layout->addWidget( pb, i / size, i % size );
   }
#endif
}


MainWidget::~MainWidget()
{
}
