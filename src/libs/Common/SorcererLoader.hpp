/*
 * src/libs/Common/SorcererLoader.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef SORCERERLOADER_HPP
#define SORCERERLOADER_HPP SORCERERLOADER_HPP

/* base class */

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QApplication;
class QDir;
class QPluginLoader;

/* forward declaration of local classes */
class SorcererInterface;
class MySettings;

/*!
  \addtogroup Common
  \brief \ref SorcererLoader is described in \ref Sorcerer
  @{
  */

/*! @} */

/*!
  \addtogroup Sorcerer

  @{
  */


/*!
 \brief loader for Sorcerer setup wizard.
 Part of \ref Common

*/
class SorcererLoader
{
public:

   /*!
    \brief detect if Sorcerer needs to be loaded and run it, when necessary

   */
   static void detect();

};

/*! @} */

#endif /* SORCERERLOADER_HPP */
