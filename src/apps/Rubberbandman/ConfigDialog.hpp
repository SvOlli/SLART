/**
 * src/apps/Rubberbandman/ConfigDialog.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef CONFIGDIALOG_HPP
#define CONFIGDIALOG_HPP CONFIGDIALOG_HPP

/* base class */
#include <QDialog>

/* system headers */

/* Qt headers */

/* local library headers */
#include <TagList.hpp>

/* local headers */

/* forward declaration of Qt classes */
class QCheckBox;
class QComboBox;
class QDoubleSpinBox;
class QLabel;
class QLineEdit;
class QSpinBox;

/* forward declaration of local classes */
class ConfigNotifyWidget;
class GlobalConfigWidget;
class ProxyWidget;


/*!
  \addtogroup Rubberbandman

  @{
*/

/*!
 \brief TODO

*/
class ConfigDialog : public QDialog
{
   Q_OBJECT

public:
   ConfigDialog( QWidget *parent = 0, Qt::WindowFlags flags = 0 );

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
   ConfigDialog( const ConfigDialog &that );
   ConfigDialog &operator=( const ConfigDialog &that );

   GlobalConfigWidget   *mpGlobalConfigWidget;
   QCheckBox            *mpAutoRescan;
   QLabel               *mpWithTrackNrLabel;
   QLineEdit            *mpWithTrackNr;
   QLabel               *mpWithTrackNrExample;
   QLabel               *mpWithoutTrackNrLabel;
   QLineEdit            *mpWithoutTrackNr;
   QLabel               *mpWithoutTrackNrExample;
   QLabel               *mpPlayingPatternLabel;
   QLineEdit            *mpPlayingPattern;
   QLabel               *mpPlayingPatternExample;
   TagList              mTagList;
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
