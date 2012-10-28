/*
 * src/apps/Magic/MagicEncoder.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef MAGICENCODER_INTERFACE_HPP
#define MAGICENCODER_INTERFACE_HPP MAGICENCODER_INTERFACE_HPP

/* base class */

/* system headers */

/* Qt headers */
#include <QFile>

/* local library headers */
#include <TagMap.hpp>

/* local headers */

/* forward declaration of Qt classes */
class QAction;
class QThread;

/* forward declaration of local classes */
class MagicEncoder;
class MagicEncoderConfig;
class MagicEncoderProxy;
class Satellite;
class ScrollLine;


/*!
  \addtogroup MagicEncoder

  @{
*/

/*!
 \brief \todo complete documentation

*/
class MagicEncoderInterface
{
public:
   /*!
    \brief destructor

   */
   virtual ~MagicEncoderInterface() {}

   /*!
    \brief constructor replacement

    \param parent the proxy that initializes the interface
    \param msgHeader Satellite message header to use when sending message upon
    completion
   */
   virtual void setup( MagicEncoderProxy *parent, const QString &msgHeader ) = 0;
   /*!
    \brief supply the pointer to the configuration widget

    Supply the pointer to the configuration widget. If the widget does no exist,
    it will be created.

    \param parent parent to use if creation is necessary
    \return MagicEncoderConfig pointer to the configuration widget
   */
   virtual MagicEncoderConfig *configWidget( QWidget *parent = 0 ) = 0;
   /*!
    \brief supply the action to enable/disable encoder

    \return QAction pointer to action
   */
   virtual QAction *toggleEnableAction() = 0;
   /*!
    \brief supply the handle to the worker thread for signal/slot communication

    \return QThread pointer to thread class
   */
   virtual QThread *workerThread() = 0;
   /*!
    \brief get the filename of the shared object

    \return QString filename
   */
   virtual QString pluginFileName() = 0;
   /*!
    \brief name of the encoder

    \return QString encoder name
   */
   virtual QString name() = 0;
   /*!
    \brief should the encoder be used?

    \return bool is enabled
   */
   virtual bool useEncoder() = 0;
   /*!
    \brief should the track be enqueued after encoding?

    \param activate enable encoder
   */
   virtual void setEnqueue( bool activate ) = 0;
   /*!
    \brief set the directory to write the files to

    \param dirOverride \todo
   */
   virtual void setDirectory( ScrollLine *dirOverride ) = 0;
   /*!
    \brief initialize the encoder

    \param fileName filename to use for writing
    \return bool success
   */
   virtual bool initialize( const QString &fileName ) = 0;
   /*!
    \brief finalize (clean up) the encoder and close the file

    \param enqueue actually enqueue
    \param cancel encoded process was cancelled
    \return bool success
   */
   virtual bool finalize( bool enqueue, bool cancel ) = 0;

public slots:
   /*!
    \brief set the tags of the encoded file, always called before(!) initialize

    \param tagMap list of tags to use on file creation
   */
   virtual void setTags( const TagMap &tagMap ) = 0;
   /*!
    \brief encode raw cd audio data

    \param data data to encode
   */
   virtual void encodeCDAudio( const QByteArray &data ) = 0;

signals:
   /*!
    \brief signals that the encoding has failed

   */
   virtual void encodingFail() = 0;
};

/*! @} */

Q_DECLARE_INTERFACE(MagicEncoderInterface,
                    "org.svolli.SLART.MagicEncoderInterface/1.0")

#include "MagicEncoder/MagicEncoder.hpp"

#endif
