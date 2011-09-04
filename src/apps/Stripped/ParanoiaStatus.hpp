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

/*!
  \addtogroup Stripped
  @{
  */

/*!
 \brief visualize the ripping status

*/
class ParanoiaStatus : public QWidget
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param parent parent widget
   */
   ParanoiaStatus( QWidget *parent = 0 );

   /*!
    \brief destructor

   */
   virtual ~ParanoiaStatus();

   /*!
    \brief detect if any errors happend during ripping

   */
   bool hasErrors();

public slots:
   /*  */
   /*!
    \brief set all entries to 0

   */
   void clear();
   /*  */
   /*!
    \brief switch to status of the given track

    \param track track number to switch to
   */
   void handleTrackNr( int track );
   /*  */
   /*!
    \brief update the status of a track

    \param track track number
    \param elements number of elements
    \param counts pointer to list of counts
   */
   void update( int track, unsigned int elements, const unsigned long *counts );

signals:

private:
   Q_DISABLE_COPY( ParanoiaStatus )

   int               *mpData; /*!< TODO */
   QSpinBox          *mpTrackNr; /*!< TODO */
   QLabel            **mpTrackErrors; /*!< TODO */
   QLabel            **mpTotalErrors; /*!< TODO */
};

/*! @} */

#endif
