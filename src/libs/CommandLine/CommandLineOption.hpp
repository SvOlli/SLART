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


/*!
 \brief helper for class CommandLine, basis for all other CommandLineOption* classes

 \class CommandLineOption CommandLineOption.hpp "libs/CommandLine/CommandLineOption.hpp"
*/
class CommandLineOption
{
public:
   CommandLineOption( const QString &name, const QString &type, const QString &desc );
   virtual ~CommandLineOption();

   /*!
    \brief try to parse option name without argument

    \fn parse
    \param name
   */
   virtual bool parse( const QString &name ) = 0;
   /*!
    \brief try to parse option name with an argument

    \fn parse
    \param name
   */
   virtual bool parse( const QString &name, const QString &option ) = 0;
   /*!
    \brief check if parsing was successful

    \fn check
   */
   virtual bool check();
   /*!
    \brief generate help message for this option

    \fn help
   */
   virtual QString help();

   const QString mName;
   const QString mType;
   const QString mDesc;

protected:
   bool          mFail;

private:
   CommandLineOption( const CommandLineOption &that );
   CommandLineOption &operator=( const CommandLineOption &that );
};

#endif
