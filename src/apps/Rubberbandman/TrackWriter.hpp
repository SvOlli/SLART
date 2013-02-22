/*
 * src/libs/Common/TrackWriter.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef TRACKWRITER_HPP
#define TRACKWRITER_HPP TRACKWRITER_HPP

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
 \brief \todo complete documentation

*/
class TrackWriter : public QObject
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param target
    \param method
    \param type
   */
   TrackWriter( QObject *target = 0, const char *method = 0,
                enum Qt::ConnectionType type = Qt::AutoConnection );

   /*!
    \brief destructor

   */
   virtual ~TrackWriter();

   /*!
    \brief 

    \param trackInfo
   */
   void write( const TrackInfo &trackInfo );

public slots:

protected:

private:
   Q_DISABLE_COPY( TrackWriter )

   QObject                 *mpTarget;
   const char              *mpMethod;
   enum Qt::ConnectionType mType;
};

/*! @} */

#endif
