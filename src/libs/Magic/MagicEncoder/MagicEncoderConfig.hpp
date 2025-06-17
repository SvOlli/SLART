/*
 * src/apps/Magic/MagicEncoder/MagicEncoderConfig.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef MAGICENCODERCONFIG_HPP
#define MAGICENCODERCONFIG_HPP MAGICENCODERCONFIG_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */
#include <QFile>

/* local library headers */
#include <TagMap.hpp>

/* local headers */

/* forward declaration of Qt classes */
class QAbstractButton;
class QCheckBox;
class QLineEdit;
class QPushButton;

/* forward declaration of local classes */

/*!
  \addtogroup Magic
  @{
  */

/*!
 \brief base for magic encoder configuration widget

 \dotfile "graphs/libs/Magic/MagicEncoder/MagicEncoderConfig_connect.dot" "Connect Graph"
*/
class MagicEncoderConfig : public QWidget
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param parent parent widget
    \param toggleEnableAction action for enable/disable
   */
   MagicEncoderConfig( QWidget *parent, QAction *toggleEnableAction );

   /*!
    \brief descructor

   */
   virtual ~MagicEncoderConfig();

   /*!
    \brief read settings from storage

   */
   virtual void readSettings() = 0;

   /*!
    \brief write settings to storage

   */
   virtual void writeSettings() = 0;

public slots:
   /*!
    \brief handle the dot button by display filesystem browser

   */
   void selectDirectory();
   /*!
    \brief override directory is only configurable when override enabled

   */
   void enableOverride( int state );

signals:
   /*!
    \brief signals that the use encoder checkbox has been clicked in configuration widget

    \param on
   */
   void useEncoderClicked( bool on );

protected:
   QCheckBox            *mpUseEncoder; /*!< \brief internal button for enabling encoder */
   QCheckBox            *mpDirOverride; /*!< \brief override for destination directory */
   QLineEdit            *mpDirEdit; /*!< \brief data for override destination directory */
   QPushButton          *mpDotButton; /*!< \brief button for directory requester */

private:
   Q_DISABLE_COPY( MagicEncoderConfig )
};

/*! @} */

#endif
