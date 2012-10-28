/*
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
 \brief \todo complete documentation

*/
class MagicEncoderProxy : public QObject
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param satellite interface to announce events
    \param parent parent object
   */
   MagicEncoderProxy( Satellite *satellite = 0, QObject *parent = 0 );
   /*!
    \brief destructor

   */
   virtual ~MagicEncoderProxy();

   /*!
    \brief try to load a plugin

    \param fileName name of plugin to load
    \param msgHeader message header for Satellite messages
    \return bool success
   */
   bool pluginLoad( const QString &fileName, const QString &msgHeader );
   /*!
    \brief kind of error when loading plugin for log

    \return QString error description as provided by QPluginLoader::errorString()
   */
   QString pluginLoadErrorString();
   /*!
    \brief the file name of the plugin

    \return QString name of the plugin as provided by pluginLoad()
   */
   QString pluginFileName();
   /*!
    \brief supply the a handle to the configuration widget

    \param parent
    \return MagicEncoderConfig
   */
   MagicEncoderConfig *configWidget( QWidget *parent = 0 );
   /*!
    \brief supply an action to enable/disable encoder

    \return QAction action
   */
   QAction *toggleEnableAction();
   /*!
    \brief supply a handle to the worker thread for signal/slot communication

    \return QThread thread
   */
   QThread *workerThread();
   /*!
    \brief name of the encoder

    \return QString name
   */
   QString name();
   /*!
    \brief set the tags of the encoded file, always called before(!) initialize

    \param tagMap tags
   */
   void setTags( const TagMap &tagMap );
   /*!
    \brief should the encoder be used?

    \return bool encoder is enabled in general
   */
   bool useEncoder();
   /*!
    \brief should the track be enqueued after encoding?

    \param activate activate enqueueing
   */
   void setEnqueue( bool activate );
   /*!
    \brief initialize the encoder

    \param fileName name of encoded file
    \return bool success
   */
   bool initialize( const QString &fileName );
   /*!
    \brief finalize (clean up) the encoder and close the file

    \param enqueue enqueue track
    \param cancel encoding was canceled
    \return bool success
   */
   bool finalize( bool enqueue, bool cancel );

   /*!
    \brief callback for sending signal encodingFail

   */
   void emitEncodingFail();
   /*!
    \brief callback for sending Satellite message for loaded plugin

    \param data data to send
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

    \param data data to be encoded
   */
   void encodeCDAudio( const QByteArray &data );

private:
   Q_DISABLE_COPY( MagicEncoderProxy )

   QPluginLoader           *mpPluginLoader; /*!< \brief plugin loader */
   MagicEncoderInterface   *mpPlugin;       /*!< \brief loaded plugin */
   Satellite               *mpSatellite;    /*!< \brief satellite communication instance */
};

/*! @} */

#endif
