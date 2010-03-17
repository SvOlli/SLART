/**
 * src/apps/Stripped/CDReader.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef CDREADER_HPP
#define CDREADER_HPP CDREADER_HPP

/* base class */
#include <QWidget>

/* system headers */
extern "C" {
#include <cdio/cdio.h>
#include <cdio/paranoia.h>
}

/* Qt headers */
#include <QStringList>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QCheckBox;
class QComboBox;
class QLabel;
class QProgressBar;

/* forward declaration of local classes */
class CDEdit;
class CDInfo;
class CDReaderThread;
class Encoder;
   

class CDReader : public QWidget
{
Q_OBJECT
   
public:
   CDReader( CDInfo *info, CDEdit *edit, QWidget *parent = 0 );
   virtual ~CDReader();
   
   /* set the encoder to use */
   void setEncoder( Encoder *encoder );
   /* insert available devices into given combobox */
   void getDevices();
   
public slots:
   /* read the toc */
   void readToc();
   /*  */
   void readCDText();
   /* rip tracks */
   void readTracks();
   /* set the device to use */
   void setDevice( const QString &device );
   /* eject disc */
   void eject();
   /* handle cancel button */
   void cancel();
   
signals:
   /*  */
   void stateNoDisc();
   /*  */
   void stateDisc();
   /*  */
   void stateScan();
   /*  */
   void stateRip();
   /*  */
   void gotToc();
   /*  */
   void foundDevices( const QStringList &devices );
   /*  */
   void message( const QString &message = QString() );
   /*  */
   void progress( int percent );
   /*  */
   void setTrackDisabled( int track, bool disabled );
   /*  */
   void ensureVisible( int track );
   
private:
   CDReader( const CDReader &other );
   CDReader &operator=( const CDReader &other );

   CDReaderThread       *mpCDReaderThread;
   CDInfo               *mpCDInfo;
   CDEdit               *mpCDEdit;
   Encoder              *mpEncoder;
   QProgressBar         *mpProgressBar;
   QString              mDevice;
};

#endif
