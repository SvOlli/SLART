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

/* forward declaration of Qt classes */
class QFileSystemModel;
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

   /*!
    * \brief when not in directory-only mode:
    *
    */
   Q_PROPERTY( QStringList nameFilters
               READ nameFilters
               WRITE setNameFilters )

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
    \brief for full access to QFileSystemModel of QCompleter

    \return QFileSystemModel
   */
   QFileSystemModel *fsModel() const;

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

   /*!
    \brief set the name filters for what files to accept when not in
    directory-only mode

    \param filters filters to set
   */
   void setNameFilters( const QStringList &filters );

   /*!
    \brief get the name filters

    \return QStringList filters
   */
   QStringList nameFilters();

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

   /*!
    \brief trigger the signal \ref newFileName

   */
   void sendFileName();

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

   /*!
    \brief signalize that a new file name has been set

    \param fileName the new fileName
   */
   void newFileName( const QString &fileName );

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

   QLineEdit         *mpLineEdit; /*!< \brief \todo TODO */
   QFileSystemModel  *mpFileSystemModel; /*!< \brief \todo TODO */
   bool              mDirOnly; /*!< \brief \todo TODO */
   QString           mBrowseMessage; /*!< \brief \todo TODO */
};

/*! @} */

#endif
