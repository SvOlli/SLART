/*
 * src/apps/Notorious/TarEntry.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef TARENTRY_HPP
#define TARENTRY_HPP TARENTRY_HPP

/* system headers */

/* Qt headers */
#include <QStringList>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


/*!
  \addtogroup Notorious

  @{
*/

/*!
 \brief \todo complete documentation

*/
class TarEntry
{
public:
   TarEntry();
   virtual ~TarEntry();

   void setData( const char *filename, const char *data );

   QStringList sql();

private:
   Q_DISABLE_COPY( TarEntry )

   bool          mDirty; /*!< \brief \todo TODO */
   const QString mSQLLine; /*!< \brief \todo TODO */
   const QChar   mCR; /*!< \brief \todo TODO */
   const QChar   mLF; /*!< \brief \todo TODO */
   const QChar   mBadData; /*!< \brief \todo TODO */
   const QChar   mQuote; /*!< \brief \todo TODO */
   const QString mDoubleQuote; /*!< \brief \todo TODO */

   QString     mCategory; /*!< \brief \todo TODO */
   QString     mID; /*!< \brief \todo TODO */
   QString     mData; /*!< \brief \todo TODO */
   QStringList mSQL; /*!< \brief \todo TODO */
   QStringList mLines; /*!< \brief \todo TODO */
   int         mTracks; /*!< \brief \todo TODO */

   int     mStartframe[100]; /*!< \brief \todo TODO */
   int     mPlaytime[100]; /*!< \brief \todo TODO */
   QString mTitle[100]; /*!< \brief \todo TODO */
#if INCLUDE_EXT
   QString mExt[100];
#endif
};

/*! @} */

#endif

