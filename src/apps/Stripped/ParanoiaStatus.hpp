/**
 * src/apps/Stripped/ParanoiaStatus.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef PARANOIASTATUS_HPP
#define PARANOIASTATUS_HPP PARANOIASTATUS_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */
#include <QList>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QSpinBox;
class QLabel;

/* forward declaration of local classes */
class CDInfo;


class ParanoiaStatus : public QWidget
{
Q_OBJECT

public:
   ParanoiaStatus( QWidget *parent = 0 );
   virtual ~ParanoiaStatus();

   bool hasErrors();

public slots:
   /*  */
   void clear();
   /*  */
   void handleTrackNr( int track );
   /*  */
   void update( int track, unsigned int elements, const unsigned long *counts );

signals:

private:
   ParanoiaStatus( const ParanoiaStatus &other );
   ParanoiaStatus &operator=( const ParanoiaStatus &other );

   int               *mpData;
   QSpinBox          *mpTrackNr;
   QLabel            **mpTrackErrors;
   QLabel            **mpTotalErrors;
};

#endif
