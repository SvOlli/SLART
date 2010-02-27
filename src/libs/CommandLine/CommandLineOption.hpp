/**
 * src/libs/CommandLine/CommandLineOption.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef COMMANDLINEOPTION_HPP
#define COMMANDLINEOPTION_HPP COMMANDLINEOPTION_HPP

/* base class */

/* system headers */

/* Qt headers */
#include <QString>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


class CommandLineOption
{
public:
   CommandLineOption( const QString &name, const QString &type, const QString &desc );
   virtual bool parse( const QString &name ) = 0;
   virtual bool parse( const QString &name, const QString &option ) = 0;
   bool check();
   QString help();

   const QString mName;
   const QString mType;
   const QString mDesc;

protected:
   bool          mFail;
};

#endif
