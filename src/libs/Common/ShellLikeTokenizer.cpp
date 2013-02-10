/*
 * src/libs/Common/ShellLikeTokenizer.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

/* class declaration */
#include "ShellLikeTokenizer.hpp"

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* class variables */


ShellLikeTokenizer::ShellLikeTokenizer()
: mIsDirty( false )
, mIsValid( true )
, mInput()
, mOutput()
{
}


ShellLikeTokenizer::ShellLikeTokenizer( const QString &input )
: mIsDirty( true )
, mIsValid( false )
, mInput( input )
, mOutput()
{
}


ShellLikeTokenizer::ShellLikeTokenizer( const ShellLikeTokenizer &that )
: mIsDirty( that.mIsDirty )
, mIsValid( that.mIsValid )
, mInput( that.mInput )
, mOutput( that.mOutput )
{
}


ShellLikeTokenizer &ShellLikeTokenizer::operator=( const ShellLikeTokenizer &that )
{
   mIsDirty = that.mIsDirty;
   mIsValid = that.mIsValid;
   mInput   = that.mInput;
   mOutput  = that.mOutput;

   return *this;
}


ShellLikeTokenizer::~ShellLikeTokenizer()
{
}


void ShellLikeTokenizer::setInput( const QString &input )
{
   mInput = input;
   mIsDirty = true;
   mIsValid = false;
}


QString ShellLikeTokenizer::input()
{
   return mInput;
}


bool ShellLikeTokenizer::isValid()
{
   if( mIsDirty )
   {
      split();
   }
   return mIsValid;
}


QStringList ShellLikeTokenizer::tokenized()
{
   if( mIsDirty )
   {
      split();
   }
   return mOutput;
}


void ShellLikeTokenizer::split()
{
   mOutput.clear();
   QString token;
   enum { stnq, stsq, stdq, stbs, stqb } state = stnq;

   foreach( const QChar &c, mInput )
   {
      switch( state )
      {
      case stnq:
         switch( c.toLatin1() )
         {
         case ' ':
            if( !token.isEmpty() )
            {
               mOutput.append( token );
            }
            token.clear();
            break;
         case '\'':
            state = stsq;
            break;
         case '"':
            state = stdq;
            break;
         case '\\':
            state = stbs;
            break;
         default:
            token.append( c );
         }
         break;
      case stsq:
         switch( c.toLatin1() )
         {
         case '\'':
            state = stnq;
            break;
         default:
            token.append( c );
         }
         break;
      case stdq:
         switch( c.toLatin1() )
         {
         case '"':
            state = stnq;
            break;
         case '\\':
            state = stqb;
            break;
         default:
            token.append( c );
         }
         break;
      case stbs:
      case stqb:
         token.append( c );
         state = (state == stqb) ? stdq : stnq;
         break;
      }
   }

   mIsDirty = false;
   mIsValid = (state == stnq);
   if( !mIsValid )
   {
      mOutput.clear();
   }
   else
   {
      if( !token.isEmpty() )
      {
         mOutput.append( token );
      }
   }
}
