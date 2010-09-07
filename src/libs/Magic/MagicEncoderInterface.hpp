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
class QThread;

/* forward declaration of local classes */
class ScrollLine;


class MagicEncoderInterface
{
public:
   virtual ~MagicEncoderInterface() {}

   /*  */
   virtual QWidget *configWidget() = 0;
   /* read settings from storage */
   virtual void readSettings() = 0;
   /* write settings to storage */
   virtual void writeSettings() = 0;
   /* name of the encoder */
   virtual QString name() = 0;
   /* set the tags of the encoded file, always called before(!) initialize */
   virtual void setTags( const TagList &tagList ) = 0;
   /*  */
   virtual bool useEncoder() = 0;
   /*  */
   virtual void setEnqueue( bool activate ) = 0;
   /*  */
   virtual void setDirectory( ScrollLine *dirOverride ) = 0;
   /* initialize the encoder */
   virtual bool initialize( const QString &fileName) = 0;
   /* finalize (clean up) the encoder and close the file */
   virtual bool finalize( bool enqueue, bool cancel ) = 0;
   /*  */
   virtual QThread *workerThread() = 0;
   /*  */
   virtual void setPluginFileName( const QString &fileName ) = 0;
   /*  */
   virtual QString pluginFileName() = 0;

public slots:
   /*  */
   virtual void start() = 0;
   /* encode raw cd audio data */
   virtual void encodeCDAudio( const QByteArray &data ) = 0;
   /*  */
   virtual void setUseEncoder( bool on ) = 0;

signals:
   /*  */
   virtual void encodingFail() = 0;
   /*  */
   virtual void useEncoderClicked( bool on ) = 0;
};

Q_DECLARE_INTERFACE(MagicEncoderInterface,
                    "org.svolli.SLART.MagicEncoderInterface/1.0")

#include "MagicEncoder/MagicEncoder.hpp"

#endif
