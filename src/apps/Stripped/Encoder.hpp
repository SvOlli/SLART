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
#include <QWidget>

/* system headers */

/* Qt headers */
#include <QFile>

/* local library headers */
#include <TagList.hpp>

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


class Encoder : public QWidget
{
Q_OBJECT
   
public:
   Encoder( QWidget *parent, const QString &encoderName );
   virtual ~Encoder();
   
   /* initialize the encoder */
   virtual bool initialize( const QString &fileName) = 0;
   /* finalize (clean up) the encoder and close the file */
   virtual bool finalize( bool enqueue, bool cancel );
   /* set the tags of the encoded file */
   void setTags( const TagList &tagList );
   /* encode raw cd audio data */
   virtual bool encodeCDAudio( const char* data, int size ) = 0;
   /* read settings from storage */
   virtual void readSettings() = 0;
   /* write settings to storage */
   virtual void writeSettings() = 0;
   /* name of the encoder */
   const QString mName;

protected:
   /* initialize the encoder (create the output file) */
   virtual bool initialize( const QString &fileName, const char *extension );
   /*  */
   bool writeChunk( const char* buffer, qint64 size );
   
   QFile   mFile;
   TagList mTagList;
   
private:
   Encoder( const Encoder &other );
   Encoder &operator=( const Encoder &other );
   
   QString mFileName;
};

#endif
