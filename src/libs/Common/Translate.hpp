/* * src/libs/Common/Translate.hpp
 * written by Sven Oliver Moll
*
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef TRANSLATE_HPP
#define TRANSLATE_HPP TRANSLATE_HPP

/* base class */
#include <QObject>

/* system headers */

/* Qt headers */
#include <QCoreApplication>
#include <QList>
#include <QLocale>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QAction;
class QActionGroup;
class QString;
class QTranslator;

/* forward declaration of local classes */


/*!
  \addtogroup Common

  @{
*/

/*!
 \brief wrapper for handling multiple translation sources

*/
class Translate : public QObject
{
   Q_OBJECT

public:
   /*!
    \brief constructor

   */
   Translate(QObject *parent);
   /*!
    \brief destructor

   */
   virtual ~Translate();

   /*!
    \brief install the default translators

    \param lang language to switch to
   */
   void install( const QString &lang = QLocale::system().name() );

   /*!
    \brief install an additional translator

    \param catalog base name of .ts file to install
    \param lang language to switch to
   */
   void load( const QString &catalog,
              const QString &lang = QLocale::system().name() );

   /*!
    \brief generate a group of available languages

    To change translation on the fly, overload changeEvent( QEvent* event ),
    and check for event->type() == QEvent::LanguageChange to update all tr().

    \return QActionGroup
   */
   static QActionGroup *langGroup();

public slots:
   /*!
    \brief slot for actiongroup created by \ref langGroup

    \param action
   */
   void languageChanged( QAction *action );

private:
   Q_DISABLE_COPY( Translate )

   /*!
    \brief get the location of own translation files

   */
   static QString location();
};

/*! @} */

#endif
