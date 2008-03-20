/**
 * CDReader.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef CDREADER_HPP
#define CDREADER_HPP CDREADER_HPP

#include <QWidget>
#include <QString>
extern "C" {
#include <cdio/cdio.h>
#include <cdio/paranoia.h>
};

class QProgressBar;
   
class CDToc;
class CDEdit;
class Encoder;
class QLabel;
class QComboBox;
class QCheckBox;
   
class CDReader : public QWidget
{
Q_OBJECT

public:
   CDReader( CDToc *toc, CDEdit *edit, QWidget *parent = 0, Qt::WindowFlags flags = 0 );
   void callback( long inpos, ::paranoia_cb_mode_t function );
   void updateCDDisplay();
   void setEncoder( Encoder *encoder );
   void getDevices( QComboBox *comboBox );

public slots:
   void readToc();
   void readCDText();
   void readTracks();
   void setDevice( const QString &device );
   void eject();
   void cancel();

signals:
   void starting();
   void stopping();

private:
   CDReader( const CDReader &other );
   CDReader &operator=( const CDReader &other );
   
   ::CdIo_t             *mpCdIo;
   ::cdrom_drive_t      *mpDrive;
   ::cdrom_paranoia_t   *mpParanoia;
   CDToc                *mpToc;
   CDEdit               *mpCDEdit;
   Encoder              *mpEncoder;
   QLabel               *mpMessage;
   QProgressBar         *mpProgress;
   unsigned int         mCallbackFunction[13];
   QString              mDevice;
   bool                 mCancel;
};

#endif
