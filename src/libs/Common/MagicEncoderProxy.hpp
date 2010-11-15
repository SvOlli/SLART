/**
 * src/libs/Common/MagicEncoderProxy.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef MAGICENCODERPROXY_HPP
#define MAGICENCODERPROXY_HPP MAGICENCODERPROXY_HPP

/* base class */
#include <QObject>

/* system headers */

/* Qt headers */

/* local library headers */
#include "../../libs/Magic/MagicEncoder/MagicEncoderConfig.hpp"

/* local headers */

/* forward declaration of Qt classes */
class QAbstractButton;
class QPluginLoader;

/* forward declaration of local classes */
class MagicEncoderInterface;
class Satellite;


class MagicEncoderProxy : public QObject
{
Q_OBJECT

public:
   MagicEncoderProxy( Satellite *satellite = 0, QObject *parent = 0 );
   virtual ~MagicEncoderProxy();

   /* try to load a plugin */
   bool pluginLoad( const QString &fileName, const QString &msgHeader );
   /* kind of error when loading plugin for log */
   QString pluginLoadErrorString();
   /* the file name of the plugin */
   QString pluginFileName();
   /* supply the a handle to the configuration widget */
   MagicEncoderConfig *configWidget( QWidget *parent = 0, QAbstractButton *button = 0 );
   /* supply a handle to the worker thread for signal/slot communication */
   QThread *workerThread();
   /* name of the encoder */
   QString name();
   /* set the tags of the encoded file, always called before(!) initialize */
   void setTags( const TagList &tagList );
   /* should the encoder be used? */
   bool useEncoder();
   /* should the track be enqueued after encoding? */
   void setEnqueue( bool activate );
   /* initialize the encoder */
   bool initialize( const QString &fileName);
   /* finalize (clean up) the encoder and close the file */
   bool finalize( bool enqueue, bool cancel );

   /* callback for sending signal encodingFail */
   void emitEncodingFail();
   /* callback for sending Satellite message */
   void satelliteSend( const QByteArray &data );

signals:
   /* signals that the encoding has failed */
   void encodingFail();

public slots:
   /* encode raw cd audio data */
   void encodeCDAudio( const QByteArray &data );

private:
   MagicEncoderProxy( const MagicEncoderProxy &that );
   MagicEncoderProxy &operator=( const MagicEncoderProxy &that );

   QPluginLoader           *mpPluginLoader;
   MagicEncoderInterface   *mpPlugin;
   Satellite               *mpSatellite;
};

#endif /* MAGICENCODERPROXY_HPP */
