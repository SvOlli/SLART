/**
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
#include <TagList.hpp>

/* local headers */

/* forward declaration of Qt classes */
class QAbstractButton;
class QCheckBox;
class QLineEdit;
class QPushButton;

/* forward declaration of local classes */

/*!
  \addtogroup MagicEncoder
  @{
  */

/*!
 \brief base for magic encoder configuration widget

*/
class MagicEncoderConfig : public QWidget
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param parent parent widget
    \param button TODO
   */
   MagicEncoderConfig( QWidget *parent, QAbstractButton *button );
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

signals:
   /*!
    \brief signals that the use encoder checkbox has been clicked in configuration widget

    \param on
   */
   void useEncoderClicked( bool on );

protected:
   QAbstractButton      *mpExternalUseEncoder; /*!< TODO */
   QCheckBox            *mpUseEncoder; /*!< TODO */
   QCheckBox            *mpDirOverride; /*!< TODO */
   QLineEdit            *mpDirEdit; /*!< TODO */
   QPushButton          *mpDotButton; /*!< TODO */

private:
   Q_DISABLE_COPY( MagicEncoderConfig )
};

#define VALUE_DIRECTORY          value( "Directory", QDir::current().absolutePath() ).toString()
#define VALUE_DIRECTORY_OVERRIDE value( "DirectoryOverride", false ).toBool()
#define VALUE_USE_ENCODER        value( "UseEncoder", false ).toBool()

#endif
