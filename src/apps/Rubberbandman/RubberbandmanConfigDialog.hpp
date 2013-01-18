/*
 * src/apps/Rubberbandman/RubberbandmanConfigDialog.hpp
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

 \dotfile "graphs/apps/Rubberbandman/RubberbandmanConfigDialog_connect.dot" "Connect Graph"
*/
class RubberbandmanConfigDialog : public QDialog
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param parent
    \param flags
   */
   RubberbandmanConfigDialog( QWidget *parent = 0, Qt::WindowFlags flags = 0 );

   /*!
    \brief destructor

   */
   virtual ~RubberbandmanConfigDialog();

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
   Q_DISABLE_COPY( RubberbandmanConfigDialog )

   GlobalConfigWidget   *mpGlobalConfigWidget; /*!< \brief \todo complete documentation */
   QCheckBox            *mpAutoRescan; /*!< \brief \todo complete documentation */
   QLabel               *mpWithTrackNrLabel; /*!< \brief \todo complete documentation */
   QLineEdit            *mpWithTrackNr; /*!< \brief \todo complete documentation */
   QLabel               *mpWithTrackNrExample; /*!< \brief \todo complete documentation */
   QLabel               *mpWithoutTrackNrLabel; /*!< \brief \todo complete documentation */
   QLineEdit            *mpWithoutTrackNr; /*!< \brief \todo complete documentation */
   QLabel               *mpWithoutTrackNrExample; /*!< \brief \todo complete documentation */
   QLabel               *mpPlayingPatternLabel; /*!< \brief \todo complete documentation */
   QLineEdit            *mpPlayingPattern; /*!< \brief \todo complete documentation */
   QLabel               *mpPlayingPatternExample; /*!< \brief \todo complete documentation */
   TagMap               mTagMap; /*!< \brief holds dummy data for output example generation */
};

/*! @} */

#endif
