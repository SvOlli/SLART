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
 \brief TODO

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
   TrackWriter( QObject *target = 0, const QString &method = QString(),
                enum Qt::ConnectionType type = Qt::AutoConnection );

   /*!
    \brief destructor

   */
   virtual ~TrackWriter();

   /*!
    \brief 

   */
   void write( const TrackInfo &trackInfo );

public slots:

protected:

private:
   Q_DISABLE_COPY( TrackWriter )

   QObject                 *mpTarget;
   QString                 mMethod;
   enum Qt::ConnectionType mType;
};

/*! @} */

#endif
