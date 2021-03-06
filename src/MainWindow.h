/*
 * nheko Copyright (C) 2017  Konstantinos Sideris <siderisk@auth.gr>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <functional>

#include <QMainWindow>
#include <QSharedPointer>
#include <QStackedWidget>
#include <QSystemTrayIcon>

#include "LoginPage.h"
#include "RegisterPage.h"
#include "UserSettingsPage.h"
#include "WelcomePage.h"
#include "dialogs/UserProfile.h"
#include "ui/OverlayModal.h"

class ChatPage;
class LoadingIndicator;
class OverlayModal;
class SnackBar;
class TrayIcon;
class UserSettings;

namespace mtx {
namespace requests {
struct CreateRoom;
}
}

namespace dialogs {
class CreateRoom;
class InviteUsers;
class JoinRoom;
class LeaveRoom;
class Logout;
class MemberList;
class ReCaptcha;
class RoomSettings;
}

class MainWindow : public QMainWindow
{
        Q_OBJECT

public:
        explicit MainWindow(QWidget *parent = 0);

        static MainWindow *instance() { return instance_; };
        void saveCurrentWindowSize();

        void openLeaveRoomDialog(const QString &room_id = "");
        void openInviteUsersDialog(std::function<void(const QStringList &invitees)> callback);
        void openCreateRoomDialog(
          std::function<void(const mtx::requests::CreateRoom &request)> callback);
        void openJoinRoomDialog(std::function<void(const QString &room_id)> callback);
        void openLogoutDialog();
        void openRoomSettings(const QString &room_id = "");
        void openMemberListDialog(const QString &room_id = "");
        void openUserProfile(const QString &user_id, const QString &room_id);
        void openReadReceiptsDialog(const QString &event_id);

        void hideOverlay();
        void showSolidOverlayModal(QWidget *content,
                                   QFlags<Qt::AlignmentFlag> flags = Qt::AlignCenter);
        void showTransparentOverlayModal(QWidget *content,
                                         QFlags<Qt::AlignmentFlag> flags = Qt::AlignTop |
                                                                           Qt::AlignHCenter);

protected:
        void closeEvent(QCloseEvent *event) override;
        void resizeEvent(QResizeEvent *event) override;
        void showEvent(QShowEvent *event) override;

private slots:
        //! Show or hide the sidebars based on window's size.
        void adjustSideBars();
        //! Handle interaction with the tray icon.
        void iconActivated(QSystemTrayIcon::ActivationReason reason);

        //! Show the welcome page in the main window.
        void showWelcomePage()
        {
                removeOverlayProgressBar();
                pageStack_->addWidget(welcome_page_);
                pageStack_->setCurrentWidget(welcome_page_);
        }

        //! Show the login page in the main window.
        void showLoginPage()
        {
                if (modal_)
                        modal_->hide();

                pageStack_->addWidget(login_page_);
                pageStack_->setCurrentWidget(login_page_);
        }

        //! Show the register page in the main window.
        void showRegisterPage()
        {
                pageStack_->addWidget(register_page_);
                pageStack_->setCurrentWidget(register_page_);
        }

        //! Show user settings page.
        void showUserSettingsPage() { pageStack_->setCurrentWidget(userSettingsPage_); }

        //! Show the chat page and start communicating with the given access token.
        void showChatPage();

        void showOverlayProgressBar();
        void removeOverlayProgressBar();

private:
        void showDialog(QWidget *dialog);
        bool hasActiveUser();
        void restoreWindowSize();
        //! Check if there is an open dialog.
        bool hasActiveDialogs() const;
        //! Check if the current page supports the "minimize to tray" functionality.
        bool pageSupportsTray() const;

        static MainWindow *instance_;

        //! The initial welcome screen.
        WelcomePage *welcome_page_;
        //! The login screen.
        LoginPage *login_page_;
        //! The register page.
        RegisterPage *register_page_;
        //! A stacked widget that handles the transitions between widgets.
        QStackedWidget *pageStack_;
        //! The main chat area.
        ChatPage *chat_page_;
        UserSettingsPage *userSettingsPage_;
        QSharedPointer<UserSettings> userSettings_;
        //! Tray icon that shows the unread message count.
        TrayIcon *trayIcon_;
        //! Notifications display.
        SnackBar *snackBar_ = nullptr;
        //! Overlay modal used to project other widgets.
        OverlayModal *modal_       = nullptr;
        LoadingIndicator *spinner_ = nullptr;
};
