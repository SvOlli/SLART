/*
 * src/libs/Common/FileIconProvider.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef FILEICONPROVIDER_HPP
#define FILEICONPROVIDER_HPP FILEICONPROVIDER_HPP

/* base class */
#include <QFileIconProvider>

/* system headers */

/* Qt headers */
#include <QIcon>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */


/*!
  \addtogroup Common

  @{
*/

/*!
 \brief \todo complete documentation

*/
class FileIconProvider : public QFileIconProvider
{
public:

   /*!
    \brief constructor

   */
   FileIconProvider();

   /*!
    \brief destructor

   */
   virtual ~FileIconProvider();

#if 0
   /*!
    \brief returns an icon set for the given type

    \param type
    \return QIcon
   */
   virtual QIcon icon( IconType type ) const;
#endif
   /*!
    \brief returns an icon for the file described by info

    \param info
    \return QIcon
   */
   virtual QIcon icon( const QFileInfo &info ) const;
#if 0
   /*!
    returns the type of the file described by info

    \param info
    \return QString
   */
   virtual QString type( const QFileInfo &info ) const;
#endif

private:
   Q_DISABLE_COPY( FileIconProvider )

   const QIcon mAudioIcon;
};

/*! @} */

#endif
