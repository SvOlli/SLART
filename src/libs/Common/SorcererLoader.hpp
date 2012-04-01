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
    \brief detect if Sorcerer needs to be loaded and run

    \param force force start of setup wizard, even system is already set up
   */
   static void detect();

private:
   /*!
    \brief constructor

   */
   SorcererLoader();

   /*!
    \brief destructor

   */
   virtual ~SorcererLoader();

   /*!
    \brief private copy constructor to deactive default

    \param that the other instance
   */
   SorcererLoader( const SorcererLoader &that );

   /*!
    \brief private assignment operator to deactive default

    \param that the other instance
   */
   SorcererLoader &operator=( const SorcererLoader &that );

   /*!
    \brief search for Sorcerer and try loading it

   */
   static SorcererInterface *tryLoading();

   /*!
    \brief try to load Sorcerer from the specified dir

    \param dir
   */
   static SorcererInterface *tryLoading( const QDir &dir );

   /*!
    \brief remove Sorcerer from memory

   */
   static void unload();

   static QPluginLoader *cpPluginLoader; /*!< \brief used for loading Sorcerer */
};

/*! @} */

#endif /* SORCERERLOADER_HPP */
