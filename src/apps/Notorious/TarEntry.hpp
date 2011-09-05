/**
 * src/apps/Notorious/TarEntry.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
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
 \brief TODO

*/
class TarEntry
{
public:
   TarEntry();
   virtual ~TarEntry();

   void setData( const char *filename, const char *data );

   QStringList sql();

private:
   TarEntry( const TarEntry &that );
   TarEntry &operator=( const TarEntry &that );

   bool          mDirty;
   const QString mSQLLine;
   const QChar   mCR;
   const QChar   mLF;
   const QChar   mBadData;
   const QChar   mQuote;
   const QString mDoubleQuote;

   QString     mCategory;
   QString     mID;
   QString     mData;
   QStringList mSQL;
   QStringList mLines;
   int         mTracks;

   int     mStartframe[100];
   int     mPlaytime[100];
   QString mTitle[100];
#if INCLUDE_EXT
   QString mExt[100];
#endif
};

/*! @} */

#endif

