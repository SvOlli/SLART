/*
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
class QGridLayout;
class QLabel;
class QSpinBox;
class QToolBar;

/* forward declaration of local classes */
class CDInfo;

/*!
  \addtogroup Stripped
  @{
  */

/*!
 \brief visualize the ripping status

 \dotfile "graphs/apps/Stripped/ParanoiaStatus_connect.dot" "Connect Graph"
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
    \brief set the toolbar this widget is running in for size adjustment

    \param toolBar the "parent" toolbar
   */
   void setToolBar( QToolBar *toolBar );

public slots:
   /*!
    \brief set all entries to 0

   */
   void clear();

   /*!
    \brief change the orientation of the widget

    \param orientation new orientation
   */
   void changeOrientation( Qt::Orientation orientation );

   /*!
    \brief switch to status of the given track

    \param track track number to switch to
   */
   void showTrackStats( int track );

   /*!
    \brief update the status of a track

    \param track track number
    \param elements number of elements
    \param counts pointer to list of counts
   */
   void setTrackData( int track, unsigned int elements, const unsigned long *counts );

signals:

private:
   Q_DISABLE_COPY( ParanoiaStatus )

   int               *mpData; /*!< \brief \todo */
   QGridLayout       *mpLayout; /*!< \brief \todo */
   QLabel            *mpError; /*!< \brief \todo */
   QSpinBox          *mpTrackNr; /*!< \brief \todo */
   QLabel            *mpTotal; /*!< \brief \todo */
   QLabel            **mpHeaders; /*!< \brief \todo */
   QLabel            **mpTrackErrors; /*!< \brief \todo */
   QLabel            **mpTotalErrors; /*!< \brief \todo */
   QToolBar          *mpToolBar;
};

/*! @} */

#endif
