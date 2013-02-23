/*
 * src/libs/CommandLine/CommandLine.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef COMMANDLINE_HPP
#define COMMANDLINE_HPP COMMANDLINE_HPP

/* system headers */

/* Qt headers */
#include <QStringList>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */
class CommandLineOption;

/*!
  \addtogroup CommandLine

  \brief library: handling command line parameters

  typical usage:
  \code
   int main( int argc, char *argv[] )
   {
      QCoreApplication app( argc, argv );
      CommandLine cl;
      QString stringtest;
      QString stringdefaulttest("defaultstring");
      QStringList otherArgs;
      bool booltest = false;
      bool help = false;

      cl.option( "-help",        "show help",                &help,      true );
      cl.option( "-enablebool",  "boolean test set",         &booltest,  true );
      cl.option( "-disablebool", "boolean test unset",       &booltest,  false );
      cl.option( "-string",      "string test",              &stringtest );
      cl.option( "-default",     "string with default test", &stringdefaulttest );

      cl.parse( &otherArgs );

      if( cl.check() )
      {
         qDebug() << "fail! try '-help'";
         return 0;
      }

      if( help )
      {
         qDebug() << cl.help().toLocal8Bit().constData();
         return 0;
      }

      qDebug() << "booltest:"         << booltest;
      qDebug() << "stringtest:"       << stringtest;
      qDebug() << "stringdefaultest:" << stringdefaulttest;
      qDebug() << "other arguments:"  << otherArgs;

      return 0;
   }
  \endcode

 @{
 */

/*!
 \brief handling of command line parameters

*/
class CommandLine
{
public:
   /*!
    \brief constructor

   */
   CommandLine();

   /*!
    \brief destructor

   */
   virtual ~CommandLine();


   /*!
    \brief add an option with a boolean value (enable/disable)

    \param name name of the parameter
    \param desc description for help message
    \param value pointer of value to set
    \param setTo value to set to
   */
   void option( const QString &name, const QString &desc, bool *value, bool setTo );

   /*!
    \brief add an option with a string parameter, new value replaces old

    \param name name of the parameter
    \param desc description for help message
    \param value pointer of value to set
   */
   void option( const QString &name, const QString &desc, QString *value );
#if 0
   /*!
    \brief add an option with an integer parameter, new value replaces old

    \param name
    \param desc
    \param value
   */
   void option( const QString &name, const QString &desc, long *value );

   /*!
    \brief add an option with a string parameter, new value adds up to old ones

    \param name
    \param desc
    \param value
   */
   void option( const QString &name, const QString &desc, QStringList *value );

   /*!
    \brief add an option with an integer parameter, new value adds up to old ones

    \param name
    \param desc
    \param value
   */
   void option( const QString &name, const QString &desc, QList<long> *value );
#endif

   /*!
    \brief parse the list of commandline parameters returning non parameters

    \param nonOptions
   */
   void parse( QStringList *nonOptions );

   /*!
    \brief check if parsing was without faults

   */
   bool check();

   /*!
    \brief generate help message of all options

   */
   QString help();

private:
   QList<CommandLineOption*>    mOptions;
};

/*! @} */

#endif
