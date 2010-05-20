 /*
 *    QRAP Project
 *
 *    Version     : 0.1
 *    Date        : 2008/04/01
 *    License     : GNU GPLv3
 *    File        : AdminWindow.h
 *    Copyright   : (c) University of Pretoria
 *    Author      : Roeland van Nieuwkerk (roeland.frans@gmail.com)
 *    Description : Qt4 custom widget that inherits QTabWidget and is responsible for
 *                  providing administration features to manage User and Group priviledges
 *                  for the Qrap system.
 *
 *
 **************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; See the GNU General Public License for      *
 *   more details                                                          *
 *                                                                         *
 ************************************************************************* */

#ifndef Qrap_AdminWindow_h
#define Qrap_AdminWindow_h

// Include global headers
#include <QWidget>
#include <QDialog>
#include <QComboBox>
#include <QFrame>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpacerItem>
#include <QStackedWidget>
#include <QTableView>
#include <QTreeWidget>
#include <QTableWidget>

#include "Config.h"


namespace Qrap
{
	/**
	* The AdminWindow class is the base class for the user and group administration dialog.
	*/
	
	class AdminWindow : public QDialog
	{
		Q_OBJECT
		
	public:
		/**
		 * Constructor.
		 */
		AdminWindow (QWidget* parent=0);
	
	public slots:
		void AdminTreeItemChanged (QTreeWidgetItem* current, QTreeWidgetItem* previous);
		void LimitersListItemClicked (QTableWidgetItem* item);
		
		void AddUserClicked ();
		void DeleteUserClicked ();
		void CommitUserClicked ();
		
		void AddLimiterClicked ();
		void DeleteLimiterClicked ();
		void CommitLimiterClicked ();
		
		void AddGroupClicked ();
		void DeleteGroupClicked ();
		void CommitGroupClicked ();
	
	private:
		QWidget         *horizontalLayout;
		QHBoxLayout     *hboxLayout;
		QTreeWidget     *mAdminTree;
		QStackedWidget  *mAdminStack;
		QWidget         *mUserAdminPage;
		QLabel          *mUserAdminLabel;
		QFrame          *line;
		QWidget         *gridLayout;
		QGridLayout     *gridLayout1;
		QLabel          *mUsernameLabel;
		QLabel          *mPasswordLabel;
		QLabel          *mConfirmLabel;
		QLabel          *mFullnamesLabel;
		QLabel          *mIDLabel;
		QLabel          *mEmailLabel;
		QLabel          *mGroupLabel;
		QLineEdit       *mUsernameEdit;
		QLineEdit       *mPasswordEdit;
		QLineEdit       *mConfirmEdit;
		QLineEdit       *mFullnamesEdit;
		QLineEdit       *mIDEdit;
		QLineEdit       *mEmailEdit;
		QComboBox       *mGroupCombo;
		QSpacerItem     *spacerItem;
		QWidget         *horizontalLayout_2;
		QHBoxLayout     *hboxLayout1;
		QPushButton     *mAddUserButton;
		QPushButton     *mDeleteUserButton;
		QPushButton     *mCommitUserButton;
		QPushButton     *mCloseUserButton;
		QWidget         *mGroupAdminPage;
		QFrame          *line_2;
		QLabel          *mGroupAdminLabel;
		QWidget         *horizontalLayout_3;
		QHBoxLayout     *hboxLayout2;
		QLabel          *mGroupNameLabel;
		QLineEdit       *mGroupNameEdit;
		QGroupBox       *mLimiterGroupBox;
		QTableWidget    *mLimitersTable;
		QLabel          *mExistingLimitersLabel;
		QWidget         *gridLayout_2;
		QGridLayout     *gridLayout2;
		QComboBox       *mTableCombo;
		QLabel          *mTableLabel;
		QLabel          *mCommandLabel;
		QLabel          *mLimiterLabel;
		QLabel          *mDoLabel;
		QLineEdit       *mLimiterEdit;
		QComboBox       *mCommandCombo;
		QLineEdit       *mDoEdit;
		QHBoxLayout     *hboxLayout3;
		QPushButton     *mCommitLimiterButton;
		//QPushButton     *mResetLimiterButton;
		QHBoxLayout     *hboxLayout4;
		QPushButton     *mAddLimiterButton;
		QPushButton     *mDeleteLimiterButton;
		QSpacerItem     *spacerItem1;
		QWidget         *horizontalLayout_6;
		QHBoxLayout     *hboxLayout5;
		QPushButton     *mAddGroupButton;
		//QPushButton     *mDuplicateGroupButton;
		QPushButton     *mDeleteGroupButton;
		QPushButton     *mCommitGroupButton;
		QPushButton     *mCloseButton;
		QTreeWidgetItem* mUsersTreeItem;
		QTreeWidgetItem* mGroupsTreeItem;
		
		/**
		 * Sets the user admin form into addition mode.
		 */
		void SetAddUserMode (bool selectUsers = true);
		
		/**
		 * Readies the controls in the user form for modification of the given user.
		 */
		void SetModifyUserMode (const QString& username);
		
		/**
		 * Sets the group admin form into addition mode.
		 */
		void SetAddGroupMode (bool selectGroups = true);
		
		/**
		 * Readies the controls in the group form for modification of the given group.
		 */
		void SetModifyGroupMode (const QString& groupname);
		
		/**
		 * Populates the users' usernames in all the various places where they should go.
		 */
		void PopulateUsers ();
		
		/**
		 * Populates the groups' names in all the various places where they should go.
		 */
		void PopulateGroups ();
		
		/**
		 * Populates the tables' names on the "Group Admin" page.
		 */
		void PopulateTables ();
		
		/**
		 * Clears the limiter entry fields' contents.
		 */
		void ClearLimiterFields ();
	};
}
#endif
