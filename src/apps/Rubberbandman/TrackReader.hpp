/*
 * src/libs/Common/TrackReader.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef TRACKREADER_HPP
#define TRACKREADER_HPP TRACKREADER_HPP

/* base class */
#include <QObject>

/* system headers */

/* Qt headers */
#include <QString>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */
class TrackInfo;


/*!
  \addtogroup Rubberbandman

  @{
*/

/*!
 \brief TODO

*/
class TrackReader : public QObject
{
   Q_OBJECT

public:

   /*!
    \brief constructor

   */
   TrackReader( QObject *target, const QString &method,
                enum Qt::ConnectionType type = Qt::AutoConnection );
   /*!
    \brief destructor

   */
   virtual ~TrackReader();

   /*!
    \brief read a track

    \param fileName
   */
   void read( const QString &fileName );

public slots:
   /*!
    \brief 

   */
   void handleTrackInfo( const TrackInfo &trackInfo );

protected:

private:
   Q_DISABLE_COPY( TrackReader )

   QObject                 *mpTarget;
   QString                 mMethod;
   enum Qt::ConnectionType mType;
};

/*! @} */

#endif
