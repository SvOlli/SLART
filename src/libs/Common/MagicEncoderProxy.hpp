/**
 * src/libs/Common/MagicEncoderProxy.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef MAGICENCODERPROXY_HPP
#define MAGICENCODERPROXY_HPP MAGICENCODERPROXY_HPP

/* base class */
#include <QObject>

/* system headers */

/* Qt headers */

/* local library headers */
#include "../../libs/Magic/MagicEncoder/MagicEncoderConfig.hpp"

/* local headers */

/* forward declaration of Qt classes */
class QAbstractButton;
class QPluginLoader;

/* forward declaration of local classes */
class MagicEncoderInterface;
class Satellite;


/*!
  \addtogroup Common

  @{
*/

/*!
 \brief TODO

*/
class MagicEncoderProxy : public QObject
{
   Q_OBJECT

public:
   MagicEncoderProxy( Satellite *satellite = 0, QObject *parent = 0 );
   virtual ~MagicEncoderProxy();

   /*!
    \brief try to load a plugin

   */
   bool pluginLoad( const QString &fileName, const QString &msgHeader );
   /*!
    \brief kind of error when loading plugin for log

   */
   QString pluginLoadErrorString();
   /*!
    \brief the file name of the plugin

   */
   QString pluginFileName();
   /*!
    \brief supply the a handle to the configuration widget

   */
   MagicEncoderConfig *configWidget( QWidget *parent = 0, QAbstractButton *button = 0 );
   /*!
    \brief supply a handle to the worker thread for signal/slot communication

   */
   QThread *workerThread();
   /*!
    \brief name of the encoder

   */
   QString name();
   /*!
    \brief set the tags of the encoded file, always called before(!) initialize

   */
   void setTags( const TagList &tagList );
   /*!
    \brief should the encoder be used?

   */
   bool useEncoder();
   /*!
    \brief should the track be enqueued after encoding?

   */
   void setEnqueue( bool activate );
   /*!
    \brief initialize the encoder

   */
   bool initialize( const QString &fileName);
   /*!
    \brief finalize (clean up) the encoder and close the file

   */
   bool finalize( bool enqueue, bool cancel );

   /*!
    \brief callback for sending signal encodingFail

   */
   void emitEncodingFail();
   /*!
    \brief callback for sending Satellite message

   */
   void satelliteSend( const QByteArray &data );

signals:
   /*!
    \brief signals that the encoding has failed

   */
   void encodingFail();

public slots:
   /*!
    \brief encode raw cd audio data

   */
   void encodeCDAudio( const QByteArray &data );

private:
   Q_DISABLE_COPY( MagicEncoderProxy )

   QPluginLoader           *mpPluginLoader;
   MagicEncoderInterface   *mpPlugin;
   Satellite               *mpSatellite;
};

/*! @} */

#endif
