/**
 * Encoder.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef ENCODER_HPP
#define ENCODER_HPP ENCODER_HPP

#include <QFile>
#include <QWidget>
#include <QByteArray>

class TagList;

class Encoder : public QWidget
{
Q_OBJECT

public:
   Encoder( QWidget *parent, const QString &encoderName );
   virtual ~Encoder();

   /* initialize the encoder */
   virtual void initialize( const QString &fileName) = 0;
   /* finalize (clean up) the encoder and close the file */
   virtual void finalize( bool enqueue, bool cancel );
   /* set the tags of the encoded file */
   virtual void setTags( const TagList &tagList ) = 0;
   /* encode raw cd audio data */
   virtual bool encodeCDAudio( const char* data, int size ) = 0;
   /* name of the encoder */
   const QString name;

protected:
   /* initialize the encoder (create the output file) */
   virtual void initialize( const QString &fileName, const char *extension );

   int mFD;

private:
   Encoder( const Encoder &other );
   Encoder &operator=( const Encoder &other );
   
   QString mFileName;
};

#endif
