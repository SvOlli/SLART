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

class Encoder : public QWidget
{
Q_OBJECT

public:
   Encoder( QWidget *parent, const QString &encoderName );
   virtual ~Encoder();
   virtual void initialize( const QString &fileName) = 0;
   virtual void finalize();
   virtual void setTag( const QString &tag, const QString &value ) = 0;
   virtual void encodeCDAudio( const char* data, int size ) = 0;
   const QString name;

protected:
   virtual void initialize( const QString &fileName, const char *extension );
   int mFD;

private:
   Encoder( const Encoder &other );
   Encoder &operator=( const Encoder &other );
   
   QByteArray mFileName;
};

#endif
