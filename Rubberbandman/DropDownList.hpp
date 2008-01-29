/**
 * DropDownList.hpp
 * written by Sven Oliver Moll
 * 
 * distributed under the terms of the GNU Public License (GPL)
 */

#ifndef DROPDOWNLIST_HPP
#define DROPDOWNLIST_HPP 1

#include <QWidget>
class QComboBox;
class QPushButton;
class QStringList;

class DropDownList : public QWidget
{
Q_OBJECT
   
public:
   DropDownList( QWidget *parent = 0 );
   virtual ~DropDownList();

signals:
   void requestNew( const QString &listEntry );
   void requestDelete( const QString &listEntry );
   void requestRename( const QString &oldListEntry, const QString &newListEntry );
   void requestChange( const QString &listEntry );

public slots:
   void loadListData( const QString &preset, const QStringList &listData );
   void handleNew();
   void handleRename();
   void handleDelete();
   void handleChange( const QString &listEntry );

private:
   DropDownList( const DropDownList &other );
   DropDownList &operator=( const DropDownList &other );

   QComboBox   *mpComboBox;
   QPushButton *mpButtonNew;
   QPushButton *mpButtonRename;
   QPushButton *mpButtonDelete;

   void showEvent( QShowEvent *event );
};

#endif
