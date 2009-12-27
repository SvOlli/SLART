
#ifndef TARENTRY_HPP
#define TARENTRY_HPP TARENTRY_HPP

#include <QStringList>

class TarEntry
{
public:
   TarEntry();
   virtual ~TarEntry();

   void setData( const char *filename, const char *data );

   QStringList sql();

private:
   TarEntry( const TarEntry &other );
   TarEntry &operator=( const TarEntry &other );

   bool          mDirty;
   const QChar   mCR;
   const QChar   mLF;
   const QChar   mQuote;
   const QString mDoubleQuote;
   
   QString     mCategory;
   QString     mID;
   QString     mData;
   QString     mSQLLine;
   QStringList mSQL;
   QStringList mLines;
   int         mTracks;

   int     mStartframe[100];
   int     mPlaytime[100];
   QString mTitle[100];
   QString mExt[100];
};

#endif

