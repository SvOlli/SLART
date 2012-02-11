/*
 * src/apps/Partyman/PartymanMainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef PARTYMANMAINWIDGET_HPP
#define PARTYMANMAINWIDGET_HPP PARTYMANMAINWIDGET_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QPushButton;

/* forward declaration of local classes */
class ConfigDialog;
class ControlWidget;
class Database;
class PlaylistControlWidget;

/*!
  \addtogroup Partyman Partyman: dual slot audio player
  @{
  */

/*!
 \brief the central widget

 \dotfile "graphs/apps/Partyman/PartymanMainWidget_connect.dot" "Connect Graph"
*/
class PartymanMainWidget : public QWidget
{
   Q_OBJECT

public:
   /*!
    \brief constuctor

    \param parent
    \param flags
   */
   PartymanMainWidget( QWidget *parent = 0, Qt::WindowFlags flags = 0 );

   /*!
    \brief destructor

   */
   virtual ~PartymanMainWidget();

   /*!
    \brief prepare startup of application

   */
   void startUp();

protected:

public slots:
   /*!
    \brief handle request for new icon and title

    \param icon
    \param title
   */
   void changeTitle( const QIcon &icon, const QString &title );
   /*!
    \brief don't autostart if database is empty

    \param allow
   */
   void allowAutostart( bool allow );

signals:
   /*!
    \brief request a new icon and title

    \param icon
    \param title
   */
   void requestChangeTitle( const QIcon &icon, const QString &title );

private:
   Q_DISABLE_COPY( PartymanMainWidget )

   bool                    mAllowAutostart; /*!< TODO */
   QWidget                 *mpParent; /*!< TODO */
   Database                *mpDatabase; /*!< TODO */
   ConfigDialog            *mpConfig; /*!< TODO */
   PlaylistControlWidget   *mpPlaylist; /*!< TODO */
   ControlWidget           *mpControl; /*!< TODO */
};

/*! @} */

#endif
