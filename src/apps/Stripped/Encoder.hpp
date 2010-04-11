/**
 * src/apps/Stripped/Encoder.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef ENCODER_HPP
#define ENCODER_HPP ENCODER_HPP

/* base class */
#include <QThread>

/* system headers */

/* Qt headers */
#include <QFile>

/* local library headers */
#include <TagList.hpp>

/* local headers */

/* forward declaration of Qt classes */
class ScrollLine;

/* forward declaration of local classes */


class Encoder : public QThread
{
Q_OBJECT
   
public:
   Encoder( QObject *parent, const QString &encoderName );
   virtual ~Encoder();
   
   /*  */
   virtual QWidget *configWidget() = 0;
   /*  */
   void run();
   /* read settings from storage */
   virtual void readSettings() = 0;
   /* write settings to storage */
   virtual void writeSettings() = 0;
   /* name of the encoder */
   const QString mName;
   /* set the tags of the encoded file, always called before(!) initialize */
   void setTags( const TagList &tagList );
   /*  */
   bool useEncoder();
   /*  */
   void setEnqueue( bool activate );
   /*  */
   void setDirectory( ScrollLine *dirOverride );
   /* initialize the encoder */
   virtual bool initialize( const QString &fileName) = 0;
   /* finalize (clean up) the encoder and close the file */
   virtual bool finalize( bool enqueue, bool cancel );

public slots:
   /* encode raw cd audio data */
   virtual void encodeCDAudio( const QByteArray &data ) = 0;
   /*  */
   virtual void setUseEncoder( bool on ) = 0;

signals:
   /*  */
   void encodingFail();
   /*  */
   void useEncoderClicked( bool on );

protected:
   /* initialize the encoder (create the output file) */
   virtual bool initialize( const QString &fileName, const char *extension );
   /*  */
   bool writeChunk( const char* buffer, qint64 size );
   
   /* settings */
   bool     mUseEncoder;
   bool     mEnqueue;
   bool     mDirOverride;
   QString  mDirectory;
   /*  */
   QFile    mFile;
   TagList  mTagList;
   
private:
   Encoder( const Encoder &other );
   Encoder &operator=( const Encoder &other );

   QString  mFileName;
};

#endif
