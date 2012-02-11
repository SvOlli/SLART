/*
 * src/libs/CommandLine/CommandLineOptionString.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 */

#ifndef COMMANDLINEOPTIONSTRING_HPP
#define COMMANDLINEOPTIONSTRING_HPP COMMANDLINEOPTIONSTRING_HPP

/* base class */
#include "CommandLineOption.hpp"

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


/*!
  \addtogroup CommandLine CommandLine: handling command line parameters

 @{
 */

/*!
 \brief helper for class CommandLine, handle string options

*/
class CommandLineOptionString : public CommandLineOption
{
public:
   /*!
    \brief created via CommandLine::option

    \param name name of the parameter
    \param desc description for help message
    \param value pointer of value to set
   */
   CommandLineOptionString( const QString &name, const QString &desc, QString *value );
   virtual ~CommandLineOptionString();

   /*!
    \brief try to parse option name without argument

    \return parameter belongs to this object
    \param name name of parameter
   */
   virtual bool parse( const QString &name );
   /*!
    \brief try to parse option name with an argument

    \return parameter belongs to this object
    \param name name of parameter
    \param option value of parameter
   */
   virtual bool parse( const QString &name, const QString &option );
   /*!
    \brief generate help message for this option

    \return string describing option
   */
   virtual QString help();

private:
   Q_DISABLE_COPY( CommandLineOptionString )

   QString *mValue;
};

/*! @} */

#endif
