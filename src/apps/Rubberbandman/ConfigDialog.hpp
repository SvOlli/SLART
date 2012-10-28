/*
 * src/apps/Rubberbandman/ConfigDialog.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef CONFIGDIALOG_HPP
#define CONFIGDIALOG_HPP CONFIGDIALOG_HPP

/* base class */
#include <QDialog>

/* system headers */

/* Qt headers */

/* local library headers */
#include <TagMap.hpp>

/* local headers */

/* forward declaration of Qt classes */
class QCheckBox;
class QComboBox;
class QDoubleSpinBox;
class QLabel;
class QLineEdit;
class QSpinBox;

/* forward declaration of local classes */
class GlobalConfigWidget;
class ProxyWidget;


/*!
  \addtogroup Rubberbandman

  @{
*/

/*!
 \brief \todo complete documentation

 \dotfile "graphs/apps/Rubberbandman/ConfigDialog_connect.dot" "Connect Graph"
*/
class ConfigDialog : public QDialog
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param parent
    \param flags
   */
   ConfigDialog( QWidget *parent = 0, Qt::WindowFlags flags = 0 );

   /*!
    \brief destructor

   */
   virtual ~ConfigDialog();

public slots:
   /*!
    \brief show dialog

   */
   void exec();
   /*!
    \brief read settings from storage

   */
   void readSettings();
   /*!
    \brief write settings to storage

   */
   void writeSettings();
   /*!
    \brief update example

   */
   void updateWithTrackNr( const QString &text );
   /*!
    \brief update example

   */
   void updateWithoutTrackNr( const QString &text );
   /*!
    \brief update example

   */
   void updatePlayingPattern( const QString &text );

signals:
   /*!
    \brief configuration has changed

   */
   void configChanged();

private:
   Q_DISABLE_COPY( ConfigDialog )

   GlobalConfigWidget   *mpGlobalConfigWidget; /*!< \brief \todo TODO */
   QCheckBox            *mpAutoRescan; /*!< \brief \todo TODO */
   QLabel               *mpWithTrackNrLabel; /*!< \brief \todo TODO */
   QLineEdit            *mpWithTrackNr; /*!< \brief \todo TODO */
   QLabel               *mpWithTrackNrExample; /*!< \brief \todo TODO */
   QLabel               *mpWithoutTrackNrLabel; /*!< \brief \todo TODO */
   QLineEdit            *mpWithoutTrackNr; /*!< \brief \todo TODO */
   QLabel               *mpWithoutTrackNrExample; /*!< \brief \todo TODO */
   QLabel               *mpPlayingPatternLabel; /*!< \brief \todo TODO */
   QLineEdit            *mpPlayingPattern; /*!< \brief \todo TODO */
   QLabel               *mpPlayingPatternExample; /*!< \brief \todo TODO */
   TagMap               mTagMap; /*!< \brief holds dummy data for output example generation */
};

/*! @} */

/* defaults */
#define VALUE_AUTORESCAN      value( "AutoRescan", true ).toBool()
#define VALUE_CURRENTTAB      value( "CurrentTab", 0 ).toInt()
#define VALUE_FILEEXTENSIONS  value( "FileExtensions", QStringList() << "*.mp3" << "*.ogg" << "*.oga" << "*.flac" ).toStringList()
#define VALUE_MUSICBASE       value( "MusicBase", QString("/") ).toString()
#define VALUE_PLAYINGPATTERN  value( "PlayingPattern", "NP: |$ARTIST| - |$TITLE|" ).toString()
#define VALUE_ROOTDIRECTORY   value( "RootDirectory", QString("/") ).toString()
#define VALUE_STYLESHEET      value( "StyleSheet", QString() ).toString()
#define VALUE_WITHOUTTRACKNR  value( "WithoutTrackNr", "|$ARTIST| - |$TITLE|" ).toString()
#define VALUE_WITHTRACKNR     value( "WithTrackNr", "(|#2TRACKNUMBER|)|$ARTIST| - |$TITLE|" ).toString()

#endif
