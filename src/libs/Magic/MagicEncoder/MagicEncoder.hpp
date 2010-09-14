/**
 * src/apps/Magic/MagicEncoder/MagicEncoder.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
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
#include <TagList.hpp>

/* local headers */

/* forward declaration of Qt classes */
class ScrollLine;

/* forward declaration of local classes */


class MagicEncoder : public QThread, public MagicEncoderInterface
{
Q_OBJECT
Q_INTERFACES(MagicEncoderInterface)
   
public:
   MagicEncoder( const QString &encoderName );
   virtual ~MagicEncoder();
   
   /* constructor replacement */
   virtual void setup( Satellite *satellite, const QString &msgHeader,
                       const QString &fileName );
   /* supply the a handle to the configuration widget */
   virtual QWidget *configWidget() = 0;
   /* supply a handle to the worker thread for signal/slot communication */
   virtual QThread *workerThread();
   /* get the filename of the shared object */
   virtual QString pluginFileName();
   /* implementation of function to run as thread */
   void run();
   /* read settings from storage */
   virtual void readSettings() = 0;
   /* write settings to storage */
   virtual void writeSettings() = 0;
   /* name of the encoder */
   QString name();
   /* set the tags of the encoded file, always called before(!) initialize */
   void setTags( const TagList &tagList );
   /* should the encoder be used? */
   bool useEncoder();
   /* should the track be enqueued after encoding? */
   void setEnqueue( bool activate );
   /* set the directory to write the files to */
   void setDirectory( ScrollLine *dirOverride );
   /* initialize the encoder */
   virtual bool initialize( const QString &fileName) = 0;
   /* finalize (clean up) the encoder and close the file */
   virtual bool finalize( bool enqueue, bool cancel ) = 0;


public slots:
   /* encode raw cd audio data */
   virtual void encodeCDAudio( const QByteArray &data ) = 0;
   /* set if the encoder should be used */
   virtual void setUseEncoder( bool on ) = 0;

signals:
   /* signals that the encoding has failed */
   void encodingFail();
   /* signals that the use encoder checkbox has been clicked in configuration widget */
   void useEncoderClicked( bool on );

protected:
   /* initialize the encoder (create the output file) */
   virtual bool initialize( const QString &fileName, const char *extension );
   /* write a chunk of encoded audio data */
   bool writeChunk( const char* buffer, qint64 size );

   /* settings */
   Satellite      *mpSatellite;
   bool           mUseEncoder;
   bool           mEnqueue;
   bool           mDirOverride;
   QString        mDirectory;
   QString        mPluginFileName;
   QString        mMsgHeader;
   const QString  mName;
   QFile          mFile;
   TagList        mTagList;

private:
   MagicEncoder( const MagicEncoder &other );
   MagicEncoder &operator=( const MagicEncoder &other );

   QString  mFileName;
};

#define VALUE_DIRECTORY          value( "Directory", QDir::current().absolutePath() ).toString()
#define VALUE_DIRECTORY_OVERRIDE value( "DirectoryOverride", false ).toBool()
#define VALUE_USE_ENCODER        value( "UseEncoder", false ).toBool()

#endif
