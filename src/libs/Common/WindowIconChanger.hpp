/*
 * src/libs/Common/WindowIconChanger.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef WINDOWICONCHANGER_HPP
#define WINDOWICONCHANGER_HPP WINDOWICONCHANGER_HPP

/* base class */
#include <QObject>

/* system headers */

/* Qt headers */
#include <QIcon>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QTimer;
class QWidget;

/* forward declaration of local classes */


/*!
  \addtogroup Common

  @{
*/

/*!
 \brief toggle window icon upon signal

 Primary intended for showing database activity using the main window icon. Set
 an icon that should be set when set changed(true) has been called. The current
 icon will be saved and restored calling changed(false).

 \dotfile "graphs/libs/Common/WindowIconChanger_connect.dot" "Connect Graph"
*/
class WindowIconChanger : public QObject
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param widget widget with icon to be changed
    \param icon icon to change to when changed(true) is called
    \param parent parent object
   */
   WindowIconChanger( QWidget *widget, const QIcon &icon,
                      QObject *parent = 0 );

   /*!
    \brief destructor

   */
   virtual ~WindowIconChanger();

   /*!
    \brief property to hold the icon

   */
   Q_PROPERTY( QIcon newIcon
               READ newIcon
               WRITE setNewIcon
               USER true )
   /*!
    \brief get the icon

    \return QIcon
   */
   QIcon newIcon();
   /*!
    \brief set the icon

    \param icon
   */
   void setNewIcon( const QIcon &icon );

public slots:
   /*!
    \brief handle the change to set or revert the icon

    \param enable true: set icon, false: restore icon
   */
   void changed( bool enable );

private slots:
   /*!
    \brief revert the change of the icon

    target for mpTimer
   */
   void revert();

private:
   Q_DISABLE_COPY( WindowIconChanger )

   QWidget     *mpWidget;
   QTimer      *mpTimer;
   QIcon       mNewIcon;
   QIcon       mOldIcon;
};

/*! @} */

#endif
