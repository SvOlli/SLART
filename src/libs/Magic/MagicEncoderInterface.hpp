/**
 * src/apps/Magic/MagicEncoder.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef MAGICENCODER_INTERFACE_HPP
#define MAGICENCODER_INTERFACE_HPP MAGICENCODER_INTERFACE_HPP

/* base class */

/* system headers */

/* Qt headers */
#include <QFile>

/* local library headers */
#include <TagList.hpp>

/* local headers */

/* forward declaration of Qt classes */
class QAbstractButton;
class QThread;

/* forward declaration of local classes */
class MagicEncoder;
class MagicEncoderConfig;
class MagicEncoderProxy;
class Satellite;
class ScrollLine;


class MagicEncoderInterface
{
public:
   virtual ~MagicEncoderInterface() {}

   /* constructor replacement */
   virtual void setup( MagicEncoderProxy *parent, const QString &msgHeader ) = 0;
   /* supply the a handle to the configuration widget */
   virtual MagicEncoderConfig *configWidget( QWidget *parent = 0, QAbstractButton *button = 0 ) = 0;
   /* supply a handle to the worker thread for signal/slot communication */
   virtual QThread *workerThread() = 0;
   /* get the filename of the shared object */
   virtual QString pluginFileName() = 0;
   /* name of the encoder */
   virtual QString name() = 0;
   /* set the tags of the encoded file, always called before(!) initialize */
   virtual void setTags( const TagList &tagList ) = 0;
   /* should the encoder be used? */
   virtual bool useEncoder() = 0;
   /* should the track be enqueued after encoding? */
   virtual void setEnqueue( bool activate ) = 0;
   /* set the directory to write the files to */
   virtual void setDirectory( ScrollLine *dirOverride ) = 0;
   /* initialize the encoder */
   virtual bool initialize( const QString &fileName) = 0;
   /* finalize (clean up) the encoder and close the file */
   virtual bool finalize( bool enqueue, bool cancel ) = 0;

public slots:
   /* encode raw cd audio data */
   virtual void encodeCDAudio( const QByteArray &data ) = 0;

signals:
   /* signals that the encoding has failed */
   virtual void encodingFail() = 0;
};

Q_DECLARE_INTERFACE(MagicEncoderInterface,
                    "org.svolli.SLART.MagicEncoderInterface/1.0")

#include "MagicEncoder/MagicEncoder.hpp"

#endif
