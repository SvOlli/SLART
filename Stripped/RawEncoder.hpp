/**
 * RawEncoder.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef RAWRawEncoder_HPP
#define RAWRawEncoder_HPP RAWRawEncoder_HPP

#include "Encoder.hpp"
#include <QWidget>

class RawEncoder : public Encoder
{
Q_OBJECT
   
public:
   RawEncoder( QWidget *parent = 0 );
   virtual ~RawEncoder();
   void initialize( const QString &fileName );
   void finalize();
   void setTag( const QString &tag, const QString &value );
   void encodeCDAudio( const char* data, int size );

private:
   RawEncoder( const RawEncoder &other );
   RawEncoder &operator=( const RawEncoder &other );
};

#endif
