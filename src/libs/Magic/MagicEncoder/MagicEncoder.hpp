/*
 * src/apps/Magic/MagicEncoder/MagicEncoder.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef MAGICENCODER_HPP
#define MAGICENCODER_HPP MAGICENCODER_HPP

/* base class */
#include "../MagicEncoderInterface.hpp"
#include <QThread>

/* system headers */

/* Qt headers */
#include <QFile>

/* local library headers */
#include <TagMap.hpp>

/* local headers */

/* forward declaration of Qt classes */
class MagicEncoderConfig;
class MagicEncoderProxy;
class ScrollLine;

/* forward declaration of local classes */

/*!
  \addtogroup Magic
  @{
  */

/*!
 \brief

*/
class MagicEncoder : public QThread, public MagicEncoderInterface
{
   Q_OBJECT
   Q_INTERFACES(MagicEncoderInterface)

public:
   /*!
    \brief constructor

    \param encoderName
   */
   MagicEncoder( const QString &encoderName );

   /*!
    \brief destructor

   */
   virtual ~MagicEncoder();


   /*!
    \brief constructor replacement

    \param parent
    \param msgHeader
   */
   virtual void setup( MagicEncoderProxy *parent, const QString &msgHeader );

   /*!
    \brief supply the a handle to the configuration widget

    \param parent
    \param button
   */
   virtual MagicEncoderConfig *configWidget( QWidget *parent ) = 0;

   /*!
    \brief supply an action to enable/disable encoder

   */
   virtual QAction *toggleEnableAction();

   /*!
    \brief supply a handle to the worker thread for signal/slot communication

   */
   virtual QThread *workerThread();

   /*!
    \brief get the filename of the shared object

   */
   virtual QString pluginFileName();

   /*!
    \brief implementation of function to run as thread

   */
   void run();

   /*!
    \brief name of the encoder

   */
   QString name();

   /*!
    \brief should the encoder be used?

   */
   bool useEncoder();

   /*!
    \brief should the track be enqueued after encoding?

    \param activate
   */
   void setEnqueue( bool activate );

   /*!
    \brief set the directory to write the files to

    \param dirOverride
   */
   void setDirectory( ScrollLine *dirOverride );

   /*!
    \brief initialize the encoder

    \param fileName
   */
   virtual bool initialize( const QString &fileName) = 0;

   /*!
    \brief finalize (clean up) the encoder and close the file

    \param enqueue
    \param cancel
   */
   virtual bool finalize( bool enqueue, bool cancel ) = 0;

public slots:
   /*!
    \brief set the tags of the encoded file, always called before(!) initialize

    \param tagList
   */
   void setTags( const TagMap &tagList );

   /*!
    \brief encode raw cd audio data

    \param data
   */
   virtual void encodeCDAudio( const QByteArray &data ) = 0;

signals:

   /*!
    \brief signals that the encoding has failed

   */
   void encodingFail();

   /*!
    \brief signals that the use encoder checkbox has been clicked in configuration widget

    \param on
   */
   void useEncoderClicked( bool on );

protected:

   /*!
    \brief initialize the encoder (create the output file)

    \param fileName
    \param extension
   */
   virtual bool initialize( const QString &fileName, const char *extension );

   /*!
    \brief write a chunk of encoded audio data

    \param buffer
    \param size
   */
   bool writeChunk( const char* buffer, qint64 size );

   /* settings */
   MagicEncoderProxy *mpProxy;/*!< \brief \todo complete documentation */
   QAction           *mpToggleEnableAction;/*!< \brief \todo complete documentation */
   bool              mEnqueue;/*!< \brief \todo complete documentation */
   bool              mDirOverride;/*!< \brief \todo complete documentation */
   const QString     mName;/*!< \brief \todo complete documentation */
   QString           mDirectory;/*!< \brief \todo complete documentation */
   QString           mPluginFileName;/*!< \brief \todo complete documentation */
   QString           mMsgHeader;/*!< \brief \todo complete documentation */
   QFile             mFile;/*!< \brief \todo complete documentation */
   TagMap            mTagMap; /*!< \brief tags to insert in encoded file */

private:
   Q_DISABLE_COPY( MagicEncoder )

   QString           mFileName;/*!< \brief \todo complete documentation */
};

/*! @} */

#endif
