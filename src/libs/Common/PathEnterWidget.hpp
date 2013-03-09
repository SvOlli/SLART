/*
 * src/libs/Common/PathEnterWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef PATHENTERWIDGET_HPP
#define PATHENTERWIDGET_HPP PATHENTERWIDGET_HPP

/* base class */
#include <QWidget>

/* system headers */

/* Qt headers */
#include <QString>

/* local library headers */

/* local headers */
#include "DnDFileInfo.hpp"

/* forward declaration of Qt classes */
class QDirModel;
class QLineEdit;

/* forward declaration of local classes */

/*!
  \addtogroup Common

  @{
*/

/*!
 \brief \todo complete documentation

 \dotfile "graphs/libs/Common/PathEnterWidget_connect.dot" "Connect Graph"
*/
class PathEnterWidget : public QWidget
{
   Q_OBJECT

   /*!
    * \brief passed on to QLineEdit
    *
    */
   Q_PROPERTY( QString text
               READ text
               WRITE setText )

   /*!
    * \brief title of dialog popping up then "..." is pressed
    *
    */
   Q_PROPERTY( QString browseMessage
               READ browseMessage
               WRITE setBrowseMessage )

   /*!
    * \brief directory-only mode
    *
    */
   Q_PROPERTY( bool dirOnly
               READ dirOnly
               WRITE setDirOnly )

public:
   /*!
    \brief constructor

    \param parent
   */
   PathEnterWidget( QWidget *parent );

   /*!
    \brief constructor

    \param content passed on to QLineEdit
    \param parent
   */
   PathEnterWidget( const QString &content, QWidget *parent );

   /*!
    \brief destructor

   */
   virtual ~PathEnterWidget();

   /*!
    \brief for full access to QLineEdit

    \return QLineEdit
   */
   QLineEdit *lineEdit() const;

   /*!
    \brief for full access to QDirModel of QCompleter

    \return QDirModel
   */
   QDirModel *dirModel() const;

   /*!
    \brief passed on to QLineEdit

    \return QString
   */
   QString text() const;

   /*!
    \brief returns if widget is in directory-only mode

    \return bool
   */
   bool dirOnly() const;

   /*!
    \brief set the directory-only mode

    \param value
   */
   void setDirOnly( bool value );

   /*!
    \brief get the title of the browse dialog

    \return QString
   */
   QString browseMessage() const;

   /*!
    \brief set the title of the browse dialog

    \param text
   */
   void setBrowseMessage( const QString &text );

public slots:
   /*!
    \brief passed on to QLineEdit

    \param value
   */
   void setText( const QString &value );

   /*!
    \brief open up a browse dialog

   */
   void browse();

signals:
   /*!
    \brief passed on from QLineEdit

    \param text
   */
   void textChanged( const QString &text );

   /*!
    \brief passed on from QLineEdit

    \param text
   */
   void textEdited( const QString &text );

   /*!
    \brief passed on from QLineEdit

   */
   void returnPressed();

protected:
   /*!
    \brief reimplemented to allow dropping of file/dir

    \param event
   */
   void dragEnterEvent( QDragEnterEvent *event );

   /*!
    \brief reimplemented to allow dropping of file/dir

    \param event
   */
   void dropEvent( QDropEvent *event );

private:
   Q_DISABLE_COPY( PathEnterWidget )

   /*!
    \brief \todo

   */
   void setup();

   QLineEdit      *mpLineEdit; /*!< \brief \todo TODO */
   QDirModel      *mpDirModel; /*!< \brief \todo TODO */
   bool           mDirOnly; /*!< \brief \todo TODO */
   QString        mBrowseMessage; /*!< \brief \todo TODO */
};

/*! @} */

#endif
