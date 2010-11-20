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
class QPushButton;

/* forward declaration of local classes */
class ScrollLine;


class MagicEncoderConfig : public QWidget
{
Q_OBJECT

public:
   MagicEncoderConfig( QWidget *parent, QAbstractButton *button );
   virtual ~MagicEncoderConfig();

   /* read settings from storage */
   virtual void readSettings() = 0;
   /* write settings to storage */
   virtual void writeSettings() = 0;

public slots:
   /* handle the dot button by display filesystem browser */
   void selectDirectory();

signals:
   /* signals that the use encoder checkbox has been clicked in configuration widget */
   void useEncoderClicked( bool on );

protected:
   QAbstractButton      *mpExternalUseEncoder;
   QCheckBox            *mpUseEncoder;
   QCheckBox            *mpDirOverride;
   ScrollLine           *mpDirectory;
   QPushButton          *mpDotButton;

private:
   MagicEncoderConfig( const MagicEncoderConfig &that );
   MagicEncoderConfig &operator=( const MagicEncoderConfig &that );

};

#define VALUE_DIRECTORY          value( "Directory", QDir::current().absolutePath() ).toString()
#define VALUE_DIRECTORY_OVERRIDE value( "DirectoryOverride", false ).toBool()
#define VALUE_USE_ENCODER        value( "UseEncoder", false ).toBool()

#endif
