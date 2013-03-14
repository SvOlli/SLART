/*
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
  \addtogroup CommandLine

 @{
 */

/*!
 \brief helper for class CommandLine, basis for all other CommandLineOption* classes

*/
class CommandLineOption
{
public:
   CommandLineOption( const QString &name, const QString &type, const QString &desc );
   virtual ~CommandLineOption();

   /*!
    \brief try to parse option name without argument

    \return parameter belongs to this object
    \param name name of parameter
   */
   virtual bool parse( const QString &name ) = 0;
   /*!
    \brief try to parse option name with an argument

    \return parameter belongs to this object
    \param name name of parameter
    \param option value of parameter
   */
   virtual bool parse( const QString &name, const QString &option ) = 0;
   /*!
    \brief check if parsing was successful

   */
   virtual bool check();
   /*!
    \brief generate help message for this option

    \return string describing option
   */
   virtual QString help();

   const QString mName; /*!< \brief name of the parameter */
   const QString mType; /*!< \brief type of the parameter for help */
   const QString mDesc; /*!< \brief description of the parameter for help */

protected:
   bool          mFail; /*!< \brief indicate failure during parameter evaluation */

private:
   Q_DISABLE_COPY( CommandLineOption )
};

/*! @} */

#endif
